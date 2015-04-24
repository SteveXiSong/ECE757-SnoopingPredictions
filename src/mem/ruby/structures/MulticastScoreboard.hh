#ifndef __MEM_RUBY_STRUCTURE_MULTICASTSCOREBOARD_HH__
#define __MEM_RUBY_STRUCTURE_MULTICASTSCOREBOARD_HH__

#include <iostream>
#include <string>
#include <stdlib.h>

#include "base/statistics.hh"
#include "mem/ruby/common/NetDest.hh"
#include "mem/ruby/common/Address.hh"
#include "mem/protocol/MachineType.hh"
#include "params/MulticastScoreboard.hh"
#include "sim/sim_object.hh"

class MulticastScoreboard : public SimObject {
 public:
  MulticastScoreboard(const Params *p); 

  // Registers statistics for a multicast GETS operation where local directory entry exists
  void record_GETS(NetDest pred_set, NetDest sharers, bool owner_valid, MachineID owner);

  // Registers statistics for a multicast GETS operation where local directory entry doesn't exist
  void record_GETS(NetDest pred_set);

  // Registers statistics for a multicast GETX operation where local directory entry exists
  void record_GETX(NetDest pred_set, NetDest sharers, bool owner_valid, MachineID owner);
  
  // Registers statistics for a multicast GETX operation where local directory entry doesn't exist
  void record_GETX(NetDest pred_set);

  // FOR STAT HANDLING
  void regStats();

  // MANDATORY SIM OBJECT METHODS
  MulticastScoreboard& operator=(const MulticastScoreboard& obj);

 private:
  Stats::Scalar GETS_count;
  Stats::Scalar GETX_count;
  Stats::Scalar total_nodes_predicted;
  Stats::Scalar extra_nodes_predicted;
};

#endif // __MEM_RUBY_STRUCTURE_MULTICASTSCOREBOARD_HH__))))
