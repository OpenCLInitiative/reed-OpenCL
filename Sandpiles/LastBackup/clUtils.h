#ifndef _CL_UTILS_H
#define _CL_UTILS_H

#include <CL/cl.h>
#include <stdlib.h>

const char* CLErrorString(cl_int error);
void err(const char *msg, int code, int line);
void eventprofiler(cl_event event, int* s, int* e);
int* file_extractor(char *file_name, int* h, int* v);
struct clVars;

#endif
