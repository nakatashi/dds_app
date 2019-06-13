#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include "rp_dds.h"
// gcc -o main.out main.c -lrp_dds
// clock_gettime() is recommended.

void swing(RP_DDS * rp_dds, double freq0_cent, double freq1_cent, double swing_freq, double swing_range_mhz, int ch);

int interrupted = 0;
void signal_handler(int sig)
{
  interrupted = 1;
}
// -- ch range freq
int main(int argc, char *argv[])
{
  RP_DDS rp_dds;
  int pWidth = (1 << PHASE_WIDTH);
  int result = rp_dds_open(&rp_dds, DEFAULT_ADDR, DEFAULT_SYSCLK_MHz);
  double f0_cent, f1_cent;

  int ch = (int) atoi(argv[1]);
  double swing_range = (double) atof(argv[2]);
  double swing_freq = (double) atof(argv[3]);
  
  f0_cent = rp_dds.pin0 * ((double) rp_dds.sys_clk_mhz/ pWidth);
  f1_cent = rp_dds.pin1 * ((double) rp_dds.sys_clk_mhz/ pWidth);
  
  swing(&rp_dds, f0_cent, f1_cent, swing_freq, swing_range, ch);
  
  result |= rp_dds_close(&rp_dds);
  return result;
}

void swing(RP_DDS * rp_dds, double freq0_cent, double freq1_cent, double swing_freq, double swing_range_mhz, int ch)
{  
  suseconds_t time_usec = 0;
  //double swing_freq = 0.1;
  double freq_offs = 0;
  //double swing_range_mhz = 5;
  
  // subscribe signal_handler  
  signal(SIGINT, signal_handler);
  
  struct timeval t1, t2;

  while(!interrupted)
	{
	  gettimeofday(&t1, NULL);

	  time_usec += (t1.tv_usec - t2.tv_usec);

	  gettimeofday(&t2, NULL);
	  
	  freq_offs = swing_range_mhz * sin(2 * M_PI * swing_freq * 10E-6 * time_usec);

	  if(ch == 0)
		{
		  rp_dds_set_freq(rp_dds, freq0_cent + freq_offs, freq1_cent);
		} else 
		{
		  rp_dds_set_freq(rp_dds, freq0_cent, freq1_cent + freq_offs);
		}
	}
  rp_dds_set_freq(rp_dds, freq0_cent, freq1_cent);
}

