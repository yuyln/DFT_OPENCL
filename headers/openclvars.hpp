#ifndef __OPENCLVARS
#define __OPENCLVARS
#include <functions.hpp>
#include <complex.hpp>

extern cl_program program;
extern cl_context context;
extern cl_command_queue queue;
extern cl_device_type devicetype;
extern cl_device_id device;
extern cl_platform_id plat[2];
extern size_t totalWork[1];
extern size_t workPerGroup[1];
extern int index_;

extern cl_kernel kDFT;

char* readKernel(const char*);
void initOpenCL(int);

template <typename T>
cl_mem* createBuffer(T*, size_t, cl_mem_flags);

template <typename T>
void writeBuffer(T*, cl_mem*, size_t);

template <typename T>
void readBuffer(T*, cl_mem*, size_t);

void setDFTXParam(Complex*, cl_mem*, size_t);
void setDFTXParam(cl_mem*);
void setDFTxParam(Complex*, cl_mem*, size_t);
void setDFTxParam(cl_mem*);
void setDFTNParam(int);

void enqueueND(cl_kernel);
void finish();
#endif