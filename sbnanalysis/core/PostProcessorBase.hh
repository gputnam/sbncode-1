#ifndef __sbnanalysis_core_PostProcessorBase__
#define __sbnanalysis_core_PostProcessorBase__

/**
 * \file PostProcessorBase.hh
 *
 * A generic post-processor that reads an sbncode TTree
 *
 * Author: G. Putnam <gputnam@uchicago.edu>, 2018/10/08
 */

#include <thread>
#include <string>
#include <vector>
#include "gallery/Event.h"
#include "Loader.hh"
#include "Event.hh"
#include "SubRun.hh"
#include "ProviderManager.hh"
#include "FileMeta.hh"

class TBranch;
class TFile;
class TTree;
template<typename Param>
class TParameter;

namespace event {
  class Event;
}

namespace fhicl {
  class ParameterSet;
}

/** Core framework functionality. */
namespace core {

/**
 * \class core::PostProcessorBase
 * \brief A generic tree-reading event-by-event processor.
 */
class PostProcessorBase {
public:
  /** Constructor */
  PostProcessorBase();

  /** Destructor */
  virtual ~PostProcessorBase();

  /**
   * Run
   *
   * \param filelist The list of files to be processed by this PostProcessor
   */
  void Run(std::vector<std::string> filelist);

  /**
   * Perform user-level initialization.
   *
   * \param config A configuration, as a JSON filename.
   */
  void Initialize(char* config=NULL, const std::string &output_fname="", unsigned n_workers=1);

protected:
  /**
   * Process one event.
   *
   * \param event The sbncode event for the current event
   */
  virtual void ProcessEvent(const event::Event *event) = 0;

  virtual void ProcessSubRun(const SubRun *subrun) {}

  virtual void ProcessFileMeta(const FileMeta *filemeta) {}

  /**
   * Setup anything needed per file
   *
   * \param f the TFile being opened
   * \param eventTree the TTree associated with the sbncode event.
   * Use this TTree to set branch addresses for everything other than
   * the sbncode event.
   *
   * Files are guaranteed to be processed in the order they are specified on
   * the command line for sbn-postprocess
   */
  virtual void FileSetup(TFile *f, TTree *eventTree) {}

  /**
   * Any cleanup needed per file
   *
   * \param eventTree the TTree associated with the sbncode event.
   */
  virtual void FileCleanup(TTree *eventTree) {}

  /**
   * Perform user-level initialization.
   *
   * \param config A configuration, as a JSON object.
   */
  virtual void Initialize(fhicl::ParameterSet* config=NULL) = 0;

  /**
 * Perform user-level initialization per-thread. Only 
 * called when multiple workers are specified on the
 * command line.
 */
  virtual void InitializeThread() {}

  /** Perform user-level finalization. Called after all events have been processed. */
  virtual void Finalize() {}

  ProviderManager* fProviderManager;  //!< Interface for provider access
  int fConfigExperimentID;

  unsigned NWorkers() { return fNWorkers; }
  unsigned WorkerID();

private:
  void ProcessFile(const std::string &fname);

  unsigned fNWorkers;
  std::vector<std::thread::id> fThreadIDs;
};

}  // namespace core


/** Macro to create plugin library for user-defined PostProcessors. */
#define DECLARE_SBN_POSTPROCESSOR(classname) extern "C" { \
core::PostProcessorBase* CreatePostProcessorObject() { return new classname; } \
void DestroyPostProcessorObject(core::PostProcessorBase* o) { delete o; } \
struct core::export_table_postprocess exports = { CreatePostProcessorObject, DestroyPostProcessorObject };}

#endif  // __sbnanalysis_core_PostProcessorBase__

