#include <CL/cl.h>
#include <reedGPU.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buildlog[1024];
int buildloglen;

cl_int rgErr;

#define RGPUERR(m) _rGerr((m),rgErr,__LINE__)

const char* _rG_errorCLstring(cl_int error)
{
    static const char* errorString[] = {
        "CL_SUCCESS",
        "CL_DEVICE_NOT_FOUND",
        "CL_DEVICE_NOT_AVAILABLE",
        "CL_COMPILER_NOT_AVAILABLE",
        "CL_MEM_OBJECT_ALLOCATION_FAILURE",
        "CL_OUT_OF_RESOURCES",
        "CL_OUT_OF_HOST_MEMORY",
        "CL_PROFILING_INFO_NOT_AVAILABLE",
        "CL_MEM_COPY_OVERLAP",
        "CL_IMAGE_FORMAT_MISMATCH",
        "CL_IMAGE_FORMAT_NOT_SUPPORTED",
        "CL_BUILD_PROGRAM_FAILURE",
        "CL_MAP_FAILURE",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "CL_INVALID_VALUE",
        "CL_INVALID_DEVICE_TYPE",
        "CL_INVALID_PLATFORM",
        "CL_INVALID_DEVICE",
        "CL_INVALID_CONTEXT",
        "CL_INVALID_QUEUE_PROPERTIES",
        "CL_INVALID_COMMAND_QUEUE",
        "CL_INVALID_HOST_PTR",
        "CL_INVALID_MEM_OBJECT",
        "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
        "CL_INVALID_IMAGE_SIZE",
        "CL_INVALID_SAMPLER",
        "CL_INVALID_BINARY",
        "CL_INVALID_BUILD_OPTIONS",
        "CL_INVALID_PROGRAM",
        "CL_INVALID_PROGRAM_EXECUTABLE",
        "CL_INVALID_KERNEL_NAME",
        "CL_INVALID_KERNEL_DEFINITION",
        "CL_INVALID_KERNEL",
        "CL_INVALID_ARG_INDEX",
        "CL_INVALID_ARG_VALUE",
        "CL_INVALID_ARG_SIZE",
        "CL_INVALID_KERNEL_ARGS",
        "CL_INVALID_WORK_DIMENSION",
        "CL_INVALID_WORK_GROUP_SIZE",
        "CL_INVALID_WORK_ITEM_SIZE",
        "CL_INVALID_GLOBAL_OFFSET",
        "CL_INVALID_EVENT_WAIT_LIST",
        "CL_INVALID_EVENT",
        "CL_INVALID_OPERATION",
        "CL_INVALID_GL_OBJECT",
        "CL_INVALID_BUFFER_SIZE",
        "CL_INVALID_MIP_LEVEL",
        "CL_INVALID_GLOBAL_WORK_SIZE",
    };
    const int errorCount = sizeof(errorString) / sizeof(errorString[0]);
    const int index = -error;
    return (index >= 0 && index < errorCount) ? errorString[index] : "Unspecified Error";
}

void _rGerr(const char *msg, int code, int line) {
  if (code) {
    printf("Error %s(%d) in line %d: %s\n", _rG_errorCLstring(code), code, line, msg);
    exit(0);
  }
}

void rGload(rGcontext *rG, const char* filename) {
    FILE* f = NULL;
    int len;

    f = fopen(filename, "rb");
    if (f == NULL) {
      RGPUERR("Could not open source file"); 
    }

    // get the length of the source code
    fseek(f, 0, SEEK_END); 
    len = ftell(f);
    fseek(f, 0, SEEK_SET); 

    // allocate a buffer for the source code string and read it in
    rG->src = (char *)malloc(len + 1); 
    rG->srclen = len;
    if (fread(rG->src, len, 1, f) != 1) {
      fclose(f);
      RGPUERR("Error while processing source file"); 
    } 
    fclose(f);
    rG->src[len] = '\0';
    rG->program = clCreateProgramWithSource(rG->context, 1, (const char **)&rG->src, &rG->srclen, &rgErr);
    RGPUERR("create from source");
    rgErr = clBuildProgram(rG->program, 0, NULL, NULL, NULL, NULL);
    clGetProgramBuildInfo(rG->program,rG->device,CL_PROGRAM_BUILD_LOG,sizeof(buildlog),buildlog,NULL);
    printf("%s",buildlog);
    RGPUERR("build");
}

void rGinit(rGcontext *rG) {
  //////// INIT DEVICE
  rgErr = clGetPlatformIDs(1, &rG->platform, NULL);
  RGPUERR("platform");
  rgErr = clGetDeviceIDs(rG->platform, CL_DEVICE_TYPE_GPU, 1, &rG->device, NULL);
  RGPUERR("device IDs");
  clGetDeviceInfo(rG->device,CL_DEVICE_MAX_COMPUTE_UNITS,sizeof(rG->units),(void *)&rG->units,NULL);
  clGetDeviceInfo(rG->device,CL_DEVICE_MAX_WORK_GROUP_SIZE,sizeof(rG->maxGroupSize),(void *)&rG->maxGroupSize,NULL);
  rG->context = clCreateContext(0, 1, &rG->device, NULL, NULL, &rgErr);
  RGPUERR("context");
  rG->queue = clCreateCommandQueue(rG->context, rG->device, 
				   CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE, 
				   &rgErr);
  RGPUERR("command queue");
  rG->event = 0;
  rG->buffer = 0;
}

