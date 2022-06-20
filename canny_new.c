#include "canny.h"

float powerOfTen(int num){
     float rst = 1.0;
	 int i;
     if(num >= 0){
         for(i = 0; i < num ; i++){
             rst *= 10.0;
         }
     }else{
         for(i = 0; i < (0 - num ); i++){
            rst *= 0.1;
        }
    }

    return rst;
 }
 
 float squareRoot(float a)
 {
     /*
	 Reference : https://www.codeproject.com/Articles/570700/SquareplusRootplusalgorithmplusforplusC
           find more detail of this method on wiki methods_of_computing_square_roots
 
           *** Babylonian method cannot get exact zero but approximately value of the square_root
      */
      float z = a; 
      float rst = 0.0;
     int max = 8;     // to define maximum digit 
     int i;
     float j = 1.0;
     for(i = max ; i > 0 ; i--){
         // value must be bigger then 0
         if(z - (( 2 * rst ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)) >= 0)
         {
             while( z - (( 2 * rst ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)) >= 0)
             {
                 j++;
                 if(j >= 10) break;

             }
             j--; //correct the extra value by minus one to j
             z -= (( 2 * rst ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)); //find value of z
 
             rst += j * powerOfTen(i);     // find sum of a

             j = 1.0; 
 
          }
 
      }
 
      for(i = 0 ; i >= 0 - max ; i--){
          if(z - (( 2 * rst ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)) >= 0)
          {
              while( z - (( 2 * rst ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)) >= 0)
              {
                  j++;

              }
              j--;
              z -= (( 2 * rst ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)); //find value of z
 
              rst += j * powerOfTen(i);     // find sum of a
              j = 1.0;
           }
      }
      // find the number on each digit
      return rst;
 }
 float exponential(int n, float x)
{
    float sum = 1.0f; // initialize sum of series
    int i;
    for (i = n - 1; i > 0; --i )
        sum = 1 + x * sum / i;
 
    return sum;
}

float ApproxAtan(float z)
{
    const float n1 = 0.97239411f;
    const float n2 = -0.19194795f;
    return (n1 + n2 * z * z) * z;
}

float ApproxAtan2(float y, float x)
{
    if (x != 0.0f)
    {
		// Reference: https://www.dsprelated.com/showarticle/1052.php
        if (fabsf(x) > fabsf(y))
        {
            const float z = y / x;
            if (x > 0.0)
            {
                // atan2(y,x) = atan(y/x) if x > 0
                return ApproxAtan(z);
            }
            else if (y >= 0.0)
            {
                // atan2(y,x) = atan(y/x) + PI if x < 0, y >= 0
                return ApproxAtan(z) + PI;
            }
            else
            {
                // atan2(y,x) = atan(y/x) - PI if x < 0, y < 0
                return ApproxAtan(z) - PI;
            }
        }
        else // Use property atan(y/x) = PI/2 - atan(x/y) if |y/x| > 1.
        {
            const float z = x / y;
            if (y > 0.0)
            {
                // atan2(y,x) = PI/2 - atan(x/y) if |y/x| > 1, y > 0
                return -ApproxAtan(z) + PI_2;
            }
            else
            {
                // atan2(y,x) = -PI/2 - atan(x/y) if |y/x| > 1, y < 0
                return -ApproxAtan(z) - PI_2;
            }
        }
    }
    else
    {
        if (y > 0.0f) // x = 0, y > 0
        {
            return PI_2;
        }
        else if (y < 0.0f) // x = 0, y < 0
        {
            return -PI_2;
        }
    }
    return 0.0f; // x,y = 0. Could return NaN instead.
}

void calc_filter(float kernel[][5]) {
	int i,j,x,y;
    float sigma = 1.0;
    float p;
    float q = 2.0 * sigma * sigma;
    float sum = 0.0;
    for (x = -2; x <= 2; x++) {
        for (y = -2; y <= 2; y++) {
            p = squareRoot(x * x + y * y);
            kernel[x + 2][y + 2] = (exponential(20,(-(p * p) / q))) / (PI * q);
            sum += kernel[x + 2][y + 2];
        }
    }
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            kernel[i][j] /= sum;
        }
    }
}

void findEdge(int rowShift, int colShift, int row, int col, int dir, int lowerThreshold, float Input[][320],  int angle[][320], float gradient[][320])
{
	int W = 320;
	int H = 240;
	int newRow=0;
	int newCol=0;
    bool edgeEnd = false;

    /* Find the row and column values for the next possible pixel on the edge */
    if (colShift < 0)
    {
        if (col > 0)
            newCol = col + colShift;
        else
            edgeEnd = true;
    }
    else if (col < W - 1)
    {
        newCol = col + colShift;
    }
    else
        edgeEnd = true;		// If the next pixel would be off image, don't do the while loop
    if (rowShift < 0)
    {
        if (row > 0)
            newRow = row + rowShift;
        else
            edgeEnd = true;
    }
    else if (row < H - 1)
    {
        newRow = row + rowShift;
    }
    else
        edgeEnd = true;

    /* Determine edge directions and gradient strengths */
    while ( (angle[newRow][newCol]==dir) && !edgeEnd && (gradient[newRow][newCol] > lowerThreshold) )
    {
        /* Set the new pixel as white to show it is an edge */
        Input[newRow][newCol] = 255;
        if (colShift < 0)
        {
            if (newCol > 0)
                newCol = newCol + colShift;
            else
                edgeEnd = true;
        }
        else if (newCol < W - 1)
        {
            newCol = newCol + colShift;
        } else
            edgeEnd = true;

        if (rowShift < 0)
        {
            if (newRow > 0)
                newRow = newRow + rowShift;
            else
                edgeEnd = true;
        }
        else if (newRow < H - 1)
        {
            newRow = newRow + rowShift;
        } else
            edgeEnd = true;
    }
}


