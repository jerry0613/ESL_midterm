#ifndef CORDIC_H_
#define CORDIC_H_
const int X0 = 9949;

using namespace std;
#include <systemc>
using namespace sc_core;
using namespace sc_dt;
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include "system.h"

class CORDIC: public sc_module {
public:
    tlm_utils::simple_target_socket<CORDIC> t_skt;

    sc_fifo<sc_int<8>> i_theta;
    sc_fifo<sc_int<16>> o_sin;
    sc_fifo<sc_int<16>> o_cos;

    SC_HAS_PROCESS(CORDIC);
    CORDIC(sc_module_name n);
    ~CORDIC(); 
private:
    void do_CORDIC();
    sc_int<16> table[10] = {6800, 3593, 1824, 915, 457, 229, 114, 57, 28, 14};
    sc_int<16> theta;
    sc_int<16> x, y, reg_x;
    unsigned int base_offset;
    void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay);
};
#endif