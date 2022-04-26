#include "System.h"
System::System( sc_module_name n): sc_module( n ), tb("tb"), CORDIC("CORDIC"), Exp("Exp"), Log("Log"), 
		clk("clk", CLOCK_PERIOD, SC_NS), rst_sin("rst_sin"), rst_exp("rst_exp"), rst_log("rst_log")
{
	tb.i_clk(clk);
	tb.o_rst_sin(rst_sin);
	tb.o_rst_exp(rst_exp);
	tb.o_rst_log(rst_log);
	tb.i_result_sin(result_sin);
	//tb.i_result_cos(result_cos);
	tb.i_result_exp(result_exp);
	tb.i_result_log(result_log);
	tb.o_theta(theta);
	tb.o_in_exp(in_exp);
	tb.o_in_log(in_log);
	CORDIC.i_clk(clk);
	CORDIC.i_rst(rst_sin);
	CORDIC.i_theta(theta);
	CORDIC.o_result_sin(result_sin);
	//CORDIC.o_result_cos(result_cos);
	Exp.i_clk(clk);
	Exp.i_rst(rst_exp);
	Exp.i_in_exp(in_exp);
	Exp.o_result_exp(result_exp);
	Log.i_clk(clk);
	Log.i_rst(rst_log);
	Log.i_in_log(in_log);
	Log.o_result_log(result_log);
}

System::~System() {
}
