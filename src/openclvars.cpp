#include <functions.hpp>
#include <complex.hpp>
#include <openclvars.hpp>
#include <funcs.tpp>

cl_program program;
cl_context context;
cl_command_queue queue;
cl_device_type devicetype;
cl_device_id device;
cl_platform_id plat[2];
size_t totalWork[1];
size_t workPerGroup[1];
int index_;

cl_kernel kDFT;

char* readKernel(const char* FileName)
{
    FILE *f = fopen(FileName, "r");

    if(f == NULL)
    {
        fprintf(stderr, "ERROR ON OPENING FILE\n");
        exit(1);
    }

    size_t length;
    int err;
    err = fseek(f, 0, SEEK_SET);

    if(err != 0)
    {
        fprintf(stderr, "ERROR ON SEEKING SET %d\n", err);
        exit(2);
    }

    err = fseek(f, 0, SEEK_END);
    if(err != 0)
    {
        fprintf(stderr, "ERROR ON SEEKING END %d\n", err);
        exit(3);
    }

    length = ftell(f);
    static char *out = new char[length + 1];
    fseek(f, 0, SEEK_SET);
    err = fread(out, sizeof(char), length, f);
    if(err != length)
    {
        fprintf(stderr, "ERROR ON READING FILE\n");
        exit(4);
    }

    out[length] = '\0';
    return out;
}

int gcd(int a, int b)
{
    if(b == 0){return a;}
    return gcd(b, a % b);
}

void initOpenCL(int N)
{
    if(index_ == 0)
    {
        devicetype = CL_DEVICE_TYPE_GPU;
    }
    else
    {
        devicetype = CL_DEVICE_TYPE_CPU;
    }

    int err;
    err = clGetPlatformIDs(2, plat, NULL);
    printCLError(err, "GET PLATAFORM ID");

    char *platInfo;
    size_t platInfoSize;
    err = clGetPlatformInfo(plat[index_], CL_PLATFORM_NAME, 0, NULL, &platInfoSize);
    printCLError(err, "GET PLATAFORM INFO");

    platInfo = new char[platInfoSize];

    err = clGetPlatformInfo(plat[index_], CL_PLATFORM_NAME, platInfoSize, platInfo, NULL);
    printCLError(err, "GET PLATAFORM INFO");
    printf("%sPLAT INFO: %s\n%s", lines, platInfo, lines);

    err = clGetDeviceIDs(plat[index_], devicetype, 1, &device, NULL);
    printCLError(err, "GET DEVICE ID");

    char *deviceInfo;
    size_t deviceInfoSize;
    err = clGetDeviceInfo(device, CL_DEVICE_NAME, 0, NULL, &deviceInfoSize);
    printCLError(err, "GET DEVICE INFO");

    deviceInfo = new char[deviceInfoSize];

    err = clGetDeviceInfo(device, CL_DEVICE_NAME, deviceInfoSize, deviceInfo, NULL);
    printCLError(err, "GET DEVICE NAME");
    printf("%sDEVICE INFO: %s\n%s", lines, deviceInfo, lines);

    size_t dims;
    err = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(size_t), &dims, NULL);
    printCLError(err, "GET DEVICE MAX WORK DIMS");
    printf("MAX DIMS: %d\n", dims);

    size_t sizet;

    err = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, 0, NULL, &sizet);
    printCLError(err, "GET DEVICE MAX WORK ITEMS");

    size_t *maxperDim = new size_t[sizet / sizeof(size_t)];
    err = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizet, maxperDim, NULL);
    printCLError(err, "GET DEVICE MAX WORK ITEMS");

    printf("MAX PER GROUP: (");
    for(int i = 0; i < dims - 1; i++)
    {
        printf("%d, ", maxperDim[i]);
    }
    printf("%d)\n", maxperDim[dims - 1]);


    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    printCLError(err, "CREATE CONTEXT");

    queue = clCreateCommandQueue(context, device, (cl_command_queue_properties)0, &err);
    printCLError(err, "CREATE QUEUE");

    const char* kernel = readKernel("./OpenCL/kernel.c");

    program = clCreateProgramWithSource(context, 1, &kernel, NULL, &err);
    printCLError(err, "CREATE PROGRAM WITH SOURCE");

    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    printCLError(err, "BUILD PROGRAM");

    char *programBuildInfo;
    size_t programBuildSize;
    err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &programBuildSize);
    printCLError(err, "GET PROGRAM BUILD INFO");

    programBuildInfo = new char[programBuildSize];
    err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, programBuildSize, programBuildInfo, NULL);
    printCLError(err, "GET PROGRAM BUILD INFO");
    printf("%sBUILD INFO:\n%s\n%s", lines, programBuildInfo, lines);

    kDFT = clCreateKernel(program, "DFT", &err);
    printCLError(err, "CREATE DFT KERNEL");

    size_t aux = gcd(N, (int)maxperDim[0]);
    printf("%d\n", aux);
    *workPerGroup = aux;
    *totalWork = N;
}

void setDFTXParam(Complex* c, cl_mem* buffer, size_t size)
{
    int err;
    buffer = createBuffer(c, size, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE);
    writeBuffer(c, buffer, size);

    err = clSetKernelArg(kDFT, 1, sizeof(cl_mem), buffer);
    printCLError(err, "SET DFT KERNEL ARG 1");
}

void setDFTXParam(cl_mem* buffer)
{
    int err;
    err = clSetKernelArg(kDFT, 1, sizeof(cl_mem), buffer);
    printCLError(err, "SET DFT KERNEL ARG 1");
}

void setDFTxParam(Complex* c, cl_mem* buffer, size_t size)
{
    int err;
    buffer = createBuffer(c, size, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE);
    writeBuffer(c, buffer, size);

    err = clSetKernelArg(kDFT, 0, sizeof(cl_mem), buffer);
    printCLError(err, "SET DFT KERNEL ARG 0");
}

void setDFTxParam(cl_mem* buffer)
{
    int err;
    err = clSetKernelArg(kDFT, 0, sizeof(cl_mem), buffer);
    printCLError(err, "SET DFT KERNEL ARG 0");
}

void setDFTNParam(int N)
{
    int err = clSetKernelArg(kDFT, 2, sizeof(int), &N);
    printCLError(err, "SET DFT KERNEL ARG 2");
}

void enqueueND(cl_kernel k)
{
    int err = clEnqueueNDRangeKernel(queue, k, 1, NULL, totalWork, workPerGroup, 0, NULL, NULL);
    printCLError(err, "ENQUEUE ND");
}

void finish()
{
    int err = clFinish(queue);
    printCLError(err, "FINISH");
}