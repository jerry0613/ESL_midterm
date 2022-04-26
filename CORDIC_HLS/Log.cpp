// 1234
#include <cmath>
#include <iomanip>
#include "Log.h"
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif
 
const sc_uint<14> y_table[15] = {11357, 5678, 2839, 1420, 830, 457, 241, 124, 63, 32, 16, 8, 4, 2, 1};
const sc_int<5> x_table[15] = {8, 4, 2, 1, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11};

Log::Log(sc_module_name n): sc_module(n) {
  SC_THREAD(do_log);
  sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

  #ifndef NATIVE_SYSTEMC
    i_in_log.clk_rst(i_clk, i_rst);
    o_result_log.clk_rst(i_clk, i_rst);
    HLS_FLATTEN_ARRAY( x_table );
    HLS_FLATTEN_ARRAY( y_table );
  #endif 
}

Log::~Log() {};

void Log::do_log() {
  #ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
    i_in_log.reset();
    o_result_log.reset();
  #endif
		wait();
  
  while(true) {
    HLS_PIPELINE_LOOP(SOFT_STALL, 1, "Loop" );
    #ifndef NATIVE_SYSTEMC
      HLS_DEFINE_PROTOCOL("input");
      x = i_in_log.get();
      wait();
    #else
      x = i_in_log.read();
    #endif
    {
      HLS_CONSTRAIN_LATENCY(0, 1, "lat00");
      y = 0;
      for (int i = 0; i < 15; i++) {
        if (i < 4) {
          if (x >> x_table[i] >= 512) {
            x = x >> x_table[i];
            y += y_table[i];
          }
        } else {
          if (x - (x >> -x_table[i]) >= 512) {
            x -= x >> -x_table[i];
            y += y_table[i];
          }
        }
      }
    }
    #ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("output");
      o_result_log.put(y);
    #else
      o_result_log.write(y);
    #endif
  }
}
