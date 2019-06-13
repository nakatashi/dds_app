#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <math.h>
#include "rp_dds.h"
// gcc -o main.out main.c -lrp_dds

int interrupted = 0;
void signal_handler(int sig)
{
  interrupted = 1;
}
  /*
  int pWidth = (1 << PHASE_WIDTH);
  double freq0_mhz = rp_dds.pin0 * ((double) rp_dds.sys_clk_mhz/ pWidth);
  double ampl0 = ((int16_t) rp_dds.asf0) / ((double) (1 << 9));

  printf("Freq for ch0 %3.3f [MHz]\r\n", freq0_mhz);
  printf("Ampl for ch0 %3.3f\r\n", ampl0);
  */

int main(void)
{
  
  RP_DDS rp_dds;
  int result = rp_dds_open(&rp_dds, DEFAULT_ADDR, DEFAULT_SYSCLK_MHz);

  suseconds_t time_usec = 0;
  double swing_freq = 0.1;
  double freq_offs = 0;
  double freq_range = 5;
  

  if(result == EXIT_FAILURE)
	{
	  printf("failed to load device.");
	  return EXIT_FAILURE;
	}
  
  rp_dds_set_ampl(&rp_dds, 0.5, 0.5);
  rp_dds_set_freq(&rp_dds, 25, 25);

  // subscribe signal_handler  
  signal(SIGINT, signal_handler);
  int pWidth = (1 << PHASE_WIDTH);
  double freq0_mhz;

  struct timeval t1, t2;

  while(!interrupted)
	{
	  gettimeofday(&t1, NULL);

	  time_usec += (t1.tv_usec - t2.tv_usec);

	  gettimeofday(&t2, NULL);
	  
	  freq_offs = freq_range * sin(2 * M_PI * swing_freq * 10E-6 * time_usec);
	  rp_dds_set_freq(&rp_dds, 25.0 + freq_offs, 25);
	  freq0_mhz = rp_dds.pin0 * ((double) rp_dds.sys_clk_mhz/ pWidth);
	  //printf("%3.3f, %3.8f\r\n", freq0_mhz, time_usec * 10E-6);
	}

  rp_dds_set_ampl(&rp_dds, 0.5, 0.5);
  rp_dds_set_freq(&rp_dds, 25, 25);
  
  result |= rp_dds_close(&rp_dds);  
  return result;
}

