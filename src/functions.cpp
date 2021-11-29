#include <complex.hpp>
#include <functions.hpp>

const double pi = acos(-1.0);
const char *lines = "-----------------------------\n";

Complex Add(Complex c1, Complex c2)
{
    return Complex(c1.x + c2.x, c1.y + c2.y);
}

Complex Mult(Complex c1, Complex c2)
{
    return Complex(c1.x * c2.x - c1.y * c2.y, c1.x * c2.y + c1.y * c2.x);
}

Complex Mult(double a, Complex c1)
{
    return Complex(c1.x * a, c1.y * a);
}


Complex expC(Complex c)
{
    double alpha = exp(-c.y);
    return Complex(alpha * cos(c.x), alpha * sin(c.x));
}

Complex expC(double t)
{
    return Complex(cos(t), sin(t));
}

Complex Conjugate(Complex c)
{
    return Complex(c.x, -c.y);
}

double Mod2(Complex c)
{
    return c.x * c.x + c.y * c.y;
}

double Mod(Complex c)
{
    return sqrt(Mod2(c));
}

void DFT(Complex *x, Complex *X, int N, int k)
{
    for(int n = 0; n < N; n++)
    {
        X[k] = Add(X[k], Mult(x[n], expC(-2.0 * pi * k * n/ N)));
    }
}

void printCLError(int err, const char* local)
{
    if(err != CL_SUCCESS)
    {
        printf("%s", lines);
        printf("ERROR %s: %d\n", local, err);
        printf("%s", lines);
    }
}

int findNFile(const char* file)
{
    FILE *f = fopen(file, "r");
    fseek(f, 0, SEEK_SET);
    fseek(f, 0, SEEK_END);
    int length = ftell(f);
    char *ptr = new char[length];
    fseek(f, 0, SEEK_SET);
    char *p;
    p = ptr;
    int ret = 0;
    while(!feof(f))
    {
        *p = fgetc(f);
        if(*p == '\n')
        {
            ret++;
        }
        p++;
    }
    fclose(f);
    return ret;
}

void readFile(const char *file, Complex *x)
{
    int N = findNFile(file);

    FILE *f = fopen(file, "r");

    for(int i = 0; i < N; i++)
    {
        int k;
        fscanf(f, "%d\t%lf\t%lf\n", &k, &x[i].x, &x[i].y);
    }
    fclose(f);
}