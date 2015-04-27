#ifndef __MEM_RUBY_COMMON_SNOOPBASICPRED_HH__
#define __MEM_RUBY_COMMON_SNOOPBASICPRED_HH__

#include <iostream>
#include <string>
#include <stdlib.h>

#include "base/statistics.hh"
#include "mem/ruby/common/NetDest.hh"
#include "mem/ruby/common/Address.hh"
#include "mem/protocol/MachineType.hh"
#include "params/RubySnoopBasicPred.hh"
#include "sim/sim_object.hh"

#include "mem/protocol/CacheRequestType.hh"
#include "mem/protocol/CoherenceRequestType.hh"

class SnoopBasicPred : public SimObject {
 public:
    SnoopBasicPred(const Params *p);
    ~SnoopBasicPred();

    virtual void regStats();

    virtual NetDest getPrediction(Address addr, MachineID local);

    enum CoherenceReqType{
        GETS,
        GETX,
        MC_GETS,
        PUTX,
    };

    virtual void profileInMsg(int reqNum, bool succ);
    void profilePred(int reqNum, bool succ);

    int getGETS();
    int getGETX();
    int getMC_GETS();
    int getPUTX();

    // MANDATORY SIM OBJECT METHODS
    SnoopBasicPred& operator=(const SnoopBasicPred& obj);

 private:

    Stats::Scalar inMsg_tot;
    Stats::Scalar inMsg_tot_peer;
    Stats::Formula inMsg_accuracy;
    Stats::Scalar inMsg_GetS;
    Stats::Scalar inMsg_GetX;
    Stats::Scalar inMsg_PutX;
    Stats::Scalar inMsg_PutS;
    Stats::Scalar inMsg_MC_GetX;
    Stats::Scalar inMsg_MC_GetS;
};

#endif // __MEM_RUBY_COMMON_SNOOPBASICPRED_HH__
