#ifndef TIMING_H
#define TIMING_H

#include <time.h>
#include <stdint.h>

/* Example usage

start = time_tsc_start();
.. // Code under test here
end = time_tsc_end();

double diff = convert_tsc(start, end);
*/

static inline unsigned long time_tsc_start()

{
  register unsigned int cycles_high, cycles_low;

  asm volatile ("CPUID\n\t"
     "RDTSC\n\t"
     "mov %%edx, %0\n\t"
     "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
    "%rax", "%rbx", "%rcx", "%rdx");

  return ( ((uint64_t)cycles_high << 32) | cycles_low );
}

static inline unsigned long time_tsc_end()
{
  register unsigned int cycles_high1, cycles_low1;
  asm volatile("RDTSCP\n\t"
     "mov %%edx, %0\n\t"
     "mov %%eax, %1\n\t"
     "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
    "%rax", "%rbx", "%rcx", "%rdx");
  
  return ( ((uint64_t)cycles_high1 << 32) | cycles_low1 );
}


// TSC rate of 2.711841566767101, taken from CS machine royal-26; change if you need to use a different machine

// Use 'dmsg | grep tsc' and look for 'Refined TSC clocksource calibration' value

// Check if constant_tsc flag is set with 'cat /proc/cpuinfo | grep constant_tsc'

static inline long double convert_tsc(unsigned long start, unsigned long end)

{

  return (end - start) / 2.711841566767101;

}



static inline long double time_monotonic()

{

    struct timespec time;

    clock_gettime(CLOCK_MONOTONIC, &time);

    return (time.tv_sec * 1000000000.0L ) + time.tv_nsec;

}

#endif
