#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include "rp_dds.h"

int main(void)
{
  RP_DDS rp_dds;
  int pWidth = (1 << PHASE_WIDTH);
  int result = rp_dds_open(&rp_dds, DEFAULT_ADDR, DEFAULT_SYSCLK_MHz);
  
  double ampl0, ampl1, freq0, freq1;

  ampl0 = (double) rp_dds.asf0 / (1 << 9);
  ampl1 = (double) rp_dds.asf1 / (1 << 9);

  freq0 = (double) rp_dds.pin0 * rp_dds.sys_clk_mhz / pWidth;
  freq1 = (double) rp_dds.pin1 * rp_dds.sys_clk_mhz / pWidth;
  
  printf("system clock : %d [MHz]\r\n", rp_dds.sys_clk_mhz);  
  printf("ch \t freq [MHz]\t ampl \r\n");
  printf("-------------------------\r\n");
  printf("0 \t %3.3f \t %.3f \r\n", freq0, ampl0);
  printf("1 \t %3.3f \t %.3f \r\n", freq1, ampl1);
  
  
  result |= rp_dds_close(&rp_dds);
  return result;
}
