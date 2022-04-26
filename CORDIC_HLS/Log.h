#ifndef LOG_H_
#define LOG_H_

using namespace std;
#include <systemc>
using namespace sc_core;
using namespace sc_dt;
#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class Log: public sc_module {
public:
    sc_in_clk i_clk;
	sc_in < bool > i_rst;
#ifndef NATIVE_SYSTEMC
    cynw_p2p< sc_uint<24> >::in i_in_log;
    cynw_p2p< sc_uint<16> >::out o_result_log;
#else
    sc_fifo_in< sc_uint<24> > i_in_log;
    sc_fifo_out< sc_uint<16> > o_result_log;
#endif

    SC_HAS_PROCESS(Log);
    Log(sc_module_name n);
    ~Log(); 
private:
    void do_log();
    sc_uint<16> y;
    sc_uint<24> x;
};
#endif