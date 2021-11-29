#pragma OPENCL EXTENSION cl_khr_fp64 : enable

typedef struct Complex
{
    double x, y;
}Complex;

Complex Add(Complex c1, Complex c2)
{
    Complex ret = {c1.x + c2.x, c1.y + c2.y};
    return ret;
}

Complex Mult(Complex c1, Complex c2)
{
    Complex ret = {c1.x * c2.x - c1.y * c2.y, c1.x * c2.y + c1.y * c2.x};
    return ret;
}

Complex expC(double t)
{
    Complex ret = {cos(t), sin(t)};
    return ret;
}

Complex Conjugate(Complex c)
{
    Complex ret = {c.x, -c.y};
    return ret;
}

double Mod2(Complex c)
{
    return c.x * c.x + c.y * c.y;
}

double Mod(Complex c)
{
    return sqrt(Mod2(c));
}

kernel void DFT(global Complex *x, global Complex *X, int N)
{
    int k = get_global_id(0);
    double pi = acos(-1.0);
    for(int n = 0; n < N; n++)
    {
        X[k] = Add(X[k], Mult(x[n], expC(-2.0 * pi * k * n / N)));
    }
}