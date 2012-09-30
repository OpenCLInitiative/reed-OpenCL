#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include "clUtils.h"

typedef struct{
  int up;
  int down;
  int left;
  int right;
  int val;
}sand;

typedef struct{
  int ping;
  int add;
}container;
const char* SourceFile = "sand.cl";

char BuildLog[10240];
size_t BuildLogSz;

// OpenCL Vars
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
char* pathAndName = NULL;      // var for full paths to data, src, etc.
char* SourceCL = NULL;         // Buffer to hold source for compilation 
cl_event event;
cl_ulong time;

int main(int argc, char **argv)
{
  if(argc != 5){
    printf("Command Line Arguments should be in the form \n <Runs> <Src file> <Print Flag> <Local Size (x)> <Local Size (y)> ");
    printf("\n");
    exit(-1);
  }
  int numComputeUnits;
  int workGroupSize;
  cl_kernel kernel;
  //////
  /// Init Data
  int graph_h, graph_v;
  int* sand_i = file_extractor(argv[2],graph_h,graph_v);
 
  int graph_n = graph_h * graph_v;
  size_t* global =(size_t*)malloc(2*sizeof(size_t));
  global[1] = graph_v;
  size_t* local =(size_t*)malloc(2*sizeof(size_t));
  int lsx = atoi(argv[4]);
  int lsy = atoi(argv[5]);
  local[0] = 1;
  local[1] = lsy;
  global[0]= graph_h/lsx;

  int si = 0;
  int start, end;
  cl_event* event = (cl_event*)malloc(20*sizeof(cl_event));
  int runs = atoi(argv[1]);
  int print = atoi(argv[3]);
  sand* src = (sand*)malloc(graph_n*sizeof(sand));
  container* CellCtrl = (container*)malloc(graph_n*sizeof(container));
  container* RowCtrl = (container*)malloc(graph_v*(graph_h/lsx)*sizeof(container));
  for(int u = 0; u <graph_n;u++){
    src[u].up = 0;
    src[u].down = 0;
    src[u].left = 0;
    src[u].right = 0;
    CellCtrl[u].ping = 0;
    CellCtrl[u].add = 0;
    if(u <= graph_h||u%graph_h ==0 || (u+1)%graph_h ==0 || u>(graph_n-graph_h)){
      src[u].val= -1;

    }
    
    else{
      src[u].val=sand_i[si];
      si++;

    }
    if(u <graph_v*(graph_h/lsx)){
      RowCtrl[u].add = 0;
      RowCtrl[u].ping = 0;
    }
  }
  size_t* wisizes = (size_t*)malloc(3*sizeof(size_t));
  //////// INIT DEVICE
  err1 = clGetPlatformIDs(1, &platform, NULL);
  ERR("platform");
  err1 = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
  ERR("device IDs");
  clGetDeviceInfo(device,CL_DEVICE_MAX_COMPUTE_UNITS,sizeof(numComputeUnits),(void *)&numComputeUnits,NULL);
  printf("Compute units:\t%d\n",(int)numComputeUnits);
  clGetDeviceInfo(device,CL_DEVICE_MAX_WORK_ITEM_SIZES,sizeof(wisizes),(void *)&wisizes,NULL);

  context = clCreateContext(0, 1, &device, NULL, NULL, &err1);
  ERR("context");
  commands = clCreateCommandQueue(context, device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE, &err1);
  ERR("command queue");

   

  //////// INIT DEVICE MEMORY
  devSrc = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(sand) * graph_n, NULL, &err1);
  devRowCtrl = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(container) * graph_v*(graph_h/lsx), NULL, &err2);
  devCellCtrl = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(container)*graph_n,NULL, &err2);
  err1 |= err2;
  ERR("gpu buffers");

  //////// COMPILE AND INSTALL KERNEL CODE
  SourceCL = loadProgSource(cSourceFile, &kernelLength);
  program = clCreateProgramWithSource(context, 1, (const char **)&SourceCL, &kernelLength, &err1);
  ERR("create from source");
  err1 = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

  clGetProgramBuildInfo(program,device,CL_PROGRAM_BUILD_LOG,sizeof(cBuildLog),cBuildLog,&cBuildLogSz);
  printf("--------\n\n");
  printf("Here are the build errors (if any)...\n");
  printf("%s",cBuildLog);
  printf("--------\n\n");


  kernel = clCreateKernel(program, "run", &err1);
  ERR("kernel");


  // Submitting Kernels
  int flag;
  int index = 0;
  for(int j = 0; j<runs; j++){
    
    if(flag){
    err1 = clEnqueueWriteBuffer(commands, devSrc, CL_TRUE, 0, sizeof(sand)*graph_n, src, 1, &event[6],&event[index]);
    ERR("write buffer");
    index++;
    }
    else{err1 = clEnqueueWriteBuffer(commands, devSrc, CL_TRUE, 0, sizeof(sand)*graph_n, src, 0,NULL,&event[index]);
      ERR("write buffer");
      index ++;
    }
    err1 = clEnqueueWriteBuffer(commands, devRowCtrl, CL_TRUE, 0, sizeof(container)* graph_v*(graph_h/lsx), RowCtrl, 1, &event[index-1],&event[index]);
    index++;
      ERR("write buffer");
      err1 = clEnqueueWriteBuffer(commands, devCellCtrl, CL_TRUE, 0, sizeof(container)*graph_n, CellCtrl, 1,&event[index-1],&event[index]);
      index++;
    ERR("write buffer");



    clFinish(commands);

    //    ppm(src,graph_h,graph_v,graph_n,j,argv[0]);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &devSrc);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &devRowCtrl);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &devCellCtrl);
    clSetKernelArg(kernel, 3, sizeof(int),  &graph_h);
    clSetKernelArg(kernel, 4, sizeof(int),  &graph_v);
    clSetKernelArg(kernel, 5, sizeof(int),  &lsy);
    clSetKernelArg(kernel, 6, sizeof(int),  &lsx);
    clSetKernelArg(kernel, 7, sizeof(int), &flag);

    clEnqueueNDRangeKernel(commands,kernel,2,NULL,global,local, 1, &event[index-1],&event[index]);
    index++;
    clFinish(commands);
    clEnqueueReadBuffer(commands, devSrc, CL_TRUE, 0, sizeof(sand)*graph_n, src, 1,&event[index-1],&event[index]);
    index++;
    clEnqueueReadBuffer(commands, devRowCtrl, CL_TRUE, 0, sizeof(container)* graph_v*(graph_h/lsx), RowCtrl, 1,&event[index-1],&event[index]);
    index++;
    clEnqueueReadBuffer(commands, devCellCtrl, CL_TRUE, 0, sizeof(container)*graph_n, CellCtrl, 1,&event[index-1],&event[index]);
    index++;

    clFinish(commands);
    for(int i = 0; i < 7; i++){
    eventprofiler(event[i],&start,&end);
    time += (end-start);
    }
    index = 0;
    flag = 1;

  }
  //////
  printf("Time: %lu\n",(long)time);
   
}

