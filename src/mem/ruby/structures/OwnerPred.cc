#include "mem/ruby/structures/OwnerPred.hh"
#include "mem/ruby/system/System.hh"
#include "debug/RubyOwnerPred.hh"
#include "debug/RubySlicc.hh"

using namespace std;
typedef RubyOwnerPredParams Params;

OwnerPred::OwnerPred(const Params *p)
    :SimObject(p)
{
    profileData.MC_req_fromL1 = 0;
}

OwnerPred::~OwnerPred(){
    //DPRINTF(OwnerPred, "OwnerPred destructor is called\n");
}

NetDest OwnerPred::getPrediction(Address addr, MachineID local) {
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

  //fprintf(stdout, "[OwnerPred] getPrediction is called.\n");
  DPRINTF( RubyOwnerPred, "[OwnerPred] L1 Prediction: %s\n", prediction );
  return prediction;
}

void OwnerPred::profileRequestMsg(int reqNum){
    DPRINTF( RubyOwnerPred, "profileRequestMsg is called.\n" );
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

int OwnerPred::getGETS(){
    return GETS;
}

OwnerPred *
RubyOwnerPredParams::create()
{
    return new OwnerPred(this);
}

