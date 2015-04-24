#include "mem/ruby/structures/MulticastScoreboard.hh"
#include "mem/ruby/system/System.hh"
#include "debug/RubySlicc.hh"

using namespace std;
typedef MulticastScoreboardParams Params;

MulticastScoreboard::MulticastScoreboard(const Params *p) : SimObject(p) 
{
  GETS_count = 0;
  GETX_count = 0;
  total_nodes_predicted = 0;
  extra_nodes_predicted = 0;
}; 

void MulticastScoreboard::record_GETS(NetDest pred_set, NetDest sharers, bool owner_valid, MachineID owner) 
{
  GETS_count++; 
}

void MulticastScoreboard::record_GETS(NetDest pred_set) 
{
  GETS_count++; 
}

void MulticastScoreboard::record_GETX(NetDest pred_set, NetDest sharers, bool owner_valid, MachineID owner)
{
  GETX_count++;
}

void MulticastScoreboard::record_GETX(NetDest pred_set)
{
  GETX_count++;
}


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
