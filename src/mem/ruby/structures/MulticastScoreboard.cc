#include "mem/ruby/structures/MulticastScoreboard.hh"
#include "mem/ruby/system/System.hh"
#include "debug/RubySlicc.hh"

using namespace std;
typedef MulticastScoreboardParams Params;

////////////////////////////////////////////////////////////////////////////////
MulticastScoreboard::MulticastScoreboard(const Params *p) : SimObject(p) 
{
  GETS_count = 0;
  GETX_count = 0;
  PUTX_count = 0;
  total_nodes_predicted = 0;
  extra_nodes_predicted = 0;
  perfect_nodes_predicted = 0;
  accurate_predictions = 0;
  l2_predictions = 0;
}; 

////////////////////////////////////////////////////////////////////////////////
void MulticastScoreboard::record_GETS(MachineID l1, NetDest pred_set, NetDest sharers, bool owner_valid, MachineID owner) 
{
  assert(l1.type == MachineType_L1Cache);

  // update basic counts
  GETS_count++; 
  total_nodes_predicted += pred_set.count();
  perfect_nodes_predicted += (owner_valid && owner.type == MachineType_L1Cache ? 3 : 2);

  // only necessary L1 nodes are itself and owner
  for (NodeID i = 0; i < MachineType_base_count(MachineType_L1Cache); i++) {
    MachineID mach = {MachineType_L1Cache, i};
    if (mach != l1 && pred_set.isElement(mach) && (!owner_valid || mach != owner)) {
      extra_nodes_predicted++;
    }
  }

  // update prediction accuracy data
  if(!owner_valid || owner.type == MachineType_L2Cache) {
    accurate_predictions++;
    l2_predictions++;
  } else if(pred_set.isElement(owner)) {
    accurate_predictions++;
  }
}

////////////////////////////////////////////////////////////////////////////////
void MulticastScoreboard::record_GETS(MachineID l1, NetDest pred_set) 
{
  assert(l1.type == MachineType_L1Cache);

  // update basic counts
  GETS_count++;
  total_nodes_predicted += pred_set.count();
  perfect_nodes_predicted += 2;

  // any L1s predicted in this case are extra since L2 owns this data
  for (NodeID i = 0; i < MachineType_base_count(MachineType_L1Cache); i++) {
    MachineID mach = {MachineType_L1Cache, i};
    if (mach != l1 && pred_set.isElement(mach)) {
      extra_nodes_predicted++;
    }
  }

  // update prediction accuracy data
  accurate_predictions++;
  l2_predictions++;
}

////////////////////////////////////////////////////////////////////////////////
void MulticastScoreboard::record_GETX(MachineID l1, NetDest pred_set, NetDest sharers, bool owner_valid, MachineID owner)
{
  assert(l1.type == MachineType_L1Cache);

  // update basic counts
  GETX_count++;
  total_nodes_predicted += pred_set.count();
  perfect_nodes_predicted += (owner_valid && owner.type == MachineType_L1Cache ? 3 : 2) + sharers.count();

  // necessary L1 are itslef, sharers and an owner if it exists
  for (NodeID i = 0; i < MachineType_base_count(MachineType_L1Cache); i++) {
    MachineID mach = {MachineType_L1Cache, i};
    if (mach != l1 && pred_set.isElement(mach) && !sharers.isElement(mach) && (!owner_valid || mach != owner)) {
      extra_nodes_predicted++;
    }
  }

  // update accuracy counts
  if(sharers.isSubset(pred_set)) {
    if(!owner_valid || owner.type == MachineType_L2Cache) {
      accurate_predictions++;
      l2_predictions++;
    } else if(pred_set.isElement(owner)) {
      accurate_predictions++;
    }
  }

}

////////////////////////////////////////////////////////////////////////////////
void MulticastScoreboard::record_GETX(MachineID l1, NetDest pred_set)
{
  assert(l1.type == MachineType_L1Cache);

  // update basic counts
  GETX_count++;
  total_nodes_predicted += pred_set.count();
  perfect_nodes_predicted += 2;

  // any L1s predicted in this case are extra since L2 owns this data
  for (NodeID i = 0; i < MachineType_base_count(MachineType_L1Cache); i++) {
    MachineID mach = {MachineType_L1Cache, i};
    if (l1 != mach && pred_set.isElement(mach)) {
      extra_nodes_predicted++;
    }
  }

  // update accuracy counts
  accurate_predictions++;
  l2_predictions++; 
}