void suppressNonMax(int rowShift, int colShift, int row, int col, int dir, int lowerThreshold,
					float Input[][320], int angle[][320], float gradient[][320])
{
	int W = 320;
	int H = 240;
	int newRow = 0;
	int newCol = 0;
    bool edgeEnd = false;
    float nonMax[320][3];		// Temporarily stores gradients and positions of pixels in parallel edges
    int pixelCount = 0;		// Stores the number of pixels in parallel edges
    int count;
    int max[3];			// Maximum point in a wide edge

    if (colShift < 0)
    {
        if (col > 0)
            newCol = col + colShift;
        else
            edgeEnd = true;
    }
    else if (col < W - 1)
    {
        newCol = col + colShift;
    }
    else
        edgeEnd = true;		// If the next pixel would be off image, don't do the while loop
    if (rowShift < 0)
    {
        if (row > 0)
            newRow = row + rowShift;
        else
            edgeEnd = true;
    }
    else if (row < H - 1)
    {
        newRow = row + rowShift;
    }
    else
        edgeEnd = true;
    /* Find non-maximum parallel edges tracing up */
    while ((angle[newRow][newCol] == dir) && !edgeEnd && (Input[newRow][newCol] == 255))
    {
        if (colShift < 0)
        {
            if (newCol > 0)
                newCol = newCol + colShift;
            else
                edgeEnd = true;
        }
        else if (newCol < W - 1)
        {
            newCol = newCol + colShift;
        }
        else
            edgeEnd = true;
        if (rowShift < 0)
        {
            if (newRow > 0)
                newRow = newRow + rowShift;
            else
                edgeEnd = true;
        }
        else if (newRow < H - 1)
        {
            newRow = newRow + rowShift;
        }
        else
            edgeEnd = true;

        nonMax[pixelCount][0] = newRow;
        nonMax[pixelCount][1] = newCol;
        nonMax[pixelCount][2] = gradient[newRow][newCol];
        pixelCount++;
    }

    /* Find non-maximum parallel edges tracing down */
    edgeEnd = false;
    colShift *= -1;
    rowShift *= -1;
    if (colShift < 0)
    {
        if (col > 0)
            newCol = col + colShift;
        else
            edgeEnd = true;
    }
    else if (col < W - 1)
    {
        newCol = col + colShift;
    }
    else
        edgeEnd = true;

    if (rowShift < 0)
    {
        if (row > 0)
            newRow = row + rowShift;
        else
            edgeEnd = true;
    }
    else if (row < H - 1)
    {
        newRow = row + rowShift;
    }
    else
        edgeEnd = true;

    while ((angle[newRow][newCol] == dir) && !edgeEnd && (Input[newRow][newCol]== 255))
    {
        if (colShift < 0)
        {
            if (newCol > 0)
                newCol = newCol + colShift;
            else
                edgeEnd = true;
        }
        else if (newCol < W - 1)
        {
            newCol = newCol + colShift;
        }
        else
            edgeEnd = true;
        if (rowShift < 0)
        {
            if (newRow > 0)
                newRow = newRow + rowShift;
            else
                edgeEnd = true;
        }
        else if (newRow < H - 1)
        {
            newRow = newRow + rowShift;
        }
        else
            edgeEnd = true;

        nonMax[pixelCount][0] = newRow;
        nonMax[pixelCount][1] = newCol;
        nonMax[pixelCount][2] = gradient[newRow][newCol];
        pixelCount++;
    }

    /* Suppress non-maximum edges */
    max[0] = 0;
    max[1] = 0;
    max[2] = 0;
    for (count = 0; count < pixelCount; count++)
    {
        if (nonMax[count][2] > max[2])
        {
            max[0] = nonMax[count][0];
            max[1] = nonMax[count][1];
            max[2] = nonMax[count][2];
        }
    }
    for (count = 0; count < pixelCount; count++)
    {
    	Input[(int)nonMax[count][0]][(int)nonMax[count][1]]= 0;
    }

}

