using namespace std;
#include "Testbench.h"
#include <iomanip>
#include <cmath>
#include <fstream>

Testbench::Testbench(sc_module_name n)
    : sc_module(n), initiator("initiator") {
  SC_THREAD(compute_sin);
  SC_THREAD(compute_exp);
  SC_THREAD(compute_log);
  SC_THREAD(run_time);
}

Testbench::~Testbench() {}

void Testbench::compute_sin() {
  int in, theta;
  package data;
  unsigned char mask[4];
  float r_sin, r_cos;
  int quadrant = 1;
  bool done = false;
  int output_num = 0;
  sc_int<16> tmp_x, tmp_y;
  ofstream file;
  file.open("output_sin.txt");

  for (in = 0; in < 360; in++) {
    theta = in;
    // based on cos
    if (theta < 0 || theta > 360) {
      cout << "It isn't a valid angle." << endl;
      sc_stop();
    } else if (theta > 270) {
      quadrant = 4;
      theta = 360 - theta;
    } else if (theta > 180) {
      quadrant = 3;
      theta -= 180;
    } else if (theta > 90) {
      quadrant = 2;
      theta = 180 - theta;
    } else
      quadrant = 1;

    data.uc[0] = theta;
    mask[0] = 0xff;
    mask[1] = 0xff;
    mask[2] = 0xff;
    mask[3] = 0xff;
    initiator.write_to_socket(CORDIC_MM_BASE + CORDIC_T_ADDR, mask, data.uc, 4);
    wait(1 * CLOCK_PERIOD, SC_NS);

    while(!done){
      initiator.read_from_socket(CORDIC_MM_BASE + CORDIC_CHECK_ADDR, mask, data.uc, 4);
      output_num = data.uc[0] & data.uc[1];
      if(output_num > 0) done = true;
    }
    initiator.read_from_socket(CORDIC_MM_BASE + CORDIC_XY_ADDR, mask, data.uc, 4);
    tmp_x = (data.uc[1] << 8) + data.uc[0];
    tmp_y = (data.uc[3] << 8) + data.uc[2];
    
    if (quadrant == 4) {
      r_sin = -float(tmp_y) / 16384;
      r_cos = float(tmp_x) / 16384;
    } else if (quadrant == 3) {
      r_sin = -float(tmp_y) / 16384;
      r_cos = -float(tmp_x) / 16384;
    } else if (quadrant == 2) {
      r_sin = float(tmp_y) / 16384;
      r_cos = -float(tmp_x) / 16384;
    } else {
      r_sin = float(tmp_y) / 16384;
      r_cos = float(tmp_x) / 16384;
    }
    
    file << "cos(" << in << ") = " << r_cos << ", error = " << abs((cos(in*M_PI/180) - r_cos) / cos(in*M_PI/180)) << endl;
    file << "sin(" << in << ") = " << r_sin << ", error = " << abs((sin(in*M_PI/180) - r_sin) / sin(in*M_PI/180)) << endl;
    file << "" << endl;
  }
  
  flag++;
}

void Testbench::compute_exp() {
  int x;
  unsigned int y;
  float in, out;
  package data;
  unsigned char mask[4];
  bool done = false;
  ofstream file;
  file.open("output_exp.txt");

  for (in = 0; in < 10.1; in += 0.001) {
    x = int(in * 4096);
    mask[0] = 0xff;
    mask[1] = 0xff;
    mask[2] = 0xff;
    mask[3] = 0xff;
    initiator.write_to_socket(EXP_MM_BASE + EXP_X_ADDR, mask, (unsigned char*)&x, 4);
    wait(1 * CLOCK_PERIOD, SC_NS);

    while(!done){
      initiator.read_from_socket(EXP_MM_BASE + EXP_CHECK_ADDR, mask, data.uc, 4);
      if(data.uc[0] > 0) done = true;
    }

    initiator.read_from_socket(EXP_MM_BASE + EXP_Y_ADDR, mask, data.uc, 4);
    y = (data.uc[2] << 16) + (data.uc[1] << 8) + data.uc[0];
    out = float(y) / 512;
    file << "exp(" << in << ") = " << out << ", error = " << abs((exp(in) - out) / out) << endl;
  }
  flag++;
}

void Testbench::compute_log() {
  int x, y;
  float in, out;
  package data;
  unsigned char mask[4];
  bool done = false;
  ofstream file;
  file.open("output_log.txt");

  for (in = 1; in < 22027; in++) {
    x = int(in * 512);
    mask[0] = 0xff;
    mask[1] = 0xff;
    mask[2] = 0xff;
    mask[3] = 0xff;
    initiator.write_to_socket(LOG_MM_BASE + LOG_X_ADDR, mask, (unsigned char*)&x, 4);
    wait(1 * CLOCK_PERIOD, SC_NS);

    while(!done){
      initiator.read_from_socket(LOG_MM_BASE + LOG_CHECK_ADDR, mask, data.uc, 4);
      if(data.uc[0] > 0) done = true;
    }

    initiator.read_from_socket(LOG_MM_BASE + LOG_Y_ADDR, mask, data.uc, 4);
    y = (data.uc[1] << 8) + data.uc[0];
    out = float(y) / 2048;
    file << "ln(" << in << ") = " << out << ", error = " << abs((log(in) - out) / out) << endl;
  }
  flag++;
}

void Testbench::run_time() {
  wait(5);
	wait(1);
  flag = 0;
  while(flag != 3) {
    wait(1);
  }
  sc_stop();
}