from m5.params import *
from m5.SimObject import SimObject
from SnoopBasicPred import RubySnoopBasicPred

class RubyStickyPred(RubySnoopBasicPred):
    type = 'RubyStickyPred'
    cxx_class = 'StickyPred'
    cxx_header = "mem/ruby/structures/StickyPred.hh"

    numPredTableEntry = Param.Int(1024, "Number of predicton entries")
    numStickyEntry = Param.Int(10, "Number of stikcy entryies")
