#include <CL\cl.h>
#include <stdio.h>
#include <stdlib.h>

const char* errorString(cl_int error)
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

void err(const char *msg, int code, int line) {
  if (code) {
    printf("Error %s(%d) in line %d: %s\n", errorString(code), code, line, msg);
    exit(0);
  }
}

#define ERR(m) err((m),ciErr,__LINE__)

char* loadProgSource(const char* filename, size_t* length)
{
  FILE* f = NULL;
  int len;

  // open the OpenCL source code file                                                                                                                                                 
#ifdef _WIN32   // Windows version                                                                                                                                                  
  if(fopen_s(&f, filename, "rb") != 0)
    {
      return NULL;
    }
#else           // Linux version                                                                                                                                                    
  f = fopen(filename, "rb");
  if(f == 0)
    {
      return NULL;
    }
    #endif

  // get the length of the source code                                                                                                                                                
  fseek(f, 0, SEEK_END);
  len = ftell(f);
  fseek(f, 0, SEEK_SET);

  // allocate a buffer for the source code string and read it in              
  char* src = (char *)malloc(len + 1);
  if (fread(src, len, 1, f) != 1) {
    fclose(f);
    free(src);
    return NULL;
  } else {
    fclose(f);
    if (length != NULL) *length = len;
    src[len] = '\0';
    return src;
  }
}


int main(int argc, char** argv){

  cl_kernel kernel;
  cl_context context;
  cl_command_queue commands;
  cl_device_id device;
  cl_program program;
  cl_mem devSrc;
  cl_mem devDst;
  int error;

  error = clGetPlatformIDs(1, &platform
  

