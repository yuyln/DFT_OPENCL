#ifndef __FUNCSTPP
#define __FUNCSTPP
#include <openclvars.hpp>

template <typename T>
cl_mem* createBuffer(T* data, size_t datasize, cl_mem_flags flags)
{
    static cl_mem buffer;
    int err;
    buffer = clCreateBuffer(context, flags, datasize, data, &err);
    printCLError(err, "CREATE BUFFER");
    return &buffer;
}

template <typename T>
void writeBuffer(T* data, cl_mem* buffer, size_t datasize)
{
    int err;
    err = clEnqueueWriteBuffer(queue, *buffer, CL_TRUE, 0, datasize, data, 0, NULL, NULL);
    printCLError(err, "WRITE BUFFER");
}

template <typename T>
void readBuffer(T* data, cl_mem* buffer, size_t datasize)
{
    int err;
    err = clEnqueueReadBuffer(queue, *buffer, CL_TRUE, 0, datasize, data, 0, NULL, NULL);
    printCLError(err, "READ BUFFER");
}


#endif