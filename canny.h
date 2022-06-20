#ifndef CANNY_H
#define CANNY_H
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define PI 3.1415926535898
#define PI_2 1.57079632679

float ApproxAtan(float z);

float ApproxAtan2(float y, float x);

float powerOfTen(int num);

float squareRoot(float a);

float exponential(int n, float x);

void cannyFilter(float Input2[][320],float Output[][320], int upperThreshold, int lowerThreshold);

void calc_filter(float kernel[][5]);

void creating_kernel(float kernel[][5]);

void findEdge(int rowShift, int colShift, int row, int col, int dir, int lowerThreshold,
			  float Input[][320], int angle[][320], float gradient[][320]);

void suppressNonMax(int rowShift, int colShift, int row, int col, int dir, int lowerThreshold,
					float Input[][320], int angle[][320], float gradient[][320]);


#endif // CANNY_H
