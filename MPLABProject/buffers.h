#ifndef BUFFERS_H
#define BUFFERS_H

#include <stdint.h>

#define APPROX_FS 9615
#define N_PER_PERIOD (APPROX_FS / 60)
#define APPROX_NO_PERIODS 3
#define BUFFER_SIZE (APPROX_NO_PERIODS * N_PER_PERIOD)



float stdev(); 
int computeMAX(int num_ms);
void config_timer();
void begin_conv();
void stop_conv();
void swapBuffers(void);

#endif