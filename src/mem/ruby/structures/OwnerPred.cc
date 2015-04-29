#include "mem/ruby/structures/OwnerPred.hh"
#include "mem/ruby/system/System.hh"
#include "debug/RubyOwnerPred.hh"
#include "debug/RubySlicc.hh"

using namespace std;
typedef RubyOwnerPredParams Params;

OwnerPred::OwnerPred(const Params *p)
    :SimObject(p), _tableLgSize( 6 )
{
    profileData.MC_req_fromL1 = 0;

    _tableSize = ( 1 << _tableLgSize );
    _L1TableEntryArray.resize( _tableSize );
    _L2TableEntryArray.resize( _tableSize );
}

OwnerPred::~OwnerPred(){
    //DPRINTF(OwnerPred, "OwnerPred destructor is called\n");
}

NetDest OwnerPred::getPrediction(Address addr, MachineID local)
{
  NetDest prediction;
  return prediction;
}

NetDest OwnerPred::getPrediction(Address pc, Address addr, MachineID local) {
  
  NetDest prediction;

  //  indexed by PC

  const size_t pcIndx = (pc.getAddress() & ~pc.maskLowOrderBits( _tableLgSize ));
  const OwnerPredL1Table & l1table = _L1TableEntryArray[pcIndx];
  const size_t l1predNodePtr = l1table.getNodePtr();

  //  1st-level predictor 
  bool isC2C = false;       //  predicted as a $2$ transfer miss
  if( l1table.getConfdCnt() >= 2 ) {
    isC2C = true;
    if( l1table.getConfdPtr() >=2 ) {
      MachineID l1pred = { MachineType_L1Cache, l1table.getNodePtr() };
      prediction.add(l1pred);
    }
  }

  if( isC2C ) {
    //  2nd-level predictor
    const OwnerPredL2Table & l2table = _L2TableEntryArray[pcIndx];

    if( l2table.getConfdPtr() >= 2 ) 
      for( size_t i = 0; i < 4; i++ ) {
        if( l2table.getValidBit( i ) && l2table.getNodePtr(i) != l1predNodePtr ) {
          MachineID l2pred = { MachineType_L1Cache, l2table.getNodePtr(i) };
          prediction.add(l2pred);
        }
      }
  }

  // add L2 nodes to mask
  for (NodeID i = 0; i < MachineType_base_count(MachineType_L2Cache); i++) {
    MachineID mach = {MachineType_L2Cache, i};
    prediction.add(mach);
  }

  return prediction;

  /*
  //fprintf(stdout, "[OwnerPred] getPrediction is called.\n");
  DPRINTF( RubyOwnerPred, "[OwnerPred] L1 Prediction: %s\n", prediction );
  return prediction;  */
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

OwnerPredL1Table::OwnerPredL1Table() 
  : _confdCnt(1), _confdPtr(1), _nodePtr(0) 
{}

OwnerPredL1Table::~OwnerPredL1Table() {
}

OwnerPredL2Table::OwnerPredL2Table()
  : _confdPtr(1) 
{
  for( size_t i = 0; i < 4; i ++ ) {
    _nodePtrArray[i] = 0;
    _nodeValidArray[i] = false; 
  }
}

OwnerPredL2Table::~OwnerPredL2Table() {
}

