#!/bin/sh
# ============
#  manyrun.sh
# ============
#
# (Function)
#   Reconstruct multiple data files 
# (Usage)
#   manyrun <maxevt>
# (Upadate Record)
#   2011/12/30   K.Fujii         Original version.
#
#--
#  Check arguments.
#--
if [ $# -ge 2 ]; then
      echo "Usage:"
      echo "  ./manyrun <maxevt>"
      echo " where <maxevt> is the maximum number of events to analyaze"
      echo " per run with its default value being 10000."
      exit 0
#--
# Set maximum number of events to reconstruct per run
#--
elif [ $# -eq 1 ]; then
      echo "maxevt is set to $1"
      maxevt=$1
else
      maxevt=20000
fi
echo "Going to analyze $maxevt events per run"
#--
# Loop over runs
#--
#for run in 17331 17352; do
#for run in 17331; do
#for run in 17319; do
#for run in 17319 17367 17324 17364 17327 17359 17375 17356 17340 17352; do
#
#for run in 17319 17367 17324 17364 17327 17359 17331 17356 17340 17352; do
#for run in 18564 18563 18562 18561 18552 18551 18550 18549 18548 18547; do
#for run in 18609 18606 18605 18601 18600 18598 18597 18595 18594 18592 18591; do
for run in  19985 19984 19983 19982 19981 19980 19979 19978 19977 19976 19975 19974 19973 19972 20042 20041 20043 20044 20045 20046 20047 20048 20049 20050 20051 20052 20053 20054 20055 20056 20057; do
	echo ".............................."
	echo "Start analyzing run" $run "..."
	sed -e "s;RUNNO;$run;" -e "s;MAXEVT;$maxevt;" reconstruction_libo_TEMPLATE.xml > temp.xml
	#sed -e "s;RUNNO;$run;" -e "s;MAXEVT;$maxevt;" reconstruction_TrackFitterKalman_multimodule_TEMPLATE.xml > temp.xml
	Marlin temp.xml >& r$run.log &
	sleep 5
	#sed -e "s;RUNNO;$run;" makeRootFile_template.xml > temp.xml
	#Marlin temp.xml
done
#--
# That's it, folks!
#--
exit 0
