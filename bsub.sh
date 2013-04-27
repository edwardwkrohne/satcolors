#!/bin/bash
#BSUB -J borelcolors # job name
#BSUB -e borelcolors.%J.err
#BSUB -o borelcolors.%J.out

# Keep track of svn info
echo SVN Header:
echo '$Header: svn://pureinsomnia.com/edeve/trunk/borelcolors/bsub.sh 170 2012-11-18 19:42:09Z ed $'
echo $PWD

# Run the application
#bin/solve.exe