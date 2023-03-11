#!/usr/bin/env python
#-----------------------------------------------------------------------------
import os, sys, re
import numpy as np
from array import array
from ROOT import gSystem
#-----------------------------------------------------------------------------
def main():
    print("\tTurtle")

    gSystem.AddDynamicPath('%s/../lib' % os.environ['PWD'])
    gSystem.Load("libturtlebinning")
    from ROOT import Turtle
    
    nbins   = 100
    nevents = 1000
    nparams = 2
    data    = array('d', np.random.uniform(0, 1, nparams*nevents))
    turtle  = Turtle(data, nbins, nevents, nparams)

    for ii in range(nbins):
        indices = turtle.indicesInBin(ii)
        print(f'bin[{ii:d}]: ', end='')
        ll = 0
        for jj in indices:
            print(f'{jj:4d}', end='')
            ll += 1
            if ll >= 10:
                print()
        if ll < 10:
            print()
#----------------------------------------------------------------------------
main()
