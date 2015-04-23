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

class SnoopBasicPred : public SimObject {
 public:
  typedef RubySnoopBasicPredParams Params;
  SnoopBasicPred(const Params *p) : SimObject(p) {};
  NetDest getPrediction(Address addr, MachineID local);

  // MANDATORY SIM OBJECT METHODS
  SnoopBasicPred& operator=(const SnoopBasicPred& obj);
};

#endif // __MEM_RUBY_COMMON_SNOOPBASICPRED_HH__
