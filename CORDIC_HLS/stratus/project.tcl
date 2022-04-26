#*******************************************************************************
# Copyright 2015 Cadence Design Systems, Inc.
# All Rights Reserved.
#
#*******************************************************************************
#
# Stratus Project File
#
############################################################
# Project Parameters
############################################################
#
# Technology Libraries
#
set LIB_PATH "[get_install_path]/share/stratus/techlibs/GPDK045/gsclib045_svt_v4.4/gsclib045/timing"
set LIB_LEAF "slow_vdd1v2_basicCells.lib"
use_tech_lib    "$LIB_PATH/$LIB_LEAF"

#
# Global synthesis attributes.
#
set_attr clock_period           6.0
set_attr message_detail         3 
#set_attr default_input_delay    0.1
#set_attr sched_aggressive_1 on
#set_attr unroll_loops on
#set_attr flatten_arrays all 
#set_attr timing_aggression 0
#set_attr default_protocol true

#
# Simulation Options
#
### 1. You may add your own options for C++ compilation here.
set_attr cc_options             "-DCLOCK_PERIOD=6.0 -g"
#enable_waveform_logging -vcd
#set_attr end_of_sim_command "make saySimPassed"

#
# Testbench or System Level Modules
#
### 2. Add your testbench source files here.
define_system_module ../main.cpp
define_system_module ../Testbench.cpp
define_system_module ../System.cpp

#
# SC_MODULEs to be synthesized
#
### 3. Add your design source files here (to be high-level synthesized).
define_hls_module CORDIC ../cordic.cpp
define_hls_module Exp ../Exp.cpp
define_hls_module Log ../Log.cpp

### 4. Define your HLS configuration (arbitrary names, BASIC and DPA in this example). 
define_hls_config CORDIC BASIC1
define_hls_config Exp BASIC2
define_hls_config Log BASIC3
define_hls_config CORDIC DPA1       --dpopt_auto=op,expr
define_hls_config Exp DPA2       --dpopt_auto=op,expr
define_hls_config Log DPA3       --dpopt_auto=op,expr

### 5. Define simulation configuration for each HLS configuration
### 5.1 The behavioral simulation (C++ only).
define_sim_config B
### 5.2 The Verilog simulation for HLS config "BASIC". 
define_sim_config V_BASIC {CORDIC RTL_V BASIC1} {Exp RTL_V BASIC2} {Log RTL_V BASIC3}
### 5.3 The Verilog simulation for HLS config "DPA". 
define_sim_config V_DPA {CORDIC RTL_V DPA1} {Exp RTL_V DPA2} {Log RTL_V DPA3}
 