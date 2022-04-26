#include <iomanip>
#include "Log.h"

Log::Log(sc_module_name n): sc_module(n), t_skt("t_skt"), base_offset(0) {
  SC_THREAD(do_log);
  t_skt.register_b_transport(this, &Log::blocking_transport);
}

Log::~Log() = default;

void Log::do_log() {
  while(true) {
    x = i_in.read();
    y = 0;

    for (int i = 0; i < 15; i++) {
      if (i < 4) {
        if (x >> x_table[i] >= 512) {
          x = x >> x_table[i];
          y += y_table[i];
        }
      } else {
        if (x - (x >> -x_table[i]) >= 512) {
          x -= x >> -x_table[i];
          y += y_table[i];
        }
      }
    }
    o_result.write(y);
  }
}

void Log::blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay) {
  sc_dt::uint64 addr = payload.get_address();
  addr -= base_offset;
  unsigned char *mask_ptr = payload.get_byte_enable_ptr();
  unsigned char *data_ptr = payload.get_data_ptr();
  package buffer;
  sc_uint<16> tmp_y;
  switch (payload.get_command()) {
  case tlm::TLM_READ_COMMAND:
    switch (addr) {
    case LOG_Y_ADDR:
      tmp_y = o_result.read();
      buffer.uc[0] = tmp_y.range(7, 0);
      buffer.uc[1] = tmp_y.range(15, 8);
      break;
    case LOG_CHECK_ADDR:
      buffer.uc[0] = o_result.num_available();
      break;  
    default:
      buffer.uc[0] = 0;
      buffer.uc[1] = 0;
      buffer.uc[2] = 0;
      buffer.uc[3] = 0;
    }
    data_ptr[0] = buffer.uc[0];
    data_ptr[1] = buffer.uc[1];
    data_ptr[2] = buffer.uc[2];
    data_ptr[3] = buffer.uc[3];
    break;
  case tlm::TLM_WRITE_COMMAND:
    switch (addr) {
    case LOG_X_ADDR:
      i_in.write((data_ptr[2] << 16) + (data_ptr[1] << 8) + data_ptr[0]);
      break;
    default:
      break;
    }
    break;
  case tlm::TLM_IGNORE_COMMAND:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  default:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  }
  payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
}