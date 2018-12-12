#!/bin/sh
# ============
#  manyrun.sh
# ============
#
# (Function)
#   Unpack multiple data files 
# (Usage)
#   manyrun_slcio
# (Upadate Record)
#   2016/12/30   K.Fujii         Original version.
#
#--
#  Check arguments.
#--
if [ $# -ge 1 ]; then
      echo "Usage:"
      echo "  ./manyrun_slcio"
      exit 0
fi
#--
# Loop over runs
#--
#for run in 19987 19985 19984 19983 19982 19981 19980 19979 19978 19977 19976 19975 19974 19973 19972; do # w/ gate
for run in 20042 20041 20043 20044 20045 20046 20047 20048 20049 20050 20051 20052 20053 20054 20055; do # w/ fs
	echo ".............................."
	echo "Start unpacking run" $run "..."
	AdvancedAltroConverter -M TPCChannelMapping_20161109_7GEM.slcio -I /proj/cdc/data/altro/desy/readout-${run}_0.dat >& u${run}.log &
done
#--
# That's it, folks!
#--
exit 0
