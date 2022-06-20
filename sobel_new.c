#include "sobel.h"


void sobelFilter(float Input[][320],float Output[][320])
{
int i,j,x,y;
float* img2dhororg[240];
for (i = 0; i < 240; i++)
	img2dhororg[i] = (float*)malloc(320 * sizeof(float));


float* img2dverorg[240];
for (i = 0; i < 240; i++)
	img2dverorg[i] = (float*)malloc(320 * sizeof(float));

float* img2dmag[240];
for (i = 0; i < 240; i++)
	img2dmag[i] = (float*)malloc(320 * sizeof(float));


//horizontal

for (i=1; i<240-1; i++)
{
    for (j=1; j<320-1; j++)
    {
         img2dhororg[i][j]=Input[i-1][j-1] + 2*Input[i-1][j]+Input[i-1][j+1]-Input[i+1][j-1]-2*Input[i+1][j]-Input[i+1][j+1];
     }
}

//vertical

for (i=1; i<240-1; i++)
{
    for (j=1; j<320-1; j++)
    {
         img2dverorg[i][j]=Input[i-1][j-1] + 2*Input[i-1][j]+Input[i-1][j+1]-Input[i+1][j-1]-2*Input[i+1][j]-Input[i+1][j+1];
     }
}

//magnitude

for (i=0; i<240; i++)
{
    for (j=0; j<320; j++)
    {
        img2dmag[i][j] = squareRoot(pow(img2dhororg[i][j], 2)+pow(img2dverorg[i][j], 2));
        img2dmag[i][j] = img2dmag[i][j] > 255 ? 255:img2dmag[i][j];
        img2dmag[i][j] = img2dmag[i][j] < 0 ? 0 : img2dmag[i][j];
    }
}

for (i = 0; i < 240; i++)
    free(img2dhororg[i]); 
for (i = 0; i < 240; i++)
    free(img2dverorg[i]);

for (y = 0; y < 240; ++y)
{
    for (x = 0; x < 320; ++x)
    {
        Output[y][x] = img2dmag[y][x];
    }
}
for (i = 0; i < 240; i++)
    free(img2dmag[i]);
 
}




