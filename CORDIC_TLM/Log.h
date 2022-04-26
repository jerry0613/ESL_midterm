#ifndef LOG_H_
#define LOG_H_

using namespace std;
#include <systemc>
using namespace sc_core;
using namespace sc_dt;
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include "system.h"

class Log: public sc_module {
public:
    tlm_utils::simple_target_socket<Log> t_skt;

    sc_fifo<sc_uint<24>> i_in;
    sc_fifo<sc_int<16>> o_result;

    SC_HAS_PROCESS(Log);
    Log(sc_module_name n);
    ~Log(); 
private:
    void do_log();
    sc_int<15> y_table[15] = {11357, 5678, 2839, 1420, 830, 457, 241, 124, 63, 32, 16, 8, 4, 2, 1};
    sc_int<16> y;
    sc_int<5> x_table[15] = {8, 4, 2, 1, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11};
    sc_uint<24> x;
    unsigned int base_offset;
    void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay);
};
#endif