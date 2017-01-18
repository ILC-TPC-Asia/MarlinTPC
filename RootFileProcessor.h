#ifndef RootFileProcessor_h
#define RootFileProcessor_h 1

#include "marlin/Processor.h"
#include "lcio.h"
#include <string>


using namespace lcio ;
using namespace marlin ;

#ifdef MARLIN_USE_AIDA
namespace AIDA {
  class ITuple;
}
#endif

namespace gear {
  class TPCModule;
}

namespace EVENT {
  class TrackerHit;
}

class RootFileProcessor : public Processor {
  
 public:
  
  virtual Processor*  newProcessor() { return new RootFileProcessor ; }
  
  
  RootFileProcessor() ;
  
  /** Called at the begin of the job before anything is read.
   * Use to initialize the processor, e.g. book histograms.
   */
  virtual void init() ;
  
  /** Called for every run.
   */
  virtual void processRunHeader( LCRunHeader* run ) ;
  
  /** Called for every event - the working horse.
   */
  virtual void processEvent( LCEvent * evt ) ; 
  
  
  virtual void check( LCEvent * evt ) ; 
  
  
  /** Called after data processing for clean up.
   */
  virtual void end() ;
  
  
 protected:

  /** Input collection name.
   */
  std::string _colName ;

  /** Output ROOT file name
   */
  std::string _rootfilename;

  /** EXTPCKalDetector
   */

#ifdef MARLIN_USE_AIDA
  /** Used to prepare Ntuple
   */
  void setNtuple();

  /** Ntuple for analysis 
   */
  AIDA::ITuple *_clrs ; // clusters
  AIDA::ITuple *_hits ; // hits
  AIDA::ITuple *_trks ; // tracks
  AIDA::ITuple *_hitmap ;
#endif

  int _nRun ;
  int _nEvt ;

  std::vector<int> _modulesToUseOverride;
  std::vector<int> _badRowsOverride;

  int getModuleID( TrackerHit* hitp ) ;
  int getRowID( EVENT::TrackerHit* hitp, const gear::TPCModule* tpcmodule ) ;
} ;

#endif



