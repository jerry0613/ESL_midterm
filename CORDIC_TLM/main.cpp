#include <iostream>
#include <string>
using namespace std;

// Wall Clock Time Measurement
#include <sys/time.h>
#include "SimpleBus.h"
#include "Testbench.h"
#include "cordic.h"
#include "Exp.h"
#include "Log.h"

// TIMEVAL STRUCT IS Defined ctime
// use start_time and end_time variables to capture
// start of simulation and end of simulation
struct timeval start_time, end_time;

int sc_main(int argc, char **argv) {
  Testbench tb("tb");
  CORDIC CORDIC("CORDIC");
  Exp Exp("Exp");
  Log Log("Log");
  SimpleBus<1, 3> bus("bus");
  bus.set_clock_period(sc_time(CLOCK_PERIOD, SC_NS));
  tb.initiator.i_skt(bus.t_skt[0]);
  bus.setDecode(0, CORDIC_MM_BASE, CORDIC_MM_BASE + CORDIC_MM_SIZE - 1);
  bus.i_skt[0](CORDIC.t_skt);
  bus.setDecode(1, EXP_MM_BASE, EXP_MM_BASE + EXP_MM_SIZE - 1);
  bus.i_skt[1](Exp.t_skt);
  bus.setDecode(2, LOG_MM_BASE, LOG_MM_BASE + LOG_MM_SIZE - 1);
  bus.i_skt[2](Log.t_skt);

  sc_start();
  std::cout << "Simulated time == " << sc_core::sc_time_stamp() << std::endl;

  return 0;
}
