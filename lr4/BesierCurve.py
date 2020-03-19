from math import factorial
import numpy as np

def binomial(n, k):
    if(k > n):
        return 0
    return factorial(n)/(factorial(n-k)*factorial(k))

def calculateBesierCurve(P):
    degree = len(P) - 1
    T = [np.array([t ** (degree - i) for i in range(0, degree + 1)]) for t in np.arange(0, 1.001, 0.001)]
    besie_matrix = np.array([[((-1) ** ((n + k) % 2)) * binomial(degree, n) * binomial(n, k) for k in range(0, degree + 1)] for n in range(degree, -1, -1)])
    return [np.dot(np.dot(T[i], besie_matrix), P) for i in range(0, len(T))]