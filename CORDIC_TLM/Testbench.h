#ifndef TESTBENCH_H_
#define TESTBENCH_H_

using namespace std;
#include <systemc>
using namespace sc_core;

#include "Initiator.h"
#include "MemoryMap.h"
#include "system.h"

class Testbench : public sc_module {
public:
  Initiator initiator;

  SC_HAS_PROCESS(Testbench);

  Testbench(sc_module_name n);
  ~Testbench();

private:
  void compute_sin();
  void compute_exp();
  void compute_log();
  void run_time();
  int flag;
};
#endif
