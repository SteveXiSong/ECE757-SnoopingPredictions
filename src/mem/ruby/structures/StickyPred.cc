#include "mem/ruby/structures/StickyPred.hh"
#include "mem/ruby/system/System.hh"
#include "debug/RubySnoopPred.hh"
#include "debug/RubySlicc.hh"

using namespace std;

typedef RubyStickyPredParams Params;

StickyPred::StickyPred(const Params *p)
    :SnoopBasicPred(p)
{
    size_PredTable = 1024;
    num_StickyOneside = 10;
    predCache = new PredCache_t();
}

StickyPred::~StickyPred(){
    delete predCache;
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

NetDest StickyPred::getPredCachePrediction(PredCacheIndex index ){
    NetDest prediction = (*predCache)[index]->prediction;
    return prediction;
}

void StickyPred::addStikcyPredEntry(Address addr, MachineID provider, NetDest predMask){

    PredCacheIndex index = getPredCacheIndex(addr);
    PredBlock_t *newBlock = new PredBlock_t(addr, predMask, provider);

    pair<PredCacheIndex, PredBlock_t*> newEntry = make_pair(index, newBlock);
    predCache->insert( newEntry );

    bool result = updatePredCache(index);
    if( result == false){
        assert(0); // update pred cache failed
    }
}

PredCacheIndex StickyPred::getPredCacheIndex(Address addr){
    PredCacheIndex predIndex = addr.getAddress() % size_PredTable;
    return predIndex;
}

bool StickyPred::updatePredCache(PredCacheIndex thisIndex){
    assert(num_StickyOneside>=1);
    if(num_StickyOneside*2 > size_PredTable)
        return false;

    NetDest thisPred = getPredCachePrediction(thisIndex);

    // for positive side
    for( int i = 1; i <= num_StickyOneside; i++ ){
        NetDest itPred = (*predCache)[(thisIndex + i)%size_PredTable]->prediction;
        thisPred = thisPred.OR(itPred);
    }

    // for negative side
    for( int i = -1; i >= -num_StickyOneside; i-- ){
        NetDest itPred = (*predCache)[(thisIndex + i)%size_PredTable]->prediction;
        thisPred = thisPred.OR(itPred);
    }

    // update the entry
    (*predCache)[thisIndex]->prediction = thisPred;

    return true;
}
