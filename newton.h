
// Given x1, x2, y1=y(x1), y2=y(x2), proposes the next value x3=x2-y2*(x2-x1)/(y2-y1)
double NewtonStep(double y1, double y2, double x1, double x2);

// Computes the 3-points right derivative of Y at the Nth point with X-step=h
double DRight3(double *Y, int N, double h);

// Computes the 3-points left derivative of Y at the Nth point with X-step=h
double DLeft3(double *Y, int N, double h);

// Computes the 3-points right logarithmic derivative of Y at the Nth point with X-step=h
double DLogRight3(double *Y, int N, double h);

// Computes the 3-points left logarithmic derivative of Y at the Nth point with X-step=h
double DLogLeft3(double *Y, int N, double h);

void TestD3(void);
