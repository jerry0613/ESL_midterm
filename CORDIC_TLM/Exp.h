#ifndef EXP_H_
#define EXP_H_

using namespace std;
#include <systemc>
using namespace sc_core;
using namespace sc_dt;
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include "system.h"

class Exp: public sc_module {
public:
    tlm_utils::simple_target_socket<Exp> t_skt;

    sc_fifo<sc_uint<16>> i_in;
    sc_fifo<sc_uint<24>> o_result;

    SC_HAS_PROCESS(Exp);
    Exp(sc_module_name n);
    ~Exp(); 
private:
    void do_exp();
    sc_uint<15> x_table[15] = {22713, 11357, 5678, 2839, 1661, 914, 482, 248, 126, 64, 32, 16, 8, 4, 2};
    sc_uint<16> x;
    sc_int<5> y_table[15] = {8, 4, 2, 1, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11};
    sc_uint<24> y;
    unsigned int base_offset;
    void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay);
};
#endif