// 1234
#include <cmath>
#include <iomanip>
#include "Exp.h"
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif
 
const sc_uint<15> x_table[15] = {22713, 11357, 5678, 2839, 1661, 914, 482, 248, 126, 64, 32, 16, 8, 4, 2};
const sc_int<5> y_table[15] = {8, 4, 2, 1, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11};

Exp::Exp(sc_module_name n): sc_module(n) {
  SC_THREAD(do_exp);
  sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

  #ifndef NATIVE_SYSTEMC
    i_in_exp.clk_rst(i_clk, i_rst);
    o_result_exp.clk_rst(i_clk, i_rst);
    HLS_FLATTEN_ARRAY( x_table );
    HLS_FLATTEN_ARRAY( y_table );
  #endif
}

Exp::~Exp() {};

void Exp::do_exp() {
  #ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
    i_in_exp.reset();
    o_result_exp.reset();
  #endif
		wait();
  
  while(true) {
    HLS_PIPELINE_LOOP(SOFT_STALL, 1, "Loop" );
    #ifndef NATIVE_SYSTEMC
      HLS_DEFINE_PROTOCOL("input");
      x = i_in_exp.get();
      wait();
    #else
      x = i_in_exp.read();
    #endif
    {
      HLS_CONSTRAIN_LATENCY(0, 1, "lat00");
      y = 1 << 9;
      for (int i = 0; i < 15; i++) {
        if (x >= x_table[i]) {
          x -= x_table[i];
          if (i < 4)
            y = y << y_table[i];
          else
            y += (y >> -y_table[i]);
        }
      }
    }
    #ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("output");
      o_result_exp.put(y);
    #else
      o_result_exp.write(y);
    #endif
  }
}
