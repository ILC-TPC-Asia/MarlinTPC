export ILCSOFT=/proj/soft/ilcsoft/v02-00

# -------------------------------------------------------------------- ---

# ---  Use the same compiler and python as used for the installation   ---

# -------------------------------------------------------------------- ---
export PATH=/opt/osxws/bin:/usr/bin:${PATH}
export LD_LIBRARY_PATH=/opt/osxws/lib64:/opt/osxws/lib:/usr/lib:${LD_LIBRARY_PATH}

export CXX=g++
export CC=gcc


#--------------------------------------------------------------------------------
#     LCCD
#--------------------------------------------------------------------------------
export LCCD="/proj/soft/ilcsoft/v02-00/lccd/v01-05"
export LD_LIBRARY_PATH="$LCCD/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     CondDBMySQL
#--------------------------------------------------------------------------------
export COND_DB_DEBUGLOG="/dev/stdout"
export CondDBMySQL="/proj/soft/ilcsoft/v02-00/CondDBMySQL/CondDBMySQL_ILC-0-9-6"
export LD_LIBRARY_PATH="$CondDBMySQL/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     LCIO
#--------------------------------------------------------------------------------
export LCIO="/proj/soft/ilcsoft/v02-00/lcio/v02-12-0"
export PYTHONPATH="$LCIO/python:$LCIO/python/examples:$PYTHONPATH"
export PATH="$LCIO/tools:$LCIO/bin:$PATH"
export LD_LIBRARY_PATH="$LCIO/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     ROOT
#--------------------------------------------------------------------------------
export ROOTSYS="/opt/root/6.13.02"
#export ROOTSYS="/opt/root/5.54.36"
export PYTHONPATH="$ROOTSYS/lib:$PYTHONPATH"
export PATH="$ROOTSYS/bin:$PATH"
export LD_LIBRARY_PATH="$ROOTSYS/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     CMake
#--------------------------------------------------------------------------------
export PATH="/opt/osxws/bin:$PATH"


#--------------------------------------------------------------------------------
#     ILCUTIL
#--------------------------------------------------------------------------------
export ilcutil="/proj/soft/ilcsoft/v02-00/ilcutil/v01-05"
export LD_LIBRARY_PATH="$ilcutil/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     Marlin
#--------------------------------------------------------------------------------
export MARLIN="/proj/soft/ilcsoft/v02-00/Marlin/v01-16"
export PATH="$MARLIN/bin:$PATH"
export LD_LIBRARY_PATH="$MARLIN/lib:$LD_LIBRARY_PATH"
export MARLIN_DLL="/proj/soft/ilcsoft/v02-00/MarlinDD4hep/v00-06/lib/libMarlinDD4hep.dylib:/proj/soft/ilcsoft/v02-00/DDMarlinPandora/v00-10/lib/libDDMarlinPandora.dylib:/proj/soft/ilcsoft/v02-00/MarlinReco/v01-24-01/lib/libMarlinReco.dylib:/proj/soft/ilcsoft/v02-00/PandoraAnalysis/v02-00-00/lib/libPandoraAnalysis.dylib:/proj/soft/ilcsoft/v02-00/LCFIVertex/v00-07-04/lib/libLCFIVertexProcessors.dylib:/proj/soft/ilcsoft/v02-00/CEDViewer/v01-15/lib/libCEDViewer.dylib:/proj/soft/ilcsoft/v02-00/Overlay/v00-21/lib/libOverlay.dylib:/proj/soft/ilcsoft/v02-00/MarlinFastJet/v00-05-01/lib/libMarlinFastJet.dylib:/proj/soft/ilcsoft/v02-00/LCTuple/v01-11/lib/libLCTuple.dylib:/proj/soft/ilcsoft/v02-00/MarlinKinfit/v00-06/lib/libMarlinKinfit.dylib:/proj/soft/ilcsoft/v02-00/MarlinTrkProcessors/v02-10/lib/libMarlinTrkProcessors.dylib:/proj/soft/ilcsoft/v02-00/MarlinKinfitProcessors/v00-04/lib/libMarlinKinfitProcessors.dylib:/proj/soft/ilcsoft/v02-00/ILDPerformance/v01-06/lib/libILDPerformance.dylib:/proj/soft/ilcsoft/v02-00/Clupatra/v01-03/lib/libClupatra.dylib:/proj/soft/ilcsoft/v02-00/Physsim/v00-04-01/lib/libPhyssim.dylib:/proj/soft/ilcsoft/v02-00/LCFIPlus/v00-06-08/lib/libLCFIPlus.dylib:/proj/soft/ilcsoft/v02-00/FCalClusterer/v01-00/lib/libFCalClusterer.dylib:/proj/soft/ilcsoft/v02-00/ForwardTracking/v01-13/lib/libForwardTracking.dylib:/proj/soft/ilcsoft/v02-00/ConformalTracking/v01-07/lib/libConformalTracking.dylib:/proj/soft/ilcsoft/v02-00/LICH/v00-01/lib/libLICH.dylib:/proj/soft/ilcsoft/v02-00/MarlinTPC/v01-04/lib/libMarlinTPC.dylib:/proj/soft/ilcsoft/v02-00/Garlic/v03-01/lib/libGarlic.dylib:$MARLIN_DLL"


