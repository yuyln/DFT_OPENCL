#ifndef __FUNCS
#define __FUNCS
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <CL/cl.h>
#include <complex.hpp>
#include <string.h>
#include <chrono>

extern const double pi;
extern const char* lines;

Complex Add(Complex, Complex);
Complex Mult(Complex, Complex);
Complex Mult(double, Complex);
Complex expC(Complex);
Complex expC(double);
Complex Conjugate(Complex);
double Mod2(Complex);
double Mod(Complex);

void DFT(Complex *x, Complex *X, int N, int k);

void printCLError(int err, const char* local);

int findNFile(const char*);
void readFile(const char*, Complex*);
#endif
