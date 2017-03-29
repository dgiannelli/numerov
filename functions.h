
extern double R0, V0;

double Zero(double r);

double Well(double r);

double Gauss(double r);

void Linspace(double *X, double from, double to, int N);

// Computes the 3-points right derivative of Y(X) at the Nth piont (Xstep must be constant)
double DRight3(double *Y, int N, double h);

// Computes the 3-points left derivative of Y(X) at the Nth point (Xstep must be constant)
double DLeft3(double *Y, int N, double h);

// Computes the 3-points right logarithmic derivative of Y(X) at the Nth point (Xstep must be constant)
double DLogRight3(double *Y, int N, double h);

// Computes the 3-points left logarithmic derivative of Y(X) at the Nth point (Xstep must be constant)
double DLogLeft3(double *Y, int N, double h);

// Numerov algorithm: given Z0 and Z1 finds Z2
void NumerovRight(double *X, double *Y, double (*W)(double), int N);

// Numerov algorithm: given Z2 and Z1 finds Z0
void NumerovLeft(double *X, double *Y, double (*W)(double), int N);

// ***** Function tests *****

void TestLinspace(void);

void TestD3(void);

void TestNumerov(void);

