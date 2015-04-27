from m5.params import *
from m5.SimObject import SimObject
from SnoopBasicPred import RubySnoopBasicPred

class RubyStickyPred(SimObject):
    type = 'RubyStickyPred'
    cxx_class = 'StickyPred'
    cxx_header = "mem/ruby/structures/StickyPred.hh"
