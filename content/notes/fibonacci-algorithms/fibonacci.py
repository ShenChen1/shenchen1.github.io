#!/usr/bin/env python

import os
import sys
import math

def f(n):
	if n < 2:
		return n

	return f(n - 1) + f(n - 2)

def f1(n):
	if n < 2:
		return n

	f0 = 0
	f1 = 1
	for i in range(2, n + 1):
		tmp = f0 + f1
		f0 = f1
		f1 = tmp
	return tmp

def f2(n):
	k = n
	bit = []
	while k:
		bit.append(k % 2)
		k = k >> 1

	a = 0
	b = 1
	for i in range(len(bit)-1, -1, -1):
		tmp = a * (2 * b - a)
		b = b * b + a * a
		a = tmp
		if bit[i]:
			tmp = a + b
			a = b
			b = tmp
	return a

# square(5) accuracy problem when n > 70
def f3(n):
	golden_ratio = (1 + 5 ** 0.5) / 2
	return int((golden_ratio ** n + 1) / 5 ** 0.5)

if __name__ == '__main__':
	i = int(sys.argv[1])
	#print(f(i))
	print(f1(i))
	print(f2(i))
	print(f3(i))

