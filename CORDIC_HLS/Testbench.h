#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
using namespace std;

#include <systemc>
using namespace sc_core;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class Testbench : public sc_module {
public:
	sc_in_clk i_clk;
	sc_out < bool > o_rst_sin, o_rst_exp, o_rst_log;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_dt::sc_int<32> >::base_in i_result_sin;
  //cynw_p2p< sc_dt::sc_int<16> >::base_in i_result_cos;
  cynw_p2p< sc_dt::sc_uint<24> >::base_in i_result_exp;
  cynw_p2p< sc_dt::sc_uint<16> >::base_in i_result_log;

  cynw_p2p< sc_dt::sc_uint<8> >::base_out o_theta;
  cynw_p2p< sc_dt::sc_uint<16> >::base_out o_in_exp;
  cynw_p2p< sc_dt::sc_uint<24> >::base_out o_in_log;
#else
	sc_fifo_in< sc_dt::sc_int<32> > i_result_sin;
  //sc_fifo_in< sc_dt::sc_int<16> > i_result_cos;
  sc_fifo_in< sc_dt::sc_uint<24> > i_result_exp;
  sc_fifo_in< sc_dt::sc_uint<16> > i_result_log;

  sc_fifo_out< sc_dt::sc_uint<8> > o_theta;
  sc_fifo_out< sc_dt::sc_uint<16> > o_in_exp;
  sc_fifo_out< sc_dt::sc_uint<24> > o_in_log;
#endif

  SC_HAS_PROCESS(Testbench);
  Testbench(sc_module_name n);
  ~Testbench();
private:
	unsigned int n_txn;
	sc_time max_txn_time;
	sc_time min_txn_time;
	sc_time total_txn_time;
	sc_time total_start_time;
	sc_time total_run_time;
  sc_time CORDIC_time;
  sc_time Exp_time;
  sc_time Log_time;
  int flag;

  void feed_sin();
  void feed_exp();
  void feed_log();
	void fetch_sin_result();
  void fetch_exp_result();
  void fetch_log_result();
  void run_time();
};
#endif
