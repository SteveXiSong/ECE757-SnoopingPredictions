#include "mem/ruby/structures/SnoopBasicPred.hh"
#include "mem/ruby/system/System.hh"
#include "debug/RubySnoopPred.hh"
#include "debug/RubySlicc.hh"

using namespace std;

typedef RubySnoopBasicPredParams Params;
SnoopBasicPred::SnoopBasicPred(const Params *p)
    :SimObject(p)
{
    profileData.MC_req_fromL1 = 0;
}

SnoopBasicPred::~SnoopBasicPred(){
    //DPRINTF(SnoopBasicPred, "SnoopBasicPred destructor is called\n");
}

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
  return prediction;
}

void SnoopBasicPred::profileRequestMsg(int reqNum){
    DPRINTF(RubySnoopPred, "profileRequestMsg is called.\n");
    profileData.tot_req_fromL1++;
    switch(reqNum){
        case GETX:
            profileData.GetS++;
            break;
        case GETS:
            profileData.GetX++;
            break;
        case MC_GETS:
            profileData.MC_GetS++;
            break;
        default:
            break;
    }
    return;
}

int SnoopBasicPred::getGETS(){
    return GETS;
}

SnoopBasicPred *
RubySnoopBasicPredParams::create()
{
    return new SnoopBasicPred(this);
}

