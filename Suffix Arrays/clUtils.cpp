#include <CL/cl.h>
#include <clUltis.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cl_int Err;

#define ERR(m) cluErr((m),Err,__LINE__)

// Outputs more detailed error messages
const char* cluErrorString(cl_int error)
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

void cluErr(const char *msg, int code, int line) {
  if (code) {
    printf("Error %s(%d) in line %d: %s\n", _cluErrorString(code), code, line, msg);
    exit(0);
  }
}

// Gives the start and end times of some event
void cluEventProfiler(cl_event event, int* s, int* e){
    clGetEventProfilingInfo (event, CL_PROFILING_COMMAND_START,
                             sizeof(cl_ulong),
			     s,
                             NULL);
    clGetEventProfilingInfo (event, CL_PROFILING_COMMAND_END,
                             sizeof(cl_ulong),
                             e,
                             NULL);
}

// Reads the contents of a .cl source file into a buffer to be compiled by the OpenCL API
int* cluFileExtractor(char *file_name){
  FILE *file;
  int sz = (int)fsize(file_name);
  int* s = (int*)malloc(sz*sizeof(int));
  file = fopen(file_name, "r");
  int z = 0;
  int x;  
  while(fscanf(file,"%d",&x) != EOF){
    s[z]=x;
    z++;
    
  }
  fclose(file);
  return(s);

}

struct cluVars{
cl_context context;        // OpenCL context
cl_command_queue commands;// OpenCL command que
cl_platform_id platform;      // OpenCL platform
cl_device_id device;          // OpenCL device
cl_program program;           // OpenCL program
cl_kernel kernel;             // OpenCL kernel
cl_mem devSrc;               // OpenCL device source buffer A
cl_mem devRowCtrl;
cl_mem devCellCtrl;
size_t kernelLength;			// Byte size of kernel code
cl_int err1, err2;			// Error code var
char* pathAndName;      // var for full paths to data, src, etc.
char* SourceCL;         // Buffer to hold source for compilation 
cl_event event;
cl_ulong time;
}