////////////////////////////////////////////////////////////////////////////////
void MulticastScoreboard::record_PUTX(MachineID l1, NetDest pred_set, NetDest sharers, bool owner_valid, MachineID owner)
{
  assert(l1.type == MachineType_L1Cache);

  // update basic counts
  PUTX_count++;
  total_nodes_predicted += pred_set.count();
  perfect_nodes_predicted += (owner_valid && owner.type == MachineType_L1Cache ? 3 : 2) + sharers.count();

  for(NodeID i = 0; i < MachineType_base_count(MachineType_L1Cache); i++) {
    MachineID mach = {MachineType_L1Cache, i};
    if (l1 != mach && pred_set.isElement(mach) && !sharers.isElement(mach)) {
      extra_nodes_predicted++;
    }
  }

  // update accuracy counts
  if(sharers.isSubset(pred_set)) {
    if(!owner_valid || owner.type == MachineType_L2Cache) {
      accurate_predictions++;
      l2_predictions++;
    } else if(pred_set.isElement(owner)) {
      accurate_predictions++;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////  
void MulticastScoreboard::record_PUTX(MachineID l1, NetDest pred_set)
{
  assert(l1.type == MachineType_L1Cache);

  // update basic counts
  PUTX_count++;
  total_nodes_predicted += pred_set.count(); 
  perfect_nodes_predicted += 2;

  // any L1s predicted in this case are extra since L2 owns this data
  for(NodeID i = 0; i < MachineType_base_count(MachineType_L1Cache); i++) {
    MachineID mach = {MachineType_L1Cache, i};
    if (l1 != mach && pred_set.isElement(mach)) {
      extra_nodes_predicted++;
    }
  }

  // update accuracy counts
  accurate_predictions++;
  l2_predictions++; 
}

////////////////////////////////////////////////////////////////////////////////
void MulticastScoreboard::regStats()
{
    GETS_count
        .name(name() + ".GETS_count")
        .desc("Number of multicast GETS's received")
        ;

    GETX_count
        .name(name() + ".GETX_count")
        .desc("Number of multicast GETX's received")
        ;

    PUTX_count
        .name(name() + ".PUTX_count")
        .desc("Number of multicast PUTX's received")
        ;

    total_nodes_predicted
        .name(name() + ".total_nodes_predicted")
        .desc("Total count of nodes predicted in multicasts")
        ;

    extra_nodes_predicted
        .name(name() + ".extra_nodes_predicted")
        .desc("Total count extra unecessary nodes predicted in multicasts")
        ;
    
    perfect_nodes_predicted
        .name(name() + ".perfect_nodes_predicted")
        .desc("Total count of nodes predicted with perfect multicasts")
        ;

    accurate_predictions
        .name(name() + ".accurate_predictions")
        .desc("Total count of accurately predicted multicasts")
        ;

    l2_predictions
        .name(name() + ".l2_predictions")
        .desc("Total count of accurately predicted multicast servied by l2")
        ;

    // Recorded Stats
    avg_nodes_in_multicast
        .name(name() + ".avg_nodes_in_multicast")
        .desc("Avg. number of nodes included in multicasts")
        ;
    avg_nodes_in_multicast = (total_nodes_predicted / (PUTX_count + GETX_count + GETS_count));

    avg_extra_nodes_predicted
        .name(name() + ".avg_extra_nodes_predicted")
        .desc("Avg. number of extra (uncessary) nodes included in multicasts")
        ;
    avg_extra_nodes_predicted = (extra_nodes_predicted / (PUTX_count + GETX_count + GETS_count));
  
    multicast_traffic_ratio
        .name(name() + ".multicast_traffic_ratio")
        .desc("Ratio of total nodes predicted to nodes predicted in multicast to nodes in perfect multicast")
        ;
    multicast_traffic_ratio = total_nodes_predicted / perfect_nodes_predicted;

    prediction_accuracy
        .name(name() + ".prediction_accuracy")
        .desc("Percent accuracy of predictions made by L1's in system")
        ;
    prediction_accuracy = accurate_predictions / (PUTX_count + GETX_count + GETS_count);

    blocks_found_at_l2
        .name(name() + ".block_found_at_l2")
        .desc("Percent of blocks that were serviced by l2")
        ;
    blocks_found_at_l2 = l2_predictions / (PUTX_count + GETX_count + GETS_count);
}

MulticastScoreboard *
MulticastScoreboardParams::create()
{
    return new MulticastScoreboard(this);
}
