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
  void record_GETS(MachineID l1, NetDest pred_set, NetDest sharers, bool owner_valid, MachineID owner);

  // Registers statistics for a multicast GETS operation where local directory entry doesn't exist
  void record_GETS(MachineID l1, NetDest pred_set);

  // Registers statistics for a multicast GETX operation where local directory entry exists
  void record_GETX(MachineID l1, NetDest pred_set, NetDest sharers, bool owner_valid, MachineID owner);
  
  // Registers statistics for a multicast GETX operation where local directory entry doesn't exist
  void record_GETX(MachineID l1, NetDest pred_set);

  // Registers statistics for a multicast PUTX operation where local directory entry exists
  void record_PUTX(MachineID l1, NetDest pred_set, NetDest sharers, bool owner_valid, MachineID owner);
  
  // Registers statistics for a multicast PUTX operation where local directory entry doesn't exist
  void record_PUTX(MachineID l1, NetDest pred_set);
  

  // FOR STAT HANDLING
  void regStats();

  // MANDATORY SIM OBJECT METHODS
  MulticastScoreboard& operator=(const MulticastScoreboard& obj);

 private:
  // Tracking Stats
  Stats::Scalar GETS_count;
  Stats::Scalar GETX_count;
  Stats::Scalar PUTX_count;
  Stats::Scalar total_nodes_predicted;
  Stats::Scalar extra_nodes_predicted;
  Stats::Scalar perfect_nodes_predicted;
  Stats::Scalar accurate_predictions;
  Stats::Scalar l2_predictions;

  // Recorded Stats
  Stats::Formula avg_nodes_in_multicast;
  Stats::Formula avg_extra_nodes_predicted;
  Stats::Formula multicast_traffic_ratio;
  Stats::Formula prediction_accuracy;
  Stats::Formula blocks_found_at_l2;
};

#endif // __MEM_RUBY_STRUCTURE_MULTICASTSCOREBOARD_HH__
