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

OwnerPred::~OwnerPred()
{
    //DPRINTF(OwnerPred, "OwnerPred destructor is called\n");
}

NetDest OwnerPred::getPrediction(Address pc, Address addr, MachineID local) 
{
  
  NetDest prediction;

  //  indexed by PC

  const size_t pcIndx = (pc.getAddress() & ~pc.maskLowOrderBits( _tableLgSize ));
  const OwnerPredL1Table & l1table = _L1TableEntryArray[pcIndx];
  const NodeID l1predNodePtr = l1table.getNodePtr();

  //  1st-level predictor 
  bool isC2C = false;       //  predicted as a $2$ transfer miss

  prediction.add(local);
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
        if( l2table.getValidBit(i) && l2table.getNodePtr(i) != l1predNodePtr ) {
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
}

void OwnerPred::updatePredictionTable( Address pc, MachineID realOwner )
{
  //  increment the $2$ confidence
  const size_t pcIndx = (pc.getAddress() & ~pc.maskLowOrderBits( _tableLgSize ));
  OwnerPredL1Table & l1table = _L1TableEntryArray[pcIndx];

  if( realOwner.getType() == MachineType_L2Cache ) {       //  the sender is from L2
    l1table.confdCntDn();
    return;
  }

  //  $2$ transfer between L1
  l1table.confdCntUp();

  const NodeID ownerID = realOwner.getNum();
  if( ownerID == l1table.getNodePtr() )
    l1table.confdPtrUp();
  else 
    l1table.confdPtrDn();

  OwnerPredL2Table & l2table = _L2TableEntryArray[pcIndx];
  l2table.updateWithRealOwnerID( ownerID );
}

void OwnerPred::profileRequestMsg(int reqNum)
{
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

int OwnerPred::getGETS()
{
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

OwnerPredL1Table::~OwnerPredL1Table() 
{}

OwnerPredL2Table::OwnerPredL2Table()
  : _confdPtr(1) 
{
  for( size_t i = 0; i < 4; i ++ ) {
    _nodePtrArray[i] = 0;
    _nodeValidArray[i] = false; 
    _missedTimesArray[i] = 0;
  }
}

OwnerPredL2Table::~OwnerPredL2Table() {}

NodeID OwnerPredL2Table::getNodePtr(size_t idx) const {
  assert( 0 <= idx && idx < 4 );
  return _nodePtrArray[idx];
}

bool OwnerPredL2Table::getValidBit(size_t idx) const {
  assert( 0 <= idx && idx < 4 );
  return _nodeValidArray[idx];
}

void OwnerPredL2Table::updateWithRealOwnerID( NodeID ownerID )
{
  //  check if l2table prediction hit
  bool hit = false;
  for( size_t i = 0;  i < 4; i++ ) {
    hit |= ( getValidBit(i)&&getNodePtr(i)==ownerID );
  }

  if( hit ) {
    confdPtrUp();
    for( size_t i = 0; i < 4; i++ ) {
      if( getValidBit(i) ) {
        if( getNodePtr(i) == ownerID )
          _missedTimesArray[i] = 0;
        else
          ++ _missedTimesArray[i];
      }
    }
  }
  else {
    confdPtrDn();
    //  evict the LRU
    size_t evictIdx = 0;
    for( size_t i = 1; i < 4; i++ ) {
      if( 
          ( getValidBit(evictIdx)==true && getValidBit(i)==false )
          ||
          ( getValidBit(evictIdx)==true && getValidBit(i)==true && _missedTimesArray[i]>_missedTimesArray[evictIdx] ) 
          )
        evictIdx = i;
    }
    replaceNodePtr( evictIdx, ownerID );
  }
}

void OwnerPredL2Table::replaceNodePtr( size_t idx, NodeID ownerID ) 
{
  _nodePtrArray[idx] = ownerID;
  _missedTimesArray[idx] = 0;
  _nodeValidArray[idx] = true;
}

