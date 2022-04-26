#ifndef EXP_H_
#define EXP_H_

using namespace std;
#include <systemc>
using namespace sc_core;
using namespace sc_dt;
#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class Exp: public sc_module {
public:
    sc_in_clk i_clk;
	sc_in < bool > i_rst;

#ifndef NATIVE_SYSTEMC
    cynw_p2p< sc_uint<16> >::in i_in_exp;
    cynw_p2p< sc_uint<24> >::out o_result_exp;
#else
    sc_fifo_in< sc_uint<16> > i_in_exp;
    sc_fifo_out< sc_uint<24> > o_result_exp;
#endif

    SC_HAS_PROCESS(Exp);
    Exp(sc_module_name n);
    ~Exp(); 
private:
    void do_exp();
    sc_uint<16> x;
    sc_uint<24> y;
};
#endif