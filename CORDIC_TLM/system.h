#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <systemc>
using namespace sc_dt;

const int CORDIC_T_ADDR = 0x00000000;
const int CORDIC_XY_ADDR = 0x00000004;
const int CORDIC_CHECK_ADDR = 0x00000008;

const int EXP_X_ADDR = 0x00000000;
const int EXP_Y_ADDR = 0x00000004;
const int EXP_CHECK_ADDR = 0x00000008;

const int LOG_X_ADDR = 0x00000000;
const int LOG_Y_ADDR = 0x00000004;
const int LOG_CHECK_ADDR = 0x00000008;

union package {
  unsigned char uc[4];
};
#endif