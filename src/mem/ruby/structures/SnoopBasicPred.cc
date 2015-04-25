#include "mem/ruby/structures/SnoopBasicPred.hh"
#include "mem/ruby/system/System.hh"
#include "debug/RubySnoopPred.hh"
#include "debug/RubySlicc.hh"

using namespace std;

typedef RubySnoopBasicPredParams Params;
SnoopBasicPred::SnoopBasicPred(const Params *p)
    :SimObject(p)
{
    //profileData.MC_req_fromL1 = 0;
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

void SnoopBasicPred::profileInMsg(int reqNum, bool succ){
    //DPRINTF(RubySnoopPred, "profileRequestMsg is called.\n");
    inMsg_tot++;
    if( succ == false)
        return;
    switch(reqNum){
        case GETX:
            inMsg_GetX++;
            break;
        case GETS:
            inMsg_GetS++;
            break;
        default:
            break;
    }
    return;
}

int SnoopBasicPred::getGETS(){
    return GETS;
}

int SnoopBasicPred::getGETX(){
    return GETX;
}

int SnoopBasicPred::getMC_GETS(){
    return MC_GETS;
}

int SnoopBasicPred::getPUTX(){
    return PUTX;
}

void SnoopBasicPred::regStats(){
   inMsg_tot
       .name(name()+".SnoopPred.tot_inMsg_network")
       .desc("..");

   //inMsg_tot_peer
   //    .name(name()+".tot_inMsg_peer")
   //    .desc("..");

   inMsg_GetS
       .name(name()+".SnoopPred.tot_inMsg_GetS")
       .desc("..");

   inMsg_GetX
       .name(name()+".SnoopPred.tot_inMsg_GetX")
       .desc("..");

   inMsg_PutX
       .name(name()+".SnoopPred.tot_inMsg_PutX")
       .desc("..");

   inMsg_accuracy
       .name(name()+".SnoopPred.inMsg_accuracy")
       .desc("..");

   inMsg_accuracy = (inMsg_GetS+inMsg_GetX+inMsg_PutX+inMsg_PutS)/(inMsg_tot);
}


SnoopBasicPred *
RubySnoopBasicPredParams::create()
{
    return new SnoopBasicPred(this);
}

