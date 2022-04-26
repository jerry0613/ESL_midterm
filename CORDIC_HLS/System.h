#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <systemc>
using namespace sc_core;
 //1
#include "Testbench.h"
#ifndef NATIVE_SYSTEMC
#include "cordic_wrap.h"
#include "Exp_wrap.h"
#include "Log_wrap.h"
#else
#include "cordic.h"
#include "Exp.h"
#include "Log.h"
#endif

class System: public sc_module
{
public:
	SC_HAS_PROCESS( System );
	System( sc_module_name n );
	~System();
private:
  Testbench tb;
#ifndef NATIVE_SYSTEMC
	CORDIC_wrapper CORDIC;
	Exp_wrapper Exp;
	Log_wrapper Log;
#else
	CORDIC CORDIC;
	Log Exp;
	Log Log;
#endif
	sc_clock clk;
	sc_signal<bool> rst_sin, rst_exp, rst_log;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_int<32> > result_sin;
	cynw_p2p< sc_dt::sc_uint<24> > result_exp;
	cynw_p2p< sc_dt::sc_uint<16> > result_log;

	cynw_p2p< sc_dt::sc_uint<8> > theta;
	cynw_p2p< sc_dt::sc_uint<16> > in_exp;
	cynw_p2p< sc_dt::sc_uint<24> > in_log;
#else
	sc_fifo< sc_dt::sc_int<32> > result_sin;
	//sc_fifo< sc_dt::sc_int<16> > result_cos;
	sc_fifo< sc_dt::sc_uint<24> > result_exp;
	sc_fifo< sc_dt::sc_uint<16> > result_log;

	sc_fifo< sc_dt::sc_uint<8> > theta;
	sc_fifo< sc_dt::sc_uint<16> > in_exp;
	sc_fifo< sc_dt::sc_uint<24> > in_log;
#endif
};
#endif
//