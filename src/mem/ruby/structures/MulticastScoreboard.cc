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
  accurate_GETS_predictions = 0;
  accurate_GETX_predictions = 0;
  accurate_PUTX_predictions = 0;
  l2_GETS_services = 0;
  l2_GETX_services = 0;
  l2_PUTX_services = 0;
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
    accurate_GETS_predictions++;
    l2_GETS_services++;
  } else if(pred_set.isElement(owner)) {
    accurate_GETS_predictions++;
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
  accurate_GETS_predictions++;
  l2_GETS_services++;
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
      accurate_GETX_predictions++;
      l2_GETX_services++;
    } else if(pred_set.isElement(owner)) {
      accurate_GETX_predictions++;
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
  accurate_GETX_predictions++;
  l2_GETX_services++; 
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
      accurate_PUTX_predictions++;
      l2_PUTX_services++;
    } else if(pred_set.isElement(owner)) {
      accurate_PUTX_predictions++;
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
  accurate_PUTX_predictions++;
  l2_PUTX_services++; 
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

    accurate_GETS_predictions
        .name(name() + ".accurate_GETS_predictions")
        .desc("Total count of accurate GETS predictions")
        ;

    accurate_GETX_predictions
        .name(name() + ".accurate_GETX_predictions")
        .desc("Total count of accurate GETX predictions")
        ;

    accurate_PUTX_predictions
        .name(name() + ".accurate_PUTX_predictions")
        .desc("Total count of accurate PUTX predictions")
        ;
    
    l2_GETS_services
        .name(name() + ".l2_GETS_services")
        .desc("Total count of GETS operations serviced by l2")
        ;

    l2_GETX_services
        .name(name() + ".l2_GETX_services")
        .desc("Total count of GETX operations serviced by l2")
        ;

    l2_PUTX_services
        .name(name() + ".l2_PUTX_services")
        .desc("Total count of PUTX operations serviced by l2")
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

    overall_prediction_accuracy
        .name(name() + ".overall_prediction_accuracy")
        .desc("Percent accuracy of all predictions made by L1's in system")
        ;
    overall_prediction_accuracy = (accurate_GETS_predictions + accurate_GETX_predictions + accurate_PUTX_predictions) / (PUTX_count + GETX_count + GETS_count);

    GETS_prediction_accuracy
        .name(name() + ".GETS_prediction_accuracy")
        .desc("Percent accuracy of all GETS predictions made by L1's in system")
        ;
    GETS_prediction_accuracy = accurate_GETS_predictions / GETS_count;

    GETX_prediction_accuracy
        .name(name() + ".GETX_prediction_accuracy")
        .desc("Percent accuracy of all GETX predictions made by L1's in system")
        ;
    GETX_prediction_accuracy = accurate_GETX_predictions / GETX_count;

    PUTX_prediction_accuracy
        .name(name() + ".PUTX_prediction_accuracy")
        .desc("Percent accuracy of all PUTX predictions made by L1's in system")
        ;
    PUTX_prediction_accuracy = accurate_PUTX_predictions / PUTX_count;

    overall_l2_accuracy
        .name(name() + ".overall_l2_accuracy")
        .desc("Percent of all predictions serviced by L2")
        ;
    overall_l2_accuracy = (l2_GETS_services + l2_GETX_services + l2_PUTX_services) / (PUTX_count + GETX_count + GETS_count);

    GETS_l2_accuracy
        .name(name() + ".GETS_l2_accuracy")
        .desc("Percent of all GETS predictions serviced by L2")
        ;
    GETS_l2_accuracy = l2_GETS_services / GETS_count;

    GETX_l2_accuracy
        .name(name() + ".GETX_l2_accuracy")
        .desc("Percent of all GETX predictions serviced by L2")
        ;
    GETX_l2_accuracy = l2_GETX_services / GETX_count;

    PUTX_l2_accuracy
        .name(name() + ".PUTX_l2_accuracy")
        .desc("Percent of all PUTX predictions serviced by L2")
        ;
    PUTX_l2_accuracy = l2_PUTX_services / PUTX_count;
}

MulticastScoreboard *
MulticastScoreboardParams::create()
{
    return new MulticastScoreboard(this);
}
