# Hough Transformation

## What is the Hough Transformation?

[This Wiki page](http://en.wikipedia.org/wiki/Hough_transform) gives a very clear and concise overview of the Hough transformation. In short, the Hough transformation converts the problem of finding collinear points to the problem of finding concurrent curves. It can pick out collinear points as well as human eyes can do.

Below is a C-style implementation of 2D Hough transformation. Hough transformations in higher dimensions require the discretization of high-dimensional phase space. This leads to computationally unacceptable costs in CPU time and memory.

## Interface
Input:

The raw data
		P_i(x_i,y_i)		i=1,2,...,N
	is passed as an array of double precision floating point number pairs,
	i.e., xy[2*N], where
		xy[2*i]   = x_i
		xy[2*i+1] = y_i
	The number of points, N, is also passed as a separate variable.


Output:
	TODO


	NOTE: Once the array xy[2*N] is passed to ht_create, the user MUST NOT
	alter its content until having called ht_destroy. The array is kept
	intact.
