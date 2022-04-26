#include <iomanip>
#include "cordic.h"

CORDIC::CORDIC(sc_module_name n): sc_module(n), t_skt("t_skt"), base_offset(0) {
  SC_THREAD(do_CORDIC);

  t_skt.register_b_transport(this, &CORDIC::blocking_transport);
}

CORDIC::~CORDIC() = default;

void CORDIC::do_CORDIC() {

  while(true) {
    theta = (i_theta.read() - 45) << 8;
    y = X0;
    x = X0;

    for (int i = 0; i < 10; i++) {
      if (theta < 0) {
        theta = theta + table[i];
        reg_x = x;
        x = x + (y >> (i+1));
        y = y - (reg_x >> (i+1)); 
      }
      else {
        theta = theta - table[i];
        reg_x = x;
        x = x - (y >> (i+1));
        y = y + (reg_x >> (i+1)); 
      }
    }

    o_sin.write(y);
    o_cos.write(x);
  }
}

void CORDIC::blocking_transport(tlm::tlm_generic_payload &payload,
                                     sc_core::sc_time &delay) {
  sc_dt::uint64 addr = payload.get_address();
  addr -= base_offset;
  unsigned char *mask_ptr = payload.get_byte_enable_ptr();
  unsigned char *data_ptr = payload.get_data_ptr();
  package buffer;
  sc_int<16> tmp_x, tmp_y;
  switch (payload.get_command()) {
  case tlm::TLM_READ_COMMAND:
    switch (addr) {
    case CORDIC_XY_ADDR:
      tmp_y = o_sin.read();
      tmp_x = o_cos.read();
      buffer.uc[0] = tmp_x.range(7, 0);
      buffer.uc[1] = tmp_x.range(15, 8);
      buffer.uc[2] = tmp_y.range(7, 0);
      buffer.uc[3] = tmp_y.range(15, 8);
      break;
    case CORDIC_CHECK_ADDR:
      buffer.uc[0] = o_sin.num_available();
      buffer.uc[1] = o_cos.num_available();
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
    case CORDIC_T_ADDR:
      i_theta.write(data_ptr[0]);
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