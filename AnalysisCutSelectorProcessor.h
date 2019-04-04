#ifndef ANALYSISCUTSELECTORPROCESSOR_H
#define ANALYSISCUTSELECTORPROCESSOR_H

// C++
#include <string>

// LCIO
#include "lcio.h"
#include <EVENT/TrackerHit.h>
#include <EVENT/LCCollection.h>
#include <EVENT/Track.h>
#include <IMPL/LCCollectionVec.h>
#include <IMPL/TrackImpl.h>
#include <IMPL/LCFlagImpl.h>

// Marlin
#include "marlin/Processor.h"
#include "gear/TPCParameters.h"
//ROOT
#include "TH1F.h"
#include "TNtuple.h"

// Aida
#ifdef MARLIN_USE_AIDA
//AIDA
#include <marlin/AIDAProcessor.h>
#endif

namespace marlintpc{

    /**  Applies cuts on all relevant track parameters.
     *
     *   All cut parameters are optional parameters. If you don't give a value in the steering file
     *   the cut is not applied. The default values are meaningless.
     *
     *   The output collection is a subset of the input collection, which means only the pointers
     *   to the tracks are stored. Do not remove the input collection from the file.
     *   If the output collection is non transient also the input collection has to be non transient.
     *
     *  <h4>Input - Prerequisites</h4>
     *  \li A collection of tracks (lcio::Track). <br>
     *
     *
     *  <h4>Output</h4>
     *  \li A collection of tracks that fulfill the cut criteria
     *
     * @param InputCollectionName: Name of input collection of tracks, default TPCTracks
     * @param OutputCollectionNam: Name of the output collection
     * @param SetOutputTransient: if not 0 the output collection is set transient, default 0
     * @param lowCutNTracks: lower cut on number of track
     * @param highCutNTracks: upper cut on number of tracks
     * @param lowCutNHitsOnTrack: lower cut on number of hits on track
     * @param highCutNHitsOnTrack: upper cut on number of hits on track
     * @param lowCutPhi: lower cut on phi
     * @param highCutPhi: upper cut on phi
     * @param lowCutTanLamba: lower cut on tan lambda
     * @param highCutTanLambda: upper cut on tan lambda
     * @param lowCutdEdx: lower cut on dEdx
     * @param highCutdEdx: upper cut on dEdx
     * @param lowCutD0: lower cut on D0
     * @param highCutD0: upper cut on D0
     * @param lowCutZ0: lower cut on Z0
     * @param highCutZ0: upper cut on Z0
     * @param lowCutOmega: lower cut on Omega
     * @param highCutOmega: upper cut on Omega
     * @param CutIfFitFailed: cut if fit failed for a track (default: false)
     * @param zPosHistogramMax: maximum for z-position histogram range (default: 600)
     *
     * @author F. Mueller
     */
  
  
    class AnalysisCutSelectorProcessor : public marlin::Processor {

    public:

	/* This method will be called by the marlin package
	 * It returns a processor of the current type (here AnalysisCutSelectorProcessor)
	 */
	virtual Processor*  newProcessor() { return new AnalysisCutSelectorProcessor ; }

	AnalysisCutSelectorProcessor() ;

	virtual void init() ;

	virtual void processRunHeader( lcio::LCRunHeader* run ) ;

	virtual void processEvent( lcio::LCEvent * evt ) ;

	virtual void end();

	protected:

	std::string _inputColName ;   ///<- the name of the input collection
	std::string _outputColName ;  ///<- the name of the output collection

	/** give the status which is set to the transient flag of the output collection
	 * it is an int instead of a boolean, because the processor parameter cannot be a boolean
	 */
	int _outputIsTransient;

	// event specific cuts
	int _lowcutNTracks;   ///<- lower cut on number of tracks
	int _highcutNTracks;  ///<- upper cut on number of tracks

	// track specific cuts
	int _lowcutNHitsOnTrack;   ///<- lower cut on number of hits on track
	int _highcutNHitsOnTrack;  ///<- upper cut on number of hits on track
	float _lowcutPhi;          ///<- lower cut on phi
	float _highcutPhi;         ///<- upper cut on phi
	float _lowcutChiSq;          ///<- lower cut on Chi square
	float _highcutChiSq;         ///<- upper cut on Chi square
	float _lowcutTanLambda;    ///<- lower cut on tan lambda
	float _highcutTanLambda;   ///<- upper cut on tan lambda
	float _lowcutdEdx;         ///<- lower cut on dEdx (track)
	float _highcutdEdx;        ///<- upper cut on dEdx (track)
	float _lowcutOmega;        ///<- lower cut on omega
	float _highcutOmega;       ///<- upper cut on omega
	float _lowcutD0;           ///<- lower cut on D0
	float _highcutD0;          ///<- upper cut on D0
	float _lowcutZ0;           ///<- lower cut on Z0
	float _highcutZ0;          ///<- upper cut on Z0
	double _runZOverride;       ///<- Optional user-supplied Z
	double _runZErrorOverride;  ///<- Optional user-supplied Z error
	bool  _cutIfFitFailed;     ///< cut if fit failed for a track
	int _bestModule; ///< Module with the best row
	int _bestRow; ///< Row with the best resolution

	float _zPosHistogramMax; ///< optional parameter: maximum for z-position histogram range (in case the drift range is larger than the default 600)

    private:
	TH1F * _cutFlowEvents;
	TH1F * _cutFlowTracks;
	TH1F * _hitZPosition;
	TH1F * _hitTime;
	TNtuple* _runZPosTuple;
	TNtuple* _runHitTimeTuple;
	// angle between the hit and the track
	TH1F * _hitPhi;
	TNtuple* _runHitPhiTuple;
	TH1F * _hitPhiBestRow;
	TNtuple* _runHitPhiBestRowTuple;

    const gear::TPCParameters* _gearParameters;

    } ;


}

#endif // ANALYSISCUTSELECTORPROCESSOR_H

