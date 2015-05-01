#ifndef __MEM_RUBY_COMMON_STICKYPRED_HH__
#define __MEM_RUBY_COMMON_STICKYPRED_HH__

#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include <utility>

#include "mem/ruby/structures/SnoopBasicPred.hh"
#include "params/RubyStickyPred.hh"

using namespace std;

class PredBlock_t{
    public:
        PredBlock_t(Address Tag, NetDest Pred, MachineID Inv){
            tag = Tag;
            prediction = Pred;
            invalidator = Inv;
        }

        NetDest prediction;
        MachineID invalidator;
        Address tag;

        bool mergeInv(){
            if( tag.getAddress() != 0){
                prediction.add(invalidator);
                return true;
            }
            return false;
        }
};

typedef unsigned int PredCacheIndex;
typedef map<PredCacheIndex, PredBlock_t*> PredCache_t;

class StickyPred : public SnoopBasicPred {
 public:
    typedef RubyStickyPredParams Params;
    StickyPred(const Params *p);
    ~StickyPred();

    // ruby interface
    NetDest getPrediction(Address addr, MachineID local);
    void addPrediction(Address addr, MachineID provider, NetDest predMask);
    bool invalidatePrediction(Address addr, MachineID inv);

    void dumpPredCache();

    // MANDATORY SIM OBJECT METHODS
    StickyPred& operator=(const StickyPred& obj);

 private:
    void addStickyPredEntry(Address addr, MachineID provider, NetDest predMask);
    bool invalidatePredCacheEntry(Address addr, MachineID inv);
    NetDest getPredCachePrediction(PredCacheIndex index);
    bool updatePredCache(PredCacheIndex thisIndex);
    PredCacheIndex getPredCacheIndex(Address addr);
    Address getPredCacheTag(PredCacheIndex index);
    void resetPredCacheTag(PredCacheIndex index, MachineID inv);
    void setPredCacheEntry(Address addr, NetDest providedPrediction, MachineID inv);
    bool isValidEntry(PredCacheIndex index);
    bool isTagMatch(Address addr, PredCacheIndex index);
    void mergeMaskIntoEntry(PredCacheIndex index, NetDest predMask);

 private:
    int size_PredTable;
    int num_StickyOneside;

    MachineID localMachine;
    /* the pred cache table's organization is like this
     * index ->|tag(address) |  mask | last invalidator |
     *         |             |       |                  |
     */
    PredCache_t *predCache;
};

#endif // __MEM_RUBY_COMMON_SNOOPBASICPRED_HH__
