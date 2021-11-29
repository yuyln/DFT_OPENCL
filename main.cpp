#include <functions.hpp>
#include <complex.hpp>
#include <openclvars.hpp>
#include <funcs.tpp>

int main()
{
    int N = findNFile("in.in");
    Complex *x = new Complex[N];
    Complex *X = new Complex[N];
    readFile("in.in", x);

    initOpenCL(N);
    cl_mem bufferx = *createBuffer(x, N * sizeof(Complex), CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE);
    writeBuffer(x, &bufferx, sizeof(Complex) * N);

    cl_mem bufferX = *createBuffer(X, N * sizeof(Complex), CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE);

    setDFTxParam(&bufferx);
    setDFTXParam(&bufferX);
    setDFTNParam(N);
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    enqueueND(kDFT);
    finish();
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

    double dur = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
    printf("GPU: %f\n", dur);
    readBuffer(X, &bufferX, sizeof(Complex) * N);


    std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < N; i++)
    {
        DFT(x, X, N, i);
    }
    
    std::chrono::high_resolution_clock::time_point t4 = std::chrono::high_resolution_clock::now();
    double dur1 = std::chrono::duration_cast<std::chrono::duration<double>>(t4 - t3).count();
    printf("CPU: %f\n", dur1);

    FILE *out = fopen("out.out", "w");
    for(int i = 0; i < N - 1; i++)
    {
        fprintf(out, "%d\t%.12f\t%.12f\n", i, fabs(X[i].x), fabs(X[i].y));
    }
    fprintf(out, "%d\t%.12f\t%.12f\n", N - 1, fabs(X[N - 1].x), fabs(X[N - 1].y));
    fclose(out);


    return 0;
}