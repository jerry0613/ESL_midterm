#include <cstdio>
#include <cstdlib>
using namespace std;
#include "Testbench.h"
#include <cmath>
#include <fstream>

Testbench::Testbench(sc_module_name n) : sc_module(n) {
  SC_THREAD(feed_sin);
  sensitive << i_clk.pos();
  dont_initialize();

  SC_THREAD(feed_exp);
  sensitive << i_clk.pos();
  dont_initialize();

  SC_THREAD(feed_log);
  sensitive << i_clk.pos();
  dont_initialize();

  SC_THREAD(fetch_sin_result);
  sensitive << i_clk.pos();
  dont_initialize();

  SC_THREAD(fetch_exp_result);
  sensitive << i_clk.pos();
  dont_initialize();

  SC_THREAD(fetch_log_result);
  sensitive << i_clk.pos();
  dont_initialize();

  SC_THREAD(run_time);
  sensitive << i_clk.pos();
  dont_initialize();
}

Testbench::~Testbench() {
	cout << "Total run time = " << total_run_time << endl;
}

void Testbench::feed_sin() {
  int in, theta, quadrant;

#ifndef NATIVE_SYSTEMC
  o_theta.reset();
#endif
  o_rst_sin.write(false);
	wait(5);
	o_rst_sin.write(true);
	wait(1);

  for (in = 0; in < 360; in++) {
    theta = in;
    // based on cos
    if (theta > 270) {
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

    #ifndef NATIVE_SYSTEMC
			o_theta.put(theta);
    #else
      o_theta.write(theta);
    #endif
  }
}

void Testbench::fetch_sin_result() {
  int in;
  float r_sin, r_cos;
  sc_dt::sc_int<32> tmp;
  ofstream file_sin;
  file_sin.open("output_sin.txt");
#ifndef NATIVE_SYSTEMC
  i_result_sin.reset();
#endif
  wait(5);
	wait(1);
  CORDIC_time = sc_time_stamp();
  for (in = 0; in < 360; in++) {
    #ifndef NATIVE_SYSTEMC
      tmp = i_result_sin.get();
    #else
      tmp = i_result_sin.read();
    #endif
    
    if (in > 270) {
      r_sin = -float(tmp.range(15, 0)) / 16384;
      r_cos = float(tmp.range(31, 16)) / 16384;
    } else if (in > 180) {
      r_sin = -float(tmp.range(15, 0)) / 16384;
      r_cos = -float(tmp.range(31, 16)) / 16384;
    } else if (in > 90) {
      r_sin = float(tmp.range(15, 0)) / 16384;
      r_cos = -float(tmp.range(31, 16)) / 16384;
    } else {
      r_sin = float(tmp.range(15, 0)) / 16384;
      r_cos = float(tmp.range(31, 16)) / 16384;
    }

    file_sin << "cos(" << in << ") = " << r_cos << ", error = " << abs((cos(in*M_PI/180) - r_cos) / cos(in*M_PI/180)) << endl;
    file_sin << "sin(" << in << ") = " << r_sin << ", error = " << abs((sin(in*M_PI/180) - r_sin) / sin(in*M_PI/180)) << endl;
    file_sin << "" << endl;
  }
  cout << "CORDIC run time = " << sc_time_stamp() - CORDIC_time << endl;
  flag++;
}

void Testbench::feed_exp() {
  int x;
  float in;
#ifndef NATIVE_SYSTEMC
  o_in_exp.reset();
#endif
  o_rst_exp.write(false);
	wait(5);
	o_rst_exp.write(true);
	wait(1);
  for (in = 0; in < 10.1; in += 0.001) {
    x = int(in * 4096);
    #ifndef NATIVE_SYSTEMC
			o_in_exp.put(x);
    #else
      o_in_exp.write(x);
    #endif
  }
}

void Testbench::fetch_exp_result() {
  unsigned int y;
  float in, out;
  ofstream file_exp;
  file_exp.open("output_exp.txt");
#ifndef NATIVE_SYSTEMC
  i_result_exp.reset();
#endif
  wait(5);
	wait(1);
  Exp_time = sc_time_stamp();
  for (in = 0; in < 10.1; in += 0.001) {
    #ifndef NATIVE_SYSTEMC
      y = i_result_exp.get();
    #else
      y = i_result_exp.read();
    #endif
    out = float(y) / 512;
    file_exp << "exp(" << in << ") = " << out << ", error = " << abs((exp(in) - out) / out) << endl;
  }
  cout << "Exp run time = " << sc_time_stamp() - Exp_time << endl;
  flag++;
}

void Testbench::feed_log() {
  int x, in;
#ifndef NATIVE_SYSTEMC
  o_in_log.reset();
#endif
  o_rst_log.write(false);
	wait(5);
	o_rst_log.write(true);
	wait(1);
  for (in = 1; in < 22027; in++) {
    x = int(in * 512);
    #ifndef NATIVE_SYSTEMC
			o_in_log.put(x);
    #else
      o_in_log.write(x);
    #endif
  }
}

void Testbench::fetch_log_result() {
  int in, y;
  float out;
  ofstream file_log;
  file_log.open("output_log.txt");
#ifndef NATIVE_SYSTEMC
  i_result_log.reset();
#endif
  wait(5);
	wait(1);
  Log_time = sc_time_stamp();
  for (in = 1; in < 22027; in++) {
    #ifndef NATIVE_SYSTEMC
      y = i_result_log.get();
    #else
      y = i_result_log.read();
    #endif
    out = float(y) / 2048;
    file_log << "ln(" << in << ") = " << out << ", error = " << abs((log(in) - out) / out) << endl;
  }
  cout << "Log run time = " << sc_time_stamp() - Log_time << endl;
  flag++;
}

void Testbench::run_time() {
  wait(5);
	wait(1);
  flag = 0;
  total_start_time = sc_time_stamp();
  while(flag != 3) {
    wait(1);
  }
  total_run_time = sc_time_stamp() - total_start_time;
  sc_stop();
}