#--------------------------------------------------------------------------------
#     CLHEP
#--------------------------------------------------------------------------------
export CLHEP="/opt/clhep/2.4.0.4"
export CLHEP_BASE_DIR="$CLHEP"
export CLHEP_INCLUDE_DIR="$CLHEP/include"
export PATH="$CLHEP_BASE_DIR/bin:$PATH"
export LD_LIBRARY_PATH="$CLHEP_BASE_DIR/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     RAIDA
#--------------------------------------------------------------------------------
export RAIDA_HOME="/proj/soft/ilcsoft/v02-00/RAIDA/v01-09"
export PATH="$RAIDA_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$RAIDA_HOME/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     GEAR
#--------------------------------------------------------------------------------
export GEAR="/proj/soft/ilcsoft/v02-00/gear/v01-08"
export PATH="$GEAR/tools:$GEAR/bin:$PATH"
export LD_LIBRARY_PATH="$GEAR/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     DD4hep
#--------------------------------------------------------------------------------
export DD4hep_ROOT="/proj/soft/ilcsoft/v02-00/DD4hep/v01-07"
export DD4hepINSTALL="/proj/soft/ilcsoft/v02-00/DD4hep/v01-07"
export DD4HEP="/proj/soft/ilcsoft/v02-00/DD4hep/v01-07"
export DD4hep_DIR="/proj/soft/ilcsoft/v02-00/DD4hep/v01-07"
export PYTHONPATH="$DD4HEP/python:$DD4HEP/DDCore/python:$PYTHONPATH"
export PATH="$DD4HEP/bin:$PATH"
export LD_LIBRARY_PATH="$DD4HEP/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     Geant4
#--------------------------------------------------------------------------------
export G4INSTALL="/opt/g4/10.04.p01"
export G4ENV_INIT="$G4INSTALL/bin/geant4.sh"
export G4SYSTEM="Darwin-g++"


#--------------------------------------------------------------------------------
#     XercesC
#--------------------------------------------------------------------------------
export XercesC_HOME="/proj/soft/ilcsoft/v02-00/xercesc/3.2.1"
export PATH="$XercesC_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$XercesC_HOME/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     Boost
#--------------------------------------------------------------------------------
export BOOST_ROOT="/opt/osxws"

#--------------------------------------------------------------------------------
#     MarlinTrk
#--------------------------------------------------------------------------------
export MARLINTRK="/proj/soft/ilcsoft/v02-00/MarlinTrk/v02-07"
export LD_LIBRARY_PATH="$MARLINTRK/lib:$LD_LIBRARY_PATH"
#--------------------------------------------------------------------------------
#     KalDet
#--------------------------------------------------------------------------------
export KALDET="/proj/soft/ilcsoft/v02-00/KalDet/v01-14-01"
export LD_LIBRARY_PATH="$KALDET/lib:$LD_LIBRARY_PATH"

