#!/usr/bin/env python
#-----------------------------------------------------------------------------
import os, sys, re
from array import array
from ROOT import *
#-----------------------------------------------------------------------------
def main():
    print("\tTurtle")

    # A workaround ROOT 6.08.04
    gSystem.AddDynamicPath('%s/../lib' % os.environ['PWD'])

    gSystem.Load("libturtle")

    turtle = Turtle()
#----------------------------------------------------------------------------
main()
