#include "mem/ruby/structures/SnoopBasicPred.hh"
#include "mem/ruby/system/System.hh"
#include "debug/RubySnoopPred.hh"
#include "debug/RubySlicc.hh"

using namespace std;

NetDest SnoopBasicPred::getPrediction(Address addr, MachineID local) {
  NetDest prediction;

  // add L1 nodes to mask
  for (NodeID i = 0; i < MachineType_base_count(MachineType_L1Cache); i++) {
    MachineID mach = {MachineType_L1Cache, i};
    if (mach == local || rand() % 2) {
      prediction.add(mach);
    }
  }

  // add L2 node to mask
  for (NodeID i = 0; i < MachineType_base_count(MachineType_L2Cache); i++) {
    MachineID mach = {MachineType_L2Cache, i};
    prediction.add(mach);
  }

  //fprintf(stdout, "[SnoopBasicPred] getPrediction is called.\n");
  DPRINTF(RubySnoopPred, "[SnoopBasicPred] L1 Prediction: %s\n", prediction);
  DPRINTF(RubySlicc, "L1 Prediction: %s\n", prediction);
  return prediction;
}

SnoopBasicPred *
RubySnoopBasicPredParams::create()
{
    return new SnoopBasicPred(this);
}

