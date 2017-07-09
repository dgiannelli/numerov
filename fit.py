#!/usr/bin/env python2

from scipy.optimize import curve_fit
import numpy as np

xdata, ydata = np.loadtxt("scattering.dat", unpack=True)

def f(x, A, B): return A + B*x

popt, pcov = curve_fit(f, xdata, ydata)
perr = np.sqrt(np.diag(pcov))

params = np.array([-1./popt[0],2*popt[1]])
errors = params*perr/popt

np.savetxt("fit.dat", np.transpose(np.concatenate((popt,params,errors)))[None], header='A\tB\ta\tr\tda\tdr')