void *rGmalloc(rGcontext *rG, const char *flags, size_t size, rGmem *id) {

  void *hbuffer;
  cl_mem dbuffer;

  int b = rG->buffer;
  if (strcmp(flags,"I") == 0) {
    hbuffer = (void *)malloc(size);
    dbuffer = clCreateBuffer(rG->context, CL_MEM_READ_ONLY, size, NULL, &rgErr);
    rG->buffers[b].type = REEDGPUBUFFERIN;
  } else if (strcmp(flags,"O") == 0) {
    hbuffer = (void *)malloc(size);
    dbuffer = clCreateBuffer(rG->context, CL_MEM_WRITE_ONLY, size, NULL, &rgErr);
    rG->buffers[b].type = REEDGPUBUFFEROUT;
  } else {
    return NULL;
  }

  rG->buffers[b].size = size;
  rG->buffers[b].deviceVersion = dbuffer;
  rG->buffers[b].hostVersion = hbuffer;
  *id = rG->buffer;

  rG->buffer++;

  return hbuffer;
}

void rGnewX(rGcontext *rG, const char *function) {
  rG->arg = 0;
  rG->kernel = clCreateKernel(rG->program,function,NULL);
}

void rGargXint(rGcontext *rG, int arg) {
  clSetKernelArg(rG->kernel, rG->arg, sizeof(cl_int), &arg);
  rG->arg++;
}

void rGargXmem(rGcontext *rG, rGmem buff) {
  clSetKernelArg(rG->kernel, rG->arg, sizeof(cl_mem), &(rG->buffers[buff].deviceVersion));
  rG->arg++;
}

rGXid rGsubmitX(rGcontext *rG, int procs, int gpsz) {
  size_t gSize = (size_t)procs;
  size_t lSize = (size_t)gpsz;
  
  if (rG->event == 0) {
    clEnqueueNDRangeKernel(rG->queue, rG->kernel, 
			   1, NULL, &gSize, &lSize, 
			   0, NULL, 
			   &rG->events[rG->event]);
  } else {
    clEnqueueNDRangeKernel(rG->queue, rG->kernel, 
			   1, NULL, &gSize, &lSize, 
			   1, &rG->events[rG->event-1], 
			   &rG->events[rG->event]);
  }
  int id = rG->event;
  rG->event++;
  return id;
}

rGXid rGsubmitX2(rGcontext *rG, int procs) {
  size_t gSize = (size_t)procs;

  if (rG->event == 0) {
    clEnqueueNDRangeKernel(rG->queue, rG->kernel,
                           1, NULL, &gSize, NULL,
                           0, NULL,
                           &rG->events[rG->event]);
  } else {
    clEnqueueNDRangeKernel(rG->queue, rG->kernel,
                           1, NULL, &gSize, NULL,
                           1, &rG->events[rG->event-1],
                           &rG->events[rG->event]);
  }
  int id = rG->event;
  rG->event++;
  return id;
}

void rGinitX(rGcontext *rG) {
  for (int b=0; b<rG->buffer; b++) {
    if (rG->buffers[b].type == REEDGPUBUFFERIN) {
      if (rG->event == 0) {
	printf("a\n");
	clEnqueueWriteBuffer(rG->queue,
			     rG->buffers[b].deviceVersion, CL_FALSE, 0, rG->buffers[b].size,
			     rG->buffers[b].hostVersion,
			     0, NULL, &rG->events[rG->event]);
      } else {
	printf("b\n");
	clEnqueueWriteBuffer(rG->queue,
			     rG->buffers[b].deviceVersion, CL_FALSE, 0, rG->buffers[b].size,
			     rG->buffers[b].hostVersion,
			     1, &rG->events[rG->event-1], &rG->events[rG->event]);
      } 
      printf("c\n");
      rG->buffers[b].event = rG->event;
      rG->event++;
    }
  }
}

void rGwaitallX(rGcontext *rG) {
 for (int b=0; b<rG->buffer; b++) {
    if (rG->buffers[b].type == REEDGPUBUFFEROUT) {
      clEnqueueReadBuffer(rG->queue,
			  rG->buffers[b].deviceVersion, CL_FALSE, 0, rG->buffers[b].size,
			  rG->buffers[b].hostVersion,
			  0, NULL, &rG->events[rG->event]);
    } else {
      clEnqueueReadBuffer(rG->queue,
			  rG->buffers[b].deviceVersion, CL_FALSE, 0, rG->buffers[b].size,
			  rG->buffers[b].hostVersion,
			  1, &rG->events[rG->event-1], &rG->events[rG->event]);
    } 
    rG->buffers[b].event = rG->event;
    rG->event++;
 }
 clWaitForEvents(1,&rG->events[rG->event-1]);
 rG->event = 0;
}

unsigned long rGgettimeX(rGcontext *rG, rGXid x) {
  cl_ulong start;
  cl_ulong end;
  clGetEventProfilingInfo (rG->events[x], CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
  clGetEventProfilingInfo (rG->events[x], CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
  return (unsigned long) (end - start);
}

unsigned long rGgetbeginX(rGcontext *rG, rGXid x) {
  cl_ulong start;
  clGetEventProfilingInfo (rG->events[x], CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
  return (unsigned long) start;
}

unsigned long rGgetendX(rGcontext *rG, rGXid x) {
  cl_ulong end;
  clGetEventProfilingInfo (rG->events[x], CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
  return (unsigned long) end;
}
