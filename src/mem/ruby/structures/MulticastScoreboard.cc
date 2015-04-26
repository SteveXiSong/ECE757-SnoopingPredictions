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
  total_nodes_predicted = 0;
  extra_nodes_predicted = 0;
}; 

////////////////////////////////////////////////////////////////////////////////
void MulticastScoreboard::record_GETS(MachineID l1, NetDest pred_set, NetDest sharers, bool owner_valid, MachineID owner) 
{
  assert(l1.type == MachineType_L1Cache);

  // update basic counts
  GETS_count++; 
  total_nodes_predicted += pred_set.count();

  // only necessary L1 nodes are itself and owner
  for (NodeID i = 0; i < MachineType_base_count(MachineType_L1Cache); i++) {
    MachineID mach = {MachineType_L1Cache, i};
    if (mach != l1 && pred_set.isElement(mach) && (!owner_valid || mach != owner)) {
      extra_nodes_predicted++;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void MulticastScoreboard::record_GETS(MachineID l1, NetDest pred_set) 
{
  assert(l1.type == MachineType_L1Cache);

  // update basic counts
  GETS_count++;
  total_nodes_predicted += pred_set.count();

  // any L1s predicted in this case are extra since L2 owns this data
  for (NodeID i = 0; i < MachineType_base_count(MachineType_L1Cache); i++) {
    MachineID mach = {MachineType_L1Cache, i};
    if (mach != l1 && pred_set.isElement(mach)) {
      extra_nodes_predicted++;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void MulticastScoreboard::record_GETX(MachineID l1, NetDest pred_set, NetDest sharers, bool owner_valid, MachineID owner)
{
  assert(l1.type == MachineType_L1Cache);

  // update basic counts
  GETX_count++;
  total_nodes_predicted += pred_set.count();

  // necessary L1 are itslef, sharers and an owner if it exists
  for (NodeID i = 0; i < MachineType_base_count(MachineType_L1Cache); i++) {
    MachineID mach = {MachineType_L1Cache, i};
    if (mach != l1 && pred_set.isElement(mach) && !sharers.isElement(mach) && (!owner_valid || mach != owner)) {
      extra_nodes_predicted++;
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

  // any L1s predicted in this case are extra since L2 owns this data
  for (NodeID i = 0; i < MachineType_base_count(MachineType_L1Cache); i++) {
    MachineID mach = {MachineType_L1Cache, i};
    if (l1 != mach && pred_set.isElement(mach)) {
      extra_nodes_predicted++;
    }
  }
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

    total_nodes_predicted
        .name(name() + ".total_nodex_predicted")
        .desc("Total count of nodes predicted in multicasts")
        ;

    extra_nodes_predicted
        .name(name() + ".extra_nodex_predicted")
        .desc("Total count extra unecessary nodes predicted in multicasts")
        ;
}

MulticastScoreboard *
MulticastScoreboardParams::create()
{
    return new MulticastScoreboard(this);
}
