#include "RootFileProcessor.h"

// STL
#include <iostream>
#include <string>

// LCIO
#include <EVENT/LCCollection.h>
#include <EVENT/Track.h>
#include <IMPL/TrackImpl.h>
#include <IMPL/TrackerHitImpl.h>
#include <IMPL/TrackerPulseImpl.h>

// GEAR
#include "gear/GEAR.h"
#include "gear/TPCParameters.h"
#include "gear/gearimpl/TPCModuleImpl.h"
#include "gearxml/GearXML.h"

// KalTest
#include "kaltest/TKalTrackSite.h"

// KalDetecotr, MeasLayer and KalHit
#include "kaldet/EXTPCKalDetector.h"
#include "kaldet/EXTPCMeasLayer.h"
#include "kaldet/EXTPCHit.h"
#include "EXHYBTrack.h"

// global constants from Marlin
#include "marlin/Global.h"

// ROOT
#include "TMath.h"
#include "TVector3.h"
#include "TObjArray.h"

#ifdef MARLIN_USE_AIDA
#include <marlin/AIDAProcessor.h>
#include <AIDA/ITupleFactory.h>
#include <AIDA/ITuple.h>
#endif
// ----- include for verbosity dependend logging ---------
#include "marlin/VerbosityLevels.h"


using namespace lcio ;
using namespace marlin ;


const int NMAXROWS = 84; // LP TPC case

static const bool gkDir = kIterBackward ;
static EXTPCKalDetector* _kaldetPtr = 0 ;

RootFileProcessor aRootFileProcessor ;


RootFileProcessor::RootFileProcessor() : Processor("RootFileProcessor") {
  
  // modify processor description
  _description = "RootFileProcessor makes ROOT file for TPC data analysis" ;
  

  // register steering parameters: name, description, class-variable, default value

  registerInputCollection( LCIO::TRACK,
			   "CollectionName" , 
			   "Name of the TPCTracks collection"  ,
			   _colName ,
			   std::string("TPCTracks") ) ;

	registerOptionalParameter("ModulesToUseOverride",
			"Optional: modules to use",
			_modulesToUseOverride,
			_modulesToUseOverride) ;

	registerOptionalParameter("RowsToSkipOverride",
			"Optional: rows to skip",
			_badRowsOverride,
			_badRowsOverride) ;
}


void RootFileProcessor::init() { 

  streamlog_out(DEBUG) << "   init called  " 
		       << std::endl ;
  
  // set Ntuple used for TPC analysis
  setNtuple() ;

  // usually a good idea to
  printParameters() ;

  // set kaldet
  _kaldetPtr = EXTPCKalDetector::GetInstance() ;

  _nRun = 0 ;
  _nEvt = 0 ;
  
}

void RootFileProcessor::processRunHeader( LCRunHeader* run) { 

  _nRun++ ;
} 

