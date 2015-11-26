===================================
Plain Fork of XIOS-1.0 Paris branch
===================================

Cloned from XIOS1-svn repository::

   git@gitlab.ecosystem-modelling.pml.ac.uk:momm/XIOS1-svn.git

For typical compilation on PML workstation with serial netcdf compile with::

   module load mpi #required on fedora
   ./make_xios --arch GCC_PMPC --prod --full --netcdf_lib netcdf4_seq

For compilation on *archer* with parallel netcdf::

   module unload PrgEnv-cray
   module unload PrgEnv-gnu
   module load PrgEnv-intel
   module load cray-netcdf-hdf5parallel
   ./make_xios --arch XC_ARCHER_INTEL --prod --full --netcdf_lib netcdf4_par

For compilation on serial nodes on *archer*::

   module swap craype-ivybridge craype-sandybridge
   module unload PrgEnv-cray
   module unload PrgEnv-gnu
   module load PrgEnv-intel
   module load cray-netcdf
   ./make_xios --arch XC_ARCHER_INTEL_PPNODE --prod --full --netcdf_lib netcdf4_seq

For use with *NEMO 3.6 stable* use the ``nemo3.6-fix`` branch::

   git checkout nemo3.6-fix
