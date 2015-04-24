#ifndef __MEM_RUBY_STRUCTURE_MULTICASTSCOREBOARD_HH__
#define __MEM_RUBY_STRUCTURE_MULTICASTSCOREBOARD_HH__

#include <iostream>
#include <string>
#include <stdlib.h>

#include "mem/ruby/common/NetDest.hh"
#include "mem/ruby/common/Address.hh"
#include "mem/protocol/MachineType.hh"
#include "params/MulticastScoreboard.hh"
#include "sim/sim_object.hh"

class MulticastScoreboard : public SimObject {
 public:
  MulticastScoreboard(const Params *p) : SimObject(p) {}; 

  // MANDATORY SIM OBJECT METHODS
  MulticastScoreboard& operator=(const MulticastScoreboard& obj);
};

#endif // __MEM_RUBY_STRUCTURE_MULTICASTSCOREBOARD_HH__))))
