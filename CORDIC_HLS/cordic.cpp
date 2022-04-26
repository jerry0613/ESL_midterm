// 1234
#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif
#include <iomanip>
#include "cordic.h"

const sc_uint<13> table[10] = {6800, 3593, 1824, 915, 457, 229, 114, 57, 28, 14};

CORDIC::CORDIC(sc_module_name n): sc_module(n) {
  SC_THREAD(do_CORDIC);
  sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
  i_theta.clk_rst(i_clk, i_rst);
  o_result_sin.clk_rst(i_clk, i_rst);
  HLS_FLATTEN_ARRAY( table );
#endif
}

CORDIC::~CORDIC() {};

void CORDIC::do_CORDIC() {
  #ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
    i_theta.reset();
    o_result_sin.reset();
  #endif
		wait();

  while(true) {
    HLS_PIPELINE_LOOP(SOFT_STALL, 1, "pipeline" );

    #ifndef NATIVE_SYSTEMC
      HLS_DEFINE_PROTOCOL("input");
      theta = (i_theta.get() - 45) << 8;
      wait();
    #else
      theta = (i_theta.read() - 45) << 8;
    #endif
    {
      HLS_CONSTRAIN_LATENCY(0, 1, "lat00");
      y = X0;
      x = X0;
      for (int i = 0; i < 10; i++) {
        //HLS_UNROLL_LOOP(ON, "loop");
        if (theta < 0) {
          theta = theta + table[i];
          reg_x = x;
          x = x + (y >> (i+1));
          y = y - (reg_x >> (i+1)); 
        }
        else {
          theta = theta - table[i];
          reg_x = x;
          x = x - (y >> (i+1));
          y = y + (reg_x >> (i+1)); 
        }
      }
    result.range(15, 0) = y;
    result.range(31, 16) = x;
    }
    #ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("output");
      o_result_sin.put(result);
    #else
      o_result_sin.write(result);
    #endif
  }
}