void cannyFilter(float Input2[][320],float Output[][320], int upperThreshold, int lowerThreshold)
{
	int i,j,x,y,k1,k2,row,col;
    float kernel[5][5];
    calc_filter(kernel);
	float Input[240][320];
	
	for(i=0;i<240;i++)
	{
		for(j=0;j<320;j++)
		{
			Input[i][j]=Input2[i][j];
		}
	}

    float curr=0;
    for (i=2; i<240-2; i++)
    {
        for (j=2; j<320-2; j++)
        {
            curr=0;
            for (k1=0; k1<5; k1++)
            {
                for (k2=0; k2<5; k2++)
                {
                    curr += kernel[k1][k2]*Input2[i+(k1-2)][j+(k2-2)];
                }
            }
            Input[i][j] = curr;
        }
    }


    float* img2dhororg[240];
    for (i = 0; i < 240; i++)
    	img2dhororg[i] = (float*)malloc(320 * sizeof(float));

    float* img2dverorg[240];
    for (i = 0; i < 240; i++)
    	img2dverorg[i] = (float*)malloc(320 * sizeof(float));

    //horizontal - Gy
    for (i=1; i<240-1; i++)
    {
        for (j=1; j<320-1; j++)
        {
            img2dhororg[i][j]=Input[i-1][j-1] + 2*Input[i-1][j]+Input[i-1][j+1]-Input[i+1][j-1]-2*Input[i+1][j]-Input[i+1][j+1];
        }
    }
    //vertical - Gx
    for (i=1; i<240-1; i++)
    {
        for (j=1; j<320-1; j++)
        {
            img2dverorg[i][j]=Input[i-1][j-1]+2*Input[i][j-1]+Input[i+1][j-1]-Input[i-1][j+1]-2*Input[i][j+1]-Input[i+1][j+1];
        }
    }
    
    float gradient[240][320];


    int angle[240][320];

    float curAngle, newAngle=0;
    //magnitude
    for (i=0; i<240; i++)
    {
        for (j=0; j<320; j++)
        {
            gradient[i][j] = squareRoot(pow(img2dhororg[i][j], 2)+  pow(img2dverorg[i][j], 2));
            gradient[i][j] = gradient[i][j] > 255 ? 255:gradient[i][j];
            gradient[i][j] = gradient[i][j] < 0 ? 0 : gradient[i][j];
            curAngle = (ApproxAtan2(img2dverorg[i][j],img2dhororg[i][j])/PI)*180;
            /* Convert actual edge direction to approximate value */
            if ( ( (curAngle < 22.5) && (curAngle > -22.5) ) || (curAngle > 157.5) || (curAngle < -157.5) )
                newAngle = 0;
            if ( ( (curAngle > 22.5) && (curAngle < 67.5) ) || ( (curAngle < -112.5) && (curAngle > -157.5) ) )
                newAngle = 45;
            if ( ( (curAngle > 67.5) && (curAngle < 112.5) ) || ( (curAngle < -67.5) && (curAngle > -112.5) ) )
                newAngle = 90;
            if ( ( (curAngle > 112.5) && (curAngle < 157.5) ) || ( (curAngle < -22.5) && (curAngle > -67.5) ) )
                newAngle = 135;

            angle[i][j] = newAngle;
        }
    }

    for (i = 0; i < 240; i++)
        free(img2dhororg[i]); 
    for (i = 0; i < 240; i++)
        free(img2dverorg[i]);

    /* Trace along all the edges in the image */
    for (row = 1; row < 240 - 1; row++)
    {
        for (col = 1; col < 320 - 1; col++)
        {
            if (gradient[row][col] > upperThreshold)
            {
                /* Switch based on current pixel's edge direction */
                switch (angle[row][col])
                {
                    case 0:
                        findEdge(0, 1, row, col, 0, lowerThreshold,Input, angle, gradient);
                        break;
                    case 45:
                        findEdge(1, 1, row, col, 45, lowerThreshold,Input, angle, gradient);
                        break;
                    case 90:
                        findEdge(1, 0, row, col, 90, lowerThreshold,Input, angle, gradient);
                        break;
                    case 135:
                        findEdge(1, -1, row, col, 135, lowerThreshold,Input, angle, gradient);
                        break;
                    default :
                        Input[row][col]= 0;
                        break;
                }
            }
            else
            {
                Input[row][col]= 0;
            }
        }
    }

    /* Non-maximum Suppression */
    for (row = 1; row < 240 - 1; row++)
    {
        for (col = 1; col < 320 - 1; col++)
        {
            if (Input[row][col] == 255)
            {		// Check to see if current pixel is an edge
                /* Switch based on current pixel's edge direction */
                switch (angle[row][col])
                {
                    case 0:
                        suppressNonMax( 1, 0, row, col, 0, lowerThreshold,Input, angle, gradient);
                        break;
                    case 45:
                        suppressNonMax( 1, -1, row, col, 45, lowerThreshold,Input, angle, gradient);
                        break;
                    case 90:
                        suppressNonMax( 0, 1, row, col, 90, lowerThreshold,Input, angle, gradient);
                        break;
                    case 135:
                        suppressNonMax( 1, 1, row, col, 135, lowerThreshold,Input, angle, gradient);
                        break;
                    default :
                        break;
                }
            }
        }
    }

for (y = 0; y < 240; ++y)
{
    for (x = 0; x < 320; ++x)
    {
        Output[y][x] = Input[y][x];
    }
}




}