#--------------------------------------------------------------------------------
#     KalTest
#--------------------------------------------------------------------------------
export KALTEST="/proj/soft/ilcsoft/v02-00/KalTest/v02-04"
export LD_LIBRARY_PATH="$KALTEST/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     aidaTT
#--------------------------------------------------------------------------------
export AIDATT="/proj/soft/ilcsoft/v02-00/aidaTT/v00-09"
export PATH="$AIDATT/bin:$PATH"
export LD_LIBRARY_PATH="$AIDATT/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     KiTrack
#--------------------------------------------------------------------------------
export KITRACK="/proj/soft/ilcsoft/v02-00/KiTrack/v01-09"
export LD_LIBRARY_PATH="$KITRACK/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     KiTrackMarlin
#--------------------------------------------------------------------------------
export KITRACKMARLIN="/proj/soft/ilcsoft/v02-00/KiTrackMarlin/v01-12"
export LD_LIBRARY_PATH="$KITRACKMARLIN/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     GSL
#--------------------------------------------------------------------------------
export GSL_HOME="/opt/osxws"
export PATH="$GSL_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$GSL_HOME/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     GBL
#--------------------------------------------------------------------------------
export GBL="/proj/soft/ilcsoft/v02-00/GBL/V02-01-01"
export LD_LIBRARY_PATH="$GBL/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     MarlinUtil
#--------------------------------------------------------------------------------
export LD_LIBRARY_PATH="/proj/soft/ilcsoft/v02-00/MarlinUtil/v01-15/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     CED
#--------------------------------------------------------------------------------
export PATH="/proj/soft/ilcsoft/v02-00/CED/v01-09-02/bin:$PATH"
export LD_LIBRARY_PATH="/proj/soft/ilcsoft/v02-00/CED/v01-09-02/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     PandoraPFANew
#--------------------------------------------------------------------------------
export PANDORAPFANEW="/proj/soft/ilcsoft/v02-00/PandoraPFANew/v03-09-00"
export LD_LIBRARY_PATH="$PANDORAPFANEW/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     PandoraAnalysis
#--------------------------------------------------------------------------------
export PANDORA_ANALYSIS_DIR="/proj/soft/ilcsoft/v02-00/PandoraAnalysis/v02-00-00"
export PATH="$PANDORA_ANALYSIS_DIR/bin:$PATH"
export LD_LIBRARY_PATH="$PANDORA_ANALYSIS_DIR/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     LCFIVertex
#--------------------------------------------------------------------------------
export LD_LIBRARY_PATH="/proj/soft/ilcsoft/v02-00/LCFIVertex/v00-07-04/lib:$LD_LIBRARY_PATH"

#--------------------------------------------------------------------------------
#     MarlinKinFit
#--------------------------------------------------------------------------------
export LD_LIBRARY_PATH="/proj/soft/ilcsoft/v02-00/MarlinKinFit/v00-06/lib:$LD_LIBRARY_PATH"

#--------------------------------------------------------------------------------
#     CEDViewer
#--------------------------------------------------------------------------------
export PATH="/proj/soft/ilcsoft/v02-00/CEDViewer/v01-15/bin:$PATH"
export LD_LIBRARY_PATH="/proj/soft/ilcsoft/v02-00/CEDViewer/v01-15/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     FastJet
#--------------------------------------------------------------------------------
export FastJet_HOME="/opt/fastjet/3.2.1"
export PATH="$FastJet_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$FastJet_HOME/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     MarlinTPC
#--------------------------------------------------------------------------------
export MARLINTPC="/proj/soft/ilcsoft/v02-00/MarlinTPC/v01-04"
export PATH="$MARLINTPC/bin:$PATH"
export LD_LIBRARY_PATH="$MARLINTPC/lib:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$MARLINTPC/tpcconddata/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     lcgeo
#--------------------------------------------------------------------------------
export lcgeo_DIR="/proj/soft/ilcsoft/v02-00/lcgeo/v00-16"
export PYTHONPATH="$lcgeo_DIR/lib/python:$PYTHONPATH"
export PATH="$lcgeo_DIR/bin:$PATH"
export LD_LIBRARY_PATH="$lcgeo_DIR/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     DD4hepExamples
#--------------------------------------------------------------------------------
export DD4hepExamples="/proj/soft/ilcsoft/v02-00/DD4hepExamples/v01-07"
export PATH="$DD4hepExamples/bin:$PATH"
export LD_LIBRARY_PATH="$DD4hepExamples/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     MySQL
#--------------------------------------------------------------------------------
export MYSQL_HOME="/proj/soft/ilcsoft/v02-00/mysql/osxws"
export MYSQL_LIBDIR="$MYSQL_HOME/lib/mysql"
export MYSQL_PATH="$MYSQL_HOME"
export MYSQL="$MYSQL_HOME"
export PATH="$MYSQL_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$MYSQL_HOME/lib64:$MYSQL_HOME/lib:$MYSQL_HOME/lib64/mysql:$MYSQL_HOME/lib/mysql:$LD_LIBRARY_PATH"

# --- source GEANT4 INIT script ---
test -r ${G4ENV_INIT} && { cd $(dirname ${G4ENV_INIT}) ; . ./$(basename ${G4ENV_INIT}) ; cd $OLDPWD ; }

# --- set DYLD_LIBRARY_PATH to LD_LIBRARY_PATH for MAC compatibility ---
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH

