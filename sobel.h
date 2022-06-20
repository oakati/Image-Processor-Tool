#ifndef SOBEL_H
#define SOBEL_H
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "canny.h"



void sobelFilter(float Input[][320], float Output[][320]);

#endif // SOBEL_H
