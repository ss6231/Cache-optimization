
#include <stdlib.h>
#include <stdio.h>


#define N	1024
#define DIM     512
#define DIM2    128
#define LARGE   10000

int list[LARGE];

int level_1();
void level_2();
void level_3();
void level_4();
void level_5();



/* Do not change anything above this line */


/***********************************************/


// For this level, I swapped the outer loop with the inner loop, so that the 2D array is being traversed in row-major order, which is exactly how the array is laid out in memory. The new function exhibits better spatial locality using a stride-1 reference pattern. Before the improvement, there was a stride-N reference pattern that resulted in a higher number of cache misses because the array elements were not being accessed consecutively and the  next memory address stored in the cache (which is organized in row-major order) is not
// present, which  significantly slows down the program.


int level_1()
{
	int B[N][N];
	
	int i, j;
	
	for(i = 0; i < N; i++)
		for(j = 0; j < N; j++)
			B[i][j] = 2*(B[i][j] + 2);
	 
	/* Do NOT change the the next few lines till the end of this function */  
	i = random () % N;
	j = random () % N;
	
	return(B[i][j]);
	
}

// This function summed up each column of B and put this summation into the corresponding diagonal of A. Before optimization, there was poor spatial locality because the inner loop was traversing in column-major order as opposed to row-major order, and because of this, the L1 cache never held the desired information, which resulted in cache misses. The general idea behind my optimization is to traverse B in row-major order, but update the diagonals of array A whenever it comes across the corresponding index of the inner loop. For example, my inner loop update is: A[j][j] += B[i][j] - if the current diagonal being updated is A[2][2], then this will be updated whenever j = 2, or in other words, whenever we are in the 2nd column. My update has a stride-1 reference pattern to exhibit good spatial locality. I decided to initialize the diagonals of A to 0 separately, because I needed to update the diagonal in such a way that adds the appropriate column value of B to the previously stored value in the corresponding diagonal of A so that I actually keep a running sum of the columns of B. If I kept the initialization of A as in the original function, the running sum of the columns would not be correct. 



void level_2()
{
	int temp = 0;
	int i, j;
	int A[DIM][DIM];
	int B[DIM][DIM];
	
	for(i = 0; i < DIM; i++) {
		A[i][i] = 0;
	}
	for (i = 0; i < DIM; i++) {
		for(j = 0; j < DIM; j++) {
			A[j][j] += B[i][j];
		} 
	}
	
	/* Do NOT change the the next few lines till the end of this function */  
	i = random () % DIM;
	for(j = 0; j < DIM; j++)
		temp += B[i][j];
	
	if( temp == A[i][i] )
		printf("level 2 ... completed!\n");
}


// Originally, this function exhibited good temporal locality with respect to the variable temp since it is referenced frequently. However, this function had subpar spatial locality because it was sequencing through the 2D array through the array in column major order, which accounted for the large number of L1 misses. By instead sequencing row by row, the cache is able to exhibit exceptional spatial locality (since the cache will pick up data that is in proximity to the current memory address and produce more hits if sequencing in row major order) and temporal locality due to the role of the temp variable. So by swapping the outer and inner loops, the function is optimized and produces more cache hits. 

void level_3()
{
		printf("\n");
	int i, j;
	int temp;
	int c[N][N];
	
	for(i = 0; i < N; i++)
		for(j = 0; j < N>>1; j++)
		{
			temp = c[i][j];
			c[i][j] = c[i][N-j];
			c[i][N-j] = temp;
		}
			
/* Do NOT change the the next few lines till the end of this function */  
		printf("level 3 ... completed!\n");

}

// This function sorts a list of numbers using bubble sort. Not only does this form of sorting exhibit subpar complexity, it also results in numerous cache misses. Although it references numbers that are next to each other in the array (and are then stored in the cache), once the algorithm reaches the end, it starts back at the beginning and results in many more cache misses. We must utilize those numbers that are most recently stored in the cache (which are the latter numbers in the array) and somehow use those to continue sorting and prevent cache misses. I decided to implement a bidirectional bubble sort that would sequence through first from left to right, bubbling out the heaviest elements, and then sequence from right to left, bubbling out the lightest elements. This way, we are utilizing the elements that were most recently placed in the cache to allow us to decrease the number of cache misses. So in the end, the new sorting algorithm has good spatial locality. 

void level_4() {
	int i, j;
	int temp;
	int t = 1;
	int stop = 0;
	for (i = LARGE-1; i > 0; i--) {
		if (t==1) {
			for (j = stop; j < i; j++) {
				if (list[j] > list[j+1]) {
					temp = list[j];
					list[j] = list[j+1];
					list[j+1] = temp;
				}
			}
			t = 0;
		}
		else if (t==0) {
			for (j=i; j > stop; j--) {
				if (list[j] < list[j-1]) {
					temp = list[j];
					list[j] = list[j-1];
					list[j-1] = temp;
				}
			}
			t = 1;
			stop++;
			i++;
		}
	}

			
/* Do NOT change the the next few lines till the end of this function */  
	i = random () % LARGE;
	if( list[0] < list[i] )
		printf("level 4 ... completed!\n");

}

// This function is multiplying elements that are not spatially related and putting the product into array C. A will produce many cache misses because we are jumping every 128th element starting at 0 and once we sequence all the way through, we move to the 1st element and then visit every 128th element from there. So all the elements that are placed in the cache that are nearby A[0] are not visited in the near future, resulting in many cache misses. Array C also exhibits this problem. Array B however does sequence through with a stride-1 pattern. By merely permuting the order of the for loops, we eliminate these problems significantly and result in an overall decrease in stride pattern due to better spatial locality.



void level_5()
{
	double A[DIM2*DIM2];
	double B[DIM2*DIM2];
	double C[DIM2*DIM2];
	double D[DIM2*DIM2];
	int n = DIM2;
	int i, j, k;

	for (j=0; j < n; j++ ) {
		for (k=0; k < n; k++) { 
		 for (i=0; i < n; i++) {
			C[i+j*n] += A[i+k*n] * B[k+j*n];
			}
		}
	}

	
	
					
/* Do NOT change the the next few lines till the end of this function */ 
	 printf("level 5 ... completed!\n");

}

int main(int argc, char *argv[])
{
	int i, j;
 
	for(i = 0; i < LARGE; i++)
		list[i] = random() % LARGE;
	
	if( level_1() % 2 == 0)
		printf("level 1 ... completed!\n");
	
	level_2();
	level_3();
	level_4();
	
	for(i = 0; i < LARGE; i++)
		list[i] = random() % LARGE;
 

	level_5( );
	
	
	return 1;
}
