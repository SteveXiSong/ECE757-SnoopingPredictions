#ifndef __MEM_RUBY_COMMON_OWNERPRED_HH__
#define __MEM_RUBY_COMMON_OWNERPRED_HH__

#include <iostream>
#include <string>
#include <stdlib.h>

#include "mem/ruby/common/NetDest.hh"
#include "mem/ruby/common/Address.hh"
#include "mem/protocol/MachineType.hh"
#include "params/RubyOwnerPred.hh"
#include "sim/sim_object.hh"

#include "mem/protocol/CacheRequestType.hh"
#include "mem/protocol/CoherenceRequestType.hh"

class OwnerPredL1Table;
class OwnerPredL2Table;

class OwnerPred : public SimObject {
 public:
    OwnerPred(const Params *p);
    ~OwnerPred();

    NetDest getPrediction(Address addr, MachineID local);

    enum CoherenceReqType{
        GETS,
        GETX,
        MC_GETS,
    };
    void profileRequestMsg(int reqNum);
    int getGETS();

    // MANDATORY SIM OBJECT METHODS
    OwnerPred & operator=(const OwnerPred & obj);

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

 private:
    profileData_t profileData;
    std::vector<OwnerPredL1Table> _L1TableEntryArray;
    std::vector<OwnerPredL2Table> _L2TableEntryArray;
    
};

class OwnerPredL1Table
{
  friend class OwnerPred;

  OwnerPredL1Table() : _confdCnt(1), _confdPtr(1), _nodePtr(0) {}
  unsigned _confdCnt : 2;   // 2-bit saturating counter regarding confidence about $2$ transfer;
  unsigned _confdPtr : 2;   // 2-bit saturating counter
  unsigned _nodePtr;
};

#endif 
