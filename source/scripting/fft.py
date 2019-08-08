import numpy as np
from scipy.fftpack import fft
import matplotlib.pyplot as plt

# Number of sample points
N = 512
# sample spacing
f_c = 800.0
t = np.linspace(0.0, N/f_c, N)
x = np.sin(10.0 * 2.0*np.pi*t) + 0.5*np.sin(80.0 * 2.0*np.pi*t)


plt.figure(1)
plt.subplot(2,1,1)
plt.plot(t, x)
plt.grid()


X = fft(x)
#for N%2=0 first N/2 elements is relative to positive frequencies
F = np.linspace(0.0, 0.5, N//2+1)
f = F*f_c

A = 2.0/N * np.abs(X[0:N//2+1])
print(F[-1])

plt.subplot(2,1,2)
plt.plot(F, A)
plt.grid()
plt.show()
