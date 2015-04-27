#include "mem/ruby/structures/StickyPred.hh"
#include "mem/ruby/system/System.hh"
#include "debug/RubySnoopPred.hh"
#include "debug/RubySlicc.hh"

using namespace std;

typedef RubyStickyPredParams Params;

StickyPred::StickyPred(const Params *p)
    :SnoopBasicPred(p)
{
    predCache.clear();
}

StickyPred::~StickyPred(){
}

StickyPred *
RubyStickyPredParams::create()
{
    return new StickyPred(this);
}

NetDest StickyPred::getPrediction(Address addr, MachineID local) {
    NetDest prediction;

    // add L1 nodes to mask
    prediction.add(local);

    // add L2 node to mask
    for (NodeID i = 0; i < MachineType_base_count(MachineType_L2Cache); i++) {
      MachineID mach = {MachineType_L2Cache, i};
      prediction.add(mach);
    }

    // get prediction from PredCache

    DPRINTF(RubySnoopPred, "[StickyPred] L1 Prediction: %s\n", prediction);
    return prediction;
}

NetDest StickyPred::getPredCacheNetDest(Address addr){
    PredBlock_t getPredBlock= predCache.find(addr)->second;
    return getPredBlock.first;
}

void StickyPred::addStikcyPredEntry(Address addr, MachineID provider, NetDest predMask){
    pair<Address, PredBlock_t> newEntry = make_pair(addr, make_pair(predMask,provider));
    predCache.insert( newEntry );
    if(updatePredCache(addr) == true){

    }else{
        assert(0); // update pred cache failed
    }
}

bool StickyPred::updatePredCache(Address addr){

    return true;
}
