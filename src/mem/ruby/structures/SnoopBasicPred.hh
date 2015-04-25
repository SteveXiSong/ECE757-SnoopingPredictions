#ifndef __MEM_RUBY_COMMON_SNOOPBASICPRED_HH__
#define __MEM_RUBY_COMMON_SNOOPBASICPRED_HH__

#include <iostream>
#include <string>
#include <stdlib.h>

#include "mem/ruby/common/NetDest.hh"
#include "mem/ruby/common/Address.hh"
#include "mem/protocol/MachineType.hh"
#include "params/RubySnoopBasicPred.hh"
#include "sim/sim_object.hh"

#include "mem/protocol/CacheRequestType.hh"
#include "mem/protocol/CoherenceRequestType.hh"

class SnoopBasicPred : public SimObject {
 public:
    //SnoopBasicPred(const SN)
    SnoopBasicPred(const Params *p);
    ~SnoopBasicPred();

    NetDest getPrediction(Address addr, MachineID local);

    enum CoherenceReqType{
        GETS,
        GETX,
        MC_GETS,
    };
    void profileRequestMsg(int reqNum);
    int getGETS();

    // MANDATORY SIM OBJECT METHODS
    SnoopBasicPred& operator=(const SnoopBasicPred& obj);

    struct profileData_t{
      int MC_req_fromL1;
      int tot_req;
      int tot_req_fromL1;
      int GetS;
      int GetX;
      int MC_GetX;
      int MC_GetS;
    };

    typedef profileData_t profileData_t;
    //typedef RubySnoopBasicPredParams Params;

 private:
    profileData_t profileData;
};

#endif // __MEM_RUBY_COMMON_SNOOPBASICPRED_HH__