void RootFileProcessor::processEvent( LCEvent * evt ) { 

  _nEvt ++ ;

  EVENT::LCCollection* colTracks = 0;
  try {
    colTracks = evt->getCollection( _colName ) ;
  } catch ( std::exception& ) {
    std::cerr << "This event has no Track collection ! Skipped." << std::endl;
    return;
  } 

  // Get gear information
  gear::TPCParameters const &theTPCParameters = marlin::Global::GEAR->getTPCParameters() ;
  int nMod = theTPCParameters.getNModules() ;
  std::vector< const gear::TPCModule* > moduleVec;
  for ( int i_mod = 0; i_mod < nMod ; i_mod++ )
    moduleVec.push_back( &theTPCParameters.getModule( i_mod ) ) ;

  int nTrks = colTracks->getNumberOfElements() ;
  for ( int i_trk = 0 ; i_trk < nTrks ; i_trk++ ) {
    std::cerr << "Track:" << i_trk << std::endl;
    IMPL::TrackImpl* trkp 
      = static_cast< IMPL::TrackImpl* >( colTracks->getElementAt( i_trk ) ) ;

    TObjArray kalhits ;
    kalhits.SetOwner() ;

    int nhits[NMAXROWS];
    for ( int i = 0; i < NMAXROWS; i++ ) nhits[i] = 0;
    const EVENT::TrackerHitVec& hitvec = trkp->getTrackerHits() ;
    for ( std::vector< EVENT::TrackerHit* >::const_iterator i_hit = hitvec.begin() ;
                                                            i_hit != hitvec.end() ;
							    i_hit++ ) {
      int modID = getModuleID( *i_hit ) ; 
      if (_modulesToUseOverride.end() == find(_modulesToUseOverride.begin(), _modulesToUseOverride.end(), modID)) continue;
      int rowID = getRowID( static_cast< EVENT::TrackerHit* >( *i_hit ) ,
		            static_cast< const gear::TPCModule* >( moduleVec[ modID ]) ) ;
      int superlayer = ( modID < 2 ? 0 : ( modID < 5 ? 1 : 2 ) ) ;
      int rowSerial  = superlayer * moduleVec[ modID ]->getNRows() + rowID ; 
      nhits[rowSerial]++;
    }
    for ( int i = 0; i < NMAXROWS; i++ ) { 
      _hitmap->fill( 0 , i ) ;
      _hitmap->fill( 1 , nhits[i] ) ;
      _hitmap->fill( 2 , _nEvt ) ;
      _hitmap->fill( 3 , i_trk ) ;
      _hitmap->addRow() ;
    }

    // Fit track again to apply additional cut and to get residual distribution 
    for ( std::vector< EVENT::TrackerHit* >::const_iterator i_hit = hitvec.begin() ;
                                                            i_hit != hitvec.end() ;
							    i_hit++ ) {
      int modID = getModuleID( *i_hit ) ; 
      int rowID = getRowID( static_cast< EVENT::TrackerHit* >( *i_hit ) ,
		            static_cast< const gear::TPCModule* >( moduleVec[ modID ]) ) ;
      int superlayer = ( modID < 2 ? 0 : ( modID < 5 ? 1 : 2 ) ) ;
      int rowSerial  = superlayer * moduleVec[ modID ]->getNRows() + rowID ; 


      IMPL::TrackerHitImpl* const hitp 
        = static_cast< IMPL::TrackerHitImpl* const >( *i_hit ) ;
      const double* hpos = hitp->getPosition() ;
      TVector3 xv( hpos[0], hpos[1], hpos[2] ) ;

      EXTPCMeasLayer* measlayerPtr
        = static_cast< EXTPCMeasLayer* >( _kaldetPtr->At( rowSerial ) );
      double meas[2];
      double dmeas[2];
      int side = +1;
      TKalMatrix mv = measlayerPtr->XvToMv( xv , side) ;
      double     d  = mv( 1 , 0 ) ;
      meas[0] = mv( 0 , 0 ) ;
      meas[1] = d;
      dmeas[0] = measlayerPtr->GetSigmaX(d) ;
      dmeas[1] = measlayerPtr->GetSigmaZ(d) ;
      if (_badRowsOverride.end() != find(_badRowsOverride.begin(), _badRowsOverride.end(), rowID)) {
	      dmeas[0] *= 100000.;
	      dmeas[1] *= 100000.;
      }
      double b = EXTPCKalDetector::GetBfield() ;
      double v = EXTPCKalDetector::GetVdrift() ;
      kalhits.Add( new EXTPCHit( *measlayerPtr,
			          meas,
				  dmeas,
				  side,
				  v,
				  static_cast< void* >( hitp ),
				  b )) ;

    } // end of hits loop

    int i1 , i2 , i3 ; // (i1,i2,i3) = (1st,mid,last) hit to filter
    if (gkDir == kIterBackward) {
      i3 = 1 ; 
      i1 = kalhits.GetEntries() -2 ;
      i2 = i1 /2;
    } else {
      i1 = 1 ;
      i3 = kalhits.GetEntries() - 2 ; 
      i2 = i3 / 2 ;
    }

    TVTrackHit* ht1Ptr = static_cast< TVTrackHit* >( kalhits.At(i1) ) ;
    TVTrackHit* htdPtr = new EXTPCHit( *static_cast< EXTPCHit* >( ht1Ptr) ) ;
    TVTrackHit& hitd   = *htdPtr ;
    hitd(0,1) = 1.e6;
    hitd(1,1) = 1.e6;
    TKalTrackSite& sited = *new TKalTrackSite( hitd ) ;
    sited.SetHitOwner() ;
    sited.SetOwner() ;
    TVTrackHit& h1 = *static_cast< TVTrackHit* >( kalhits.At(i1) ) ;
    TVTrackHit& h2 = *static_cast< TVTrackHit* >( kalhits.At(i2) ) ;
    TVTrackHit& h3 = *static_cast< TVTrackHit* >( kalhits.At(i3) ) ;
    TVector3    x1 = h1.GetMeasLayer().HitToXv(h1);
    TVector3    x2 = h2.GetMeasLayer().HitToXv(h2);
    TVector3    x3 = h3.GetMeasLayer().HitToXv(h3);
    THelicalTrack helstart(x1,x2,x3,h1.GetBfield(),gkDir);
    static TKalMatrix svd( kSdim , 1 ) ;
    svd( 0 , 0 ) = 0. ;
    svd( 1 , 0 ) = helstart.GetPhi0() ;
    svd( 2 , 0 ) = helstart.GetKappa() ;
    svd( 3 , 0 ) = 0. ;
    svd( 4 , 0 ) = helstart.GetTanLambda() ;
    if ( kSdim == 6 ) svd( 5 , 0 ) = 0. ; // t0
    static TKalMatrix C( kSdim , kSdim ) ;
    for ( int i = 0; i < kSdim; i++) C(i,i) = 1.e4 ; // dummy error matrix
    sited.Add(new TKalTrackState( svd , C , sited , TVKalSite::kPredicted ) ) ;
    sited.Add(new TKalTrackState( svd , C , sited , TVKalSite::kFiltered ) ) ;
    EXHYBTrack kaltrack ; // a track is a kal system
    kaltrack.SetOwner() ;
    kaltrack.Add( &sited ) ;

    
    TIter nexthit( &kalhits , gkDir ) ;
    TVTrackHit *hitPtr = 0 ;
    while ( ( hitPtr = static_cast< TVTrackHit *>( nexthit() ) ) ) {
      TKalTrackSite &site = *new TKalTrackSite( *hitPtr ) ;

      // FIXME
      // Temporary remove multi hits in a row 
      // Only for beam test analysis
      int rowSerial = site.GetHit().GetMeasLayer().GetIndex();
      if ( nhits[rowSerial] != 1 ) {
	delete &site;
        continue;
      }
      
      if ( !kaltrack.AddAndFilter(site) ) {
        streamlog_out(DEBUG2) << "Kalman Filter: site discarded!" << std::endl;
	delete &site;
      } 

    }
   
    kaltrack.SmoothBackTo(1);

    int    ndf  = kaltrack.GetNDF() ;
    double chi2 = kaltrack.GetChi2() ;
    double fi0  = kaltrack.GetCurSite().GetCurState()(1, 0); 
    double cpa  = kaltrack.GetCurSite().GetCurState()(2, 0);
    double tnl  = kaltrack.GetCurSite().GetCurState()(4, 0);

    _trks->fill( 0 , ndf  ) ;
    _trks->fill( 1 , chi2 ) ;
    _trks->fill( 2 , fi0  ) ;
    _trks->fill( 3 , cpa  ) ;
    _trks->fill( 4 , tnl  ) ;
    _trks->fill( 5 , nTrks  ) ;
    _trks->addRow() ;

    TIter nextlayer( &kaltrack ) ;
    nextlayer() ;
    TKalTrackSite* sitep;
    while ( ( sitep = static_cast< TKalTrackSite* >( nextlayer() ) ) ) {
      const TrackerHit* hp 
        = static_cast<const TrackerHit* >(static_cast<const EXTPCHit& >(sitep->GetHit()).GetHitPtr() ) ;
      int index = sitep->GetHit().GetMeasLayer().GetIndex();
      double dxin = sitep->GetResVec()(0,0);
      double dzin = sitep->GetResVec()(1,0);
      double dxot = 999999.;
      double dzot = 999999.;
      double chg  = hp->getEDep() ;
      double fi0loc = sitep->GetCurState()(1, 0);

      int modID = getModuleID( const_cast< EVENT::TrackerHit* >( hp ) ) ; 
      int rowID = getRowID( const_cast< EVENT::TrackerHit* >( hp ) ,
		            static_cast< const gear::TPCModule* >( moduleVec[ modID ]) ) ;
      const double* hpos = hp->getPosition() ;
      TVector3 xv( hpos[0], hpos[1], hpos[2] ) ;
      float rc     = xv.Perp() ;
      float phic   = TMath::ATan2( xv.X() , xv.Y() ) ;
      const EVENT::LCObjectVec& pulseVec = hp->getRawHits() ;
      
      //--2018.11.21--//
      gear::Vector2D offset = moduleVec[ modID ]->getOffset(); //offsetにmoduleの中心座標(r,phi)を入れる
      double mcx = offset[0] * cos(offset[1]);
      double mcy = offset[0] * sin(offset[1]);
      TVector3 mcv( mcx, mcy, 0.) ;
      TVector3 xyv( xv.X(), xv.Y(), 0.) ; //
      TVector3 mctoxv = xyv - mcv;

      //THelicalTrack helix = static_cast<TKalTrackState>(sitep->GetCurState()).GetHelix();
      const TKalTrackState &curstate = static_cast<TKalTrackState>(sitep->GetCurState());
      std::cerr << "cuestate = " << (const void *) & curstate << std::endl;
      curstate.DebugPrint();
      THelicalTrack helix(curstate, sitep->GetPivot(), sitep->GetBfield());

      TMatrixD dxdphi(3,1);
      dxdphi = helix.CalcDxDphi(0.); //hit点でのhelixの接線ベクトル
      TVector3 tanv(dxdphi[0][0], dxdphi[1][0], 0.);

      TVector3 extprod = (tanv.Unit()).Cross(mctoxv.Unit());
      double sphi = extprod.Z();
      double cphi = (tanv.Unit())*(mctoxv.Unit());
      double incidentphi = TMath::ATan2( sphi , cphi );

      //--end--//


      sitep->InvFilter() ;
      dxot = sitep->GetResVec( TVKalSite::kInvFiltered)(0,0) ;
      dzot = sitep->GetResVec( TVKalSite::kInvFiltered)(1,0) ;
      _hits->fill( 0 , index ) ;
      _hits->fill( 1 , dxin  ) ;
      _hits->fill( 2 , dxot  ) ;
      _hits->fill( 3 , dzin  ) ;
      _hits->fill( 4 , dzot  ) ;
      _hits->fill( 5 , chg   ) ;
      _hits->fill( 6 , ndf   ) ;
      _hits->fill( 7 , chi2  ) ;
      _hits->fill( 8 , fi0   ) ;
      _hits->fill( 9 , cpa   ) ;
      _hits->fill( 10 , tnl   ) ;
      _hits->fill( 11 , nhits[index] ) ;
      _hits->fill( 12 , nTrks ) ;
      _hits->fill( 13 , fi0loc ) ;
      _hits->fill( 14 , incidentphi ) ;
      _hits->addRow() ;
     
       
      for ( std::vector< EVENT::LCObject* >::const_iterator i_pls  = pulseVec.begin() ;
                                                            i_pls != pulseVec.end() ;
							    i_pls++ ) {
        IMPL::TrackerPulseImpl* const plsp
          = static_cast< IMPL::TrackerPulseImpl* const >( *i_pls ) ;
        int padID = plsp->getCellID0() ;
	gear::Vector2D ppos = moduleVec[ modID ]->getPadCenter( padID ) ;
	TVector2 xy( ppos[1], ppos[0] ) ;
	// Convert to polar coodinate
	float r   = xy.Mod() ;
	float phi = TMath::ATan2( xy.Y(), xy.X() ) ;
	float dt  = plsp->getTime() ;

        float hitchg  = hp->getEDep() ;
        float normchg = plsp->getCharge()/hitchg ;
	float drphi   = r * phi - rc * phic ;

        _clrs->fill( 0 , modID   ) ;
        _clrs->fill( 1 , rowID   ) ;
        _clrs->fill( 2 , normchg ) ;
        _clrs->fill( 3 , drphi   ) ;
        _clrs->fill( 4 , dt      ) ;
        _clrs->fill( 5 , hitchg  ) ;
        _clrs->fill( 6 , ndf     ) ;
        _clrs->fill( 7 , chi2    ) ;
        _clrs->fill( 8 , cpa     ) ;
        _clrs->fill( 9 , tnl     ) ;
        _clrs->fill( 10 , nTrks     ) ;
        _clrs->fill( 11 , fi0loc     ) ;

	_clrs->addRow() ;

      } // endl of pulses loop
    }
  }//end of tracks loop

}



