from m5.params import *
from m5.SimObject import SimObject

class RubySnoopBasicPred(SimObject):
    type = 'RubySnoopBasicPred'
    cxx_class = 'SnoopBasicPred'
    cxx_header = "mem/ruby/structures/SnoopBasicPred.hh"

    numPredTableEntry = Param.Int(1024, "Number of predicton entries")
