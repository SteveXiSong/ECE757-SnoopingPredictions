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
  Stats::Scalar accurate_GETS_predictions;
  Stats::Scalar accurate_GETX_predictions;
  Stats::Scalar accurate_PUTX_predictions;
  Stats::Scalar l2_GETS_services;
  Stats::Scalar l2_GETX_services;
  Stats::Scalar l2_PUTX_services;
  Stats::Scalar GETS_false_positive;
  Stats::Scalar GETS_true_positive;
  Stats::Scalar GETS_false_negative;
  Stats::Scalar GETS_true_negative;
  Stats::Scalar GETX_false_positive;
  Stats::Scalar GETX_true_positive;
  Stats::Scalar GETX_false_negative;
  Stats::Scalar GETX_true_negative;
  Stats::Scalar PUTX_false_positive;
  Stats::Scalar PUTX_true_positive;
  Stats::Scalar PUTX_false_negative;
  Stats::Scalar PUTX_true_negative;

  // Recorded Stats
  Stats::Formula avg_nodes_in_multicast;
  Stats::Formula avg_extra_nodes_predicted;
  Stats::Formula multicast_traffic_ratio;
  Stats::Formula overall_prediction_accuracy;
  Stats::Formula GETS_prediction_accuracy;
  Stats::Formula GETX_prediction_accuracy;
  Stats::Formula PUTX_prediction_accuracy;
  Stats::Formula overall_l2_accuracy;
  Stats::Formula GETS_l2_accuracy;
  Stats::Formula GETX_l2_accuracy;
  Stats::Formula PUTX_l2_accuracy;
  Stats::Formula overall_prevalence;
  Stats::Formula overall_sensitivity;
  Stats::Formula overall_pvp;
  Stats::Formula GETS_prevalence;
  Stats::Formula GETS_sensitivity;
  Stats::Formula GETS_pvp;
  Stats::Formula GETX_prevalence;
  Stats::Formula GETX_sensitivity;
  Stats::Formula GETX_pvp;
  Stats::Formula PUTX_prevalence;
  Stats::Formula PUTX_sensitivity;
  Stats::Formula PUTX_pvp;
};

#endif // __MEM_RUBY_STRUCTURE_MULTICASTSCOREBOARD_HH__