void RootFileProcessor::check( LCEvent * evt ) { 
  // nothing to check here - could be used to fill checkplots in reconstruction processor
}


void RootFileProcessor::end(){ 
  
//   std::cout << "RootFileProcessor::end()  " << name() 
// 	    << " processed " << _nEvt << " events in " << _nRun << " runs "
// 	    << std::endl ;

}

#ifdef MARLIN_USE_AIDA
void RootFileProcessor::setNtuple() {

  /** For storing cluster data 
   */
  std::vector<std::string> _clrsItemNames;
  std::vector<std::string> _clrsItemType;

  /** For storing hit data 
   */
  std::vector<std::string> _hitsItemNames;
  std::vector<std::string> _hitsItemType;

  /** For storing track data 
   */
  std::vector<std::string> _trksItemNames;
  std::vector<std::string> _trksItemType;

  /** For hit efficiency 
   */
  std::vector<std::string> _hitmapItemNames;
  std::vector<std::string> _hitmapItemType;

  _clrsItemNames.push_back("module");
  _clrsItemType.push_back("int");

  _clrsItemNames.push_back("row");
  _clrsItemType.push_back("int");

  _clrsItemNames.push_back("normchg");
  _clrsItemType.push_back("float");

  _clrsItemNames.push_back("drphi");
  _clrsItemType.push_back("float");

  _clrsItemNames.push_back("dt");
  _clrsItemType.push_back("float");

  _clrsItemNames.push_back("chgsum");
  _clrsItemType.push_back("float");

  _clrsItemNames.push_back("ndf");
  _clrsItemType.push_back("int");

  _clrsItemNames.push_back("chi2");
  _clrsItemType.push_back("float");

  _clrsItemNames.push_back("cpa");
  _clrsItemType.push_back("float");

  _clrsItemNames.push_back("tnl");
  _clrsItemType.push_back("float");

  _clrsItemNames.push_back("nTrks");
  _clrsItemType.push_back("int");

  _clrsItemNames.push_back("fi0loc");
  _clrsItemType.push_back("double");

  _hitsItemNames.push_back("serialrow");
  _hitsItemType.push_back("int");

  _hitsItemNames.push_back("dxin");
  _hitsItemType.push_back("double");

  _hitsItemNames.push_back("dxot");
  _hitsItemType.push_back("double");

  _hitsItemNames.push_back("dzin");
  _hitsItemType.push_back("double");

  _hitsItemNames.push_back("dzot");
  _hitsItemType.push_back("double");

  _hitsItemNames.push_back("chg");
  _hitsItemType.push_back("double");

  _hitsItemNames.push_back("ndf");
  _hitsItemType.push_back("int");

  _hitsItemNames.push_back("chi2");
  _hitsItemType.push_back("double");

  _hitsItemNames.push_back("fi0");
  _hitsItemType.push_back("double");

  _hitsItemNames.push_back("cpa");
  _hitsItemType.push_back("double");

  _hitsItemNames.push_back("tnl");
  _hitsItemType.push_back("double");

  _hitsItemNames.push_back("nhits");
  _hitsItemType.push_back("int");

  _hitsItemNames.push_back("nTrks");
  _hitsItemType.push_back("int");

  _hitsItemNames.push_back("fi0loc");
  _hitsItemType.push_back("double");

  _hitsItemNames.push_back("incidentphi");
  _hitsItemType.push_back("double");

  _trksItemNames.push_back("ndf");
  _trksItemType.push_back("int");

  _trksItemNames.push_back("chi2");
  _trksItemType.push_back("double");

  _trksItemNames.push_back("fi0");
  _trksItemType.push_back("double");

  _trksItemNames.push_back("cpa");
  _trksItemType.push_back("double");

  _trksItemNames.push_back("tnl");
  _trksItemType.push_back("double");

  _trksItemNames.push_back("nTrks");
  _trksItemType.push_back("int");

  _hitmapItemNames.push_back("serialrow");
  _hitmapItemType.push_back("int");

  _hitmapItemNames.push_back("nhits");
  _hitmapItemType.push_back("int");

  _hitmapItemNames.push_back("evt");
  _hitmapItemType.push_back("int");

  _hitmapItemNames.push_back("trkid");
  _hitmapItemType.push_back("int");


  _clrs = AIDAProcessor::tupleFactory(this)
	    ->create("clrs", "cluster data", _clrsItemNames, _clrsItemType, "" );

  _hits = AIDAProcessor::tupleFactory(this)
	    ->create("hits", "hit data", _hitsItemNames, _hitsItemType, "" );

  _trks = AIDAProcessor::tupleFactory(this)
	    ->create("trks", "track data", _trksItemNames, _trksItemType, "" );

  _hitmap = AIDAProcessor::tupleFactory(this)
	      ->create( "hitmap", "hit map", _hitmapItemNames , _hitmapItemType , "" ) ;


}
#endif

int RootFileProcessor::getModuleID( TrackerHit* hitp ) {

  TrackerPulse* plsp = static_cast< TrackerPulse* >( hitp->getRawHits()[0] ) ;

  int mod = plsp->getCellID1() ;

  return mod;

}

int RootFileProcessor::getRowID( TrackerHit* hitp , const gear::TPCModule* tpcmodule ) {

  TrackerPulse* plsp = static_cast< TrackerPulse* >( hitp->getRawHits()[0] ) ;

  int padID = plsp->getCellID0() ;

  int rowID = tpcmodule->getRowNumber( padID ) ;

  return rowID ;

}
