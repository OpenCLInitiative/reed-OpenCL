#ifndef _REED_GPU_H
#define _REED_GPU_H

#include <CL/cl.h>
#include <stdlib.h>


#define RG_MAX_NUMBER_BUFFERS 100
#define RG_MAX_NUMBER_EXECS 100

#define REEDGPUBUFFERIN    0
#define REEDGPUBUFFEROUT   1
#define REEDGPUBUFFERINOUT 2


typedef int rGXid;
typedef int rGmem;



typedef struct _reedGPUbuffer {
  int type;
  size_t size;
  cl_mem deviceVersion;
  void *hostVersion;
  int event;
} rGbuffer;
  
typedef struct _reedGPU {

  // GPU device execution context
  cl_context context;        // OpenCL context
  cl_command_queue queue;    // OpenCL command que
  cl_platform_id platform;   // OpenCL platform
  cl_device_id device;       // OpenCL device
  cl_program program;        // OpenCL program
  size_t units;              // number of compute units
  size_t maxGroupSize;      // largest allowable work group size 

  // source code and build log
  char *src;
  size_t srclen;

  // minimal kernel support 
  int arg;
  cl_kernel kernel;

  // events
  int event;
  cl_event events[RG_MAX_NUMBER_EXECS + RG_MAX_NUMBER_BUFFERS];

  // registered buffers for input and output data
  int buffer;
  rGbuffer buffers[RG_MAX_NUMBER_BUFFERS];
  
} rGcontext;

void rGinit(rGcontext *);
void rGload(rGcontext *, const char *);
void *rGmalloc(rGcontext *, const char *, size_t, rGmem *);
void rGinitX(rGcontext *);
void rGnewX(rGcontext *, const char *);
void rGargXmem(rGcontext *, rGmem);
void rGargXint(rGcontext *, int);
rGXid rGsubmitX(rGcontext *, int, int);
void rGwaitallX(rGcontext *);
unsigned long rGgettimeX(rGcontext *, rGXid);
unsigned long rGgetbeginX(rGcontext *, rGXid);
unsigned long rGgetendX(rGcontext *, rGXid);

#endif
