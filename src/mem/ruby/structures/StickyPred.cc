#include "mem/ruby/structures/StickyPred.hh"
#include "mem/ruby/system/System.hh"
#include "debug/RubySnoopPred.hh"
#include "debug/RubySlicc.hh"

using namespace std;

typedef RubyStickyPredParams Params;

#define TABLE_SIZE 256
#define STICKY_NUM 40

StickyPred::StickyPred(const Params *p)
    :SnoopBasicPred(p)
{
    size_PredTable = TABLE_SIZE;
    num_StickyOneside = STICKY_NUM;
    predCache = new PredCache_t();

    fprintf(stdout, "StickyPred class is constructed:\n");
    fprintf(stdout, "Number of PredTable entries: %d\n", size_PredTable);
    fprintf(stdout, "Number of Sticky entries(one side): %d\n", num_StickyOneside);
}

StickyPred::~StickyPred(){
    delete predCache;
}

StickyPred *
RubyStickyPredParams::create()
{
    return new StickyPred(this);
}

// this is for Ruby machine
NetDest StickyPred::getPrediction(Address addr, MachineID local) {
    NetDest prediction;

    // add L1 nodes to mask
    prediction.add(local);

    // add L2 node to mask
    for (NodeID i = 0; i < MachineType_base_count(MachineType_L2Cache); i++) {
      MachineID mach = {MachineType_L2Cache, i};
      prediction.add(mach);
    }

    // TODO: this could be not totally good
    PredCacheIndex index = getPredCacheIndex(addr);
    if( isValidEntry(index) == true ){
        // get prediction from PredCache
        bool result = updatePredCache(index);
        if( result == false){
            //assert(0); // update pred cache failed
        }
        prediction = prediction.OR(getPredCachePrediction(index));
    }

    DPRINTF(RubySnoopPred, "[StickyPred] L1 Prediction: %s\n", prediction);
    return prediction;
}

NetDest StickyPred::getPredCachePrediction(PredCacheIndex index ){
    assert( isValidEntry(index) == true );
    assert( (*predCache)[index]->tag.getAddress() != 0 );

    NetDest prediction = (*predCache)[index]->prediction;
    return prediction;
}

void StickyPred::addStickyPredEntry(Address addr, MachineID provider, NetDest predMask){

    PredCacheIndex index = getPredCacheIndex(addr);

    if(isValidEntry(index) ){
        if( isTagMatch(addr, index) ){
            // tag matched, update
            mergeMaskIntoEntry(index, predMask);
        }
        else{
            // not matched, set it
            setPredCacheEntry(addr, predMask, provider);
        }
        return ;
    }else{
        //is not presented
        //replace the original data
        PredBlock_t *newBlock = new PredBlock_t(addr, predMask, provider);
        (*predCache)[index] = newBlock;
        return ;
    }
    return ;
}

PredCacheIndex StickyPred::getPredCacheIndex(Address addr){
    PredCacheIndex predIndex = addr.getAddress() % size_PredTable;
    return predIndex;
}

bool StickyPred::updatePredCache(PredCacheIndex thisIndex){
    assert( isValidEntry(thisIndex) == true );
    assert( num_StickyOneside>=1 );

    if(num_StickyOneside*2 > size_PredTable)
        num_StickyOneside = size_PredTable/2 - 1;

    if(num_StickyOneside*2 > predCache->size())
        return false;

    NetDest thisPred = getPredCachePrediction(thisIndex);

    // for positive side
    for( int i = 1; i <= num_StickyOneside; i++ ){
        PredCacheIndex itIndex = (thisIndex + i)%size_PredTable;
        if( isValidEntry(itIndex) == false )
            continue;
        NetDest itPred = getPredCachePrediction(itIndex);
        thisPred = thisPred.OR(itPred);
    }

    // for negative side
    for( int i = -1; i >= -num_StickyOneside; i-- ){
        PredCacheIndex itIndex = (thisIndex + i)%size_PredTable;
        if( isValidEntry(itIndex) == false )
            continue;
        NetDest itPred = getPredCachePrediction(itIndex);
        thisPred = thisPred.OR(itPred);
    }

    // update the entry
    (*predCache)[thisIndex]->prediction = thisPred;
    (*predCache)[thisIndex]->mergeInv();

    return true;
}

Address StickyPred::getPredCacheTag(PredCacheIndex index){
    return (*predCache)[index]->tag;
}

bool StickyPred::invalidatePredCacheEntry(Address addr, MachineID inv){
    PredCacheIndex thisIndex = getPredCacheIndex(addr);
    if(isValidEntry(thisIndex) == true){
    //if( addr.getAddress() == getPredCacheTag(thisIndex).getAddress() ){
        resetPredCacheTag(thisIndex, inv);
        return true;
    }
    else{
        // is not presented
        return false;
    }
    return false;
}

void StickyPred::resetPredCacheTag(PredCacheIndex index, MachineID inv){
    (*predCache)[index]->tag.setAddress(0);
    (*predCache)[index]->prediction.clear();
    (*predCache)[index]->invalidator = inv;
    //predCache->erase(index);
}

bool StickyPred::isValidEntry(PredCacheIndex index){
    //assert( isValidEntry(index) == true );
    if( predCache->find(index) != predCache->end() ){
        if( (*predCache)[index]->tag.getAddress() == 0 )
            return false;
        else
            return true;
    }
    else
        return false;

    return false;
}

void StickyPred::addPrediction(Address addr, MachineID provider, NetDest predMask){
    addStickyPredEntry(addr, provider, predMask);
}


bool StickyPred::invalidatePrediction(Address addr, MachineID inv){
    return invalidatePredCacheEntry(addr, inv);
}

void StickyPred::dumpPredCache(){
    DPRINTF(RubySnoopPred, "----Dumping PredCache---------");
    DPRINTF(RubySnoopPred, "Index\t|Tag\t\t| Mask\t\t|Invalidator\n");
    for(PredCache_t::iterator it = predCache->begin();
            it != predCache->end(); it++){
        DPRINTF(RubySnoopPred, "0x%x\t", it->first);
        DPRINTF(RubySnoopPred, "0x%lx\t", it->second->tag.getAddress());
        //Debug::RubySnoopPred << it->second->prediction << "\t";
    }
}

bool StickyPred::isTagMatch(Address addr, PredCacheIndex index){
    assert( isValidEntry(index) == true );
    return (getPredCacheTag(index).getAddress() == addr.getAddress())? true:false;
}

void StickyPred::setPredCacheEntry(Address addr, NetDest providedPrediction, MachineID provider){
    PredCacheIndex thisIndex = getPredCacheIndex(addr);
    (*predCache)[thisIndex]->tag = addr;
    (*predCache)[thisIndex]->prediction = providedPrediction;
    (*predCache)[thisIndex]->invalidator = provider;
}

void StickyPred::mergeMaskIntoEntry(PredCacheIndex index, NetDest predMask){
    assert( isValidEntry(index) == true );
    (*predCache)[index]->prediction.OR(predMask);
}
