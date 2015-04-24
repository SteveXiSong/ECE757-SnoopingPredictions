from m5.params import *
from m5.SimObject import SimObject

class MulticastScoreboard(SimObject):
    type = 'MulticastScoreboard'
    cxx_class = 'MulticastScoreboard'
    cxx_header = "mem/ruby/structures/MulticastScoreboard.hh"
