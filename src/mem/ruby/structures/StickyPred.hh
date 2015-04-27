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

typedef pair<NetDest, MachineID> PredBlock_t;
typedef map<Address, PredBlock_t> PredCache_t;

class StickyPred : public SnoopBasicPred {
 public:
    StickyPred(const Params *p);
    ~StickyPred();

    NetDest getPrediction(Address addr, MachineID local);
    void addStikcyPredEntry(Address addr, MachineID provider, NetDest predMask);
    NetDest getPredCacheNetDest(Address addr);
    bool updatePredCache(Address addr);

    // MANDATORY SIM OBJECT METHODS
    StickyPred& operator=(const StickyPred& obj);

 private:
    int size_SnoopTable;
    PredCache_t predCache;
};

#endif // __MEM_RUBY_COMMON_SNOOPBASICPRED_HH__
