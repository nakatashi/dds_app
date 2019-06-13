#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include "rp_dds.h"

int main(int argc, char *argv[])
{
  RP_DDS rp_dds;
  int pWidth = (1 << PHASE_WIDTH);
  int result = rp_dds_open(&rp_dds, DEFAULT_ADDR, DEFAULT_SYSCLK_MHz);
  int ch = atoi(argv[1]);
  double freq = (double) atof(argv[2]);
  double ampl = (double) atof(argv[3]);
  
  uint32_t pin0, pin1;
  uint16_t asf0, asf1;

  uint32_t pin_in = (uint32_t) (freq * pWidth / (double) rp_dds.sys_clk_mhz);
  uint32_t asf_in = (uint16_t) (ampl * (1 << 9)) & 0x3FF;
  
  if(ch == 0)
	{
	  pin0 = pin_in;
	  pin1 = rp_dds.pin1;
	  asf0 = asf_in;
	  asf1 = rp_dds.asf1;
	} else 
	{
	  pin1 = pin_in;
	  pin0 = rp_dds.pin0;
	  asf1 = asf_in;
	  asf0 = rp_dds.asf0;
	}
  
  rp_dds_set_freq_bin(&rp_dds, pin0, pin1);
  rp_dds_set_ampl_bin(&rp_dds, asf0, asf1);
  
  result |= rp_dds_close(&rp_dds);
  return result;
}
