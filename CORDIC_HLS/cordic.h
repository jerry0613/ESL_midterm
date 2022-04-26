#ifndef CORDIC_H_
#define CORDIC_H_
#define X0 9949;

using namespace std;
#include <systemc>
using namespace sc_core;
using namespace sc_dt;

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class CORDIC: public sc_module {
public:
    sc_in_clk i_clk;
	sc_in < bool > i_rst;

#ifndef NATIVE_SYSTEMC
    cynw_p2p< sc_uint<8> >::in i_theta;
    cynw_p2p< sc_int<32> >::out o_result_sin;
#else
    sc_fifo_in< sc_uint<8> > i_theta;
    sc_fifo_out< sc_int<32> > o_result_sin;
#endif

    SC_HAS_PROCESS(CORDIC);
    CORDIC(sc_module_name n);
    ~CORDIC(); 
private:
    void do_CORDIC();
    
    sc_int<16> theta;
    sc_int<16> x, y, reg_x;
    sc_int<32> result;
};
#endif