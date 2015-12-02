#!/bin/bash

export XIOS_HOME=$HOME/git/XIOS1


module unload PrgEnv-cray
module unload PrgEnv-gnu
module load PrgEnv-intel
module load cray-netcdf-hdf5parallel

cd $XIOS_HOME && ./make_xios --arch XC_ARCHER_INTEL --prod --full --netcdf_lib netcdf4_par && rsync -a $XIOS_HOME/bin $XIOS_HOME/inc $XIOS_HOME/lib ~/local/xios-intel
