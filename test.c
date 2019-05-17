/* 
 * This program demonstrates the use of MPI_Alltoall when
 * transpsong a square matrix.
 * For simplicity, the number of processes is 4 and the dimension
 * of the matrix is fixed to 128
 */

#include <stdio.h>
#include <mpi.h>




void
trans (int *a, int n)
/* transpose square matrix a, dimension nxn
 * Consider this as a black box for the MPI course
 */

{
  int i, j;
  int ij, ji, l;
  double tmp;
  ij = 0;
  l = -1;
  for (i = 0; i < n; i++)
    {
      l += n + 1;
      ji = l;
      ij += i + 1;
      for (j = i+1; j < n; j++)
	{
	  tmp = a[ij];
	  a[ij] = a[ji];
	  a[ji] = tmp;
	  ij++;
	  ji += n;
	}
    }
}

int main (int argc, char *argv[])
{

	FILE * fp1;
FILE * fp2;
FILE * fp3;
FILE * fp4;
FILE * fpFinal;
int SIZE = 128;
int SIZEH = 32;

	fp1 = fopen("process1.txt", "w+");
	fp2 = fopen("process2.txt", "w+");
	fp3 = fopen("process3.txt", "w+");
	fp4 = fopen("process4.txt", "w+");
	fpFinal = fopen("transMatrix.txt", "w");
	char string[80];
  int a[SIZE][SIZEH];
  int b[SIZE][SIZEH];

  int i, j, nprocs, rank;
  MPI_Datatype colType;
  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);

  

  MPI_Type_vector(SIZE, 1,SIZE, MPI_INT, &colType);
  MPI_Type_commit(&colType);


  if(rank == 0)
  {
	//int c[8][8];
    printf("Transposing a 128x128 matrix, divided among 4 processors\n");
  }

  if (nprocs != 4)
    {
      if (rank == 0)
	printf ("Error, number of processes must be 5\n");
      MPI_Finalize ();
      return 1;
    }

//if(rank != 5){

	//printf("Here is the normal matrix");
	//printf("Hello, World!");
	for (i = 0; i < SIZE; i++){
		for (j = 0; j < SIZEH; j++){
	     	a[i][j] = 1000 * i + j + SIZEH* rank; /* give every element a unique value */
		//printf("%d ", a[i][j]);
		}
		//printf("\n");
	}
	//printf("End of the normal matrix");

  /* do the MPI part of the transpose */

  /* Tricky here is the number of items to send and receive. 
   * Not 128x32 as one may guess, but the amount to send to one process
   * and the amount to receive from any process */

  MPI_Alltoall (&a[0][0],	/* address of data to send  */
		SIZEH * SIZEH,	/* number of items to send to one process  */
		MPI_INT,	/* type of data  */
		&b[0][0],	/* address for receiving the data  */
		/* NOTE: send data and receive data may NOT overlap */
		SIZEH * SIZEH,	/* number of items to receive 
				   from any process  */
		MPI_INT,	/* type of receive data  */
		MPI_COMM_WORLD);

  /* MPI_Alltoall does not a transpose of the data received, we have to
   * do this ourself: */

  /* transpose 4 square matrices, order 32x32: */

  for (i = 0; i < 4; i++)
      trans (&b[i * SIZEH][0], SIZEH);

  /* now check the result */

  for (i = 0; i < SIZE; i++)
    for (j = 0; j < SIZEH; j++)
      {
	if (b[i][j] != 1000 * (j + SIZEH * rank) + i )
	  {
	    printf ("process %d found b[%d][%d] = %d, but %d was expected\n",
		   rank, i, j, b[i][j], (int) (1000 * (j + SIZEH * rank) + i));
	    MPI_Abort (MPI_COMM_WORLD,SIZEH);
	    return 1;
	  }
      }
  if(rank == 0)
    printf ("Transpose seems ok\n");

	
  if(nprocs == SIZE){
 	
    if(rank != 0){

	for(int i = 0; i < nprocs; i++){}
	
    }

  }



/*

		printf("Here is the normal matrix\n");
	for (int k = 0; k < 4; k++){
		for (int n = 0; n < 1; n++){
	     	//a[k][n] = 1000 * k + n + 32 * rank; // give every element a unique value 
		printf("%d ", a[k][n]);
		}
		printf("\n");
	}
	printf("End of the normal matrix\n\n\n");
*/
	
//}

MPI_Barrier(MPI_COMM_WORLD);



		//printf("Here is the altered matrix\n");
		for (int k = 0; k < SIZE; k++){
			for (int n = 0; n < SIZEH; n++){
		     	//a[k][n] = 1000 * k + n + 32 * rank; // give every element a unique value 
				/*if(rank == 0)
					printf("r0: %d ", b[k][n]);
				if(rank == 1)
					printf("r1: %d ", b[k][n]);
				if(rank == 2)
					printf("r2: %d ", b[k][n]);
				if(rank == 3)
					printf("r3: %d ", b[k][n]);*/
				/*if(n > -1 && n < SIZE/nprocs){
					if(rank == 0)					
					printf("%d ", b[k][n]);					
				}
				if(n >= SIZE/nprocs && n < SIZE/nprocs*2){
					if(rank == 1)					
					printf("%d ", b[k][n]);					
				}
				if(n >= SIZE/nprocs*2 && n < SIZE/nprocs*3){
					if(rank == 2)					
					printf("%d ", b[k][n]);					
				}
				if(n >= SIZE/nprocs*3 && n < SIZE/nprocs*4){
					if(rank == 3)					
					printf("%d ", b[k][n]);					
				}*/
			if(rank == 0){
				fprintf(fp1, "%d ", b[k][n]);
							
			}
			if(rank == 1){
				fprintf(fp2, "%d ", b[k][n]);
							
			}
			if(rank == 2){
				fprintf(fp3, "%d ", b[k][n]);
							
			}
			if(rank == 3){
				fprintf(fp4, "%d ", b[k][n]);
							
			}
				
		}

			if(rank == 0){
				fprintf(fp1, "\n");
							
			}
			if(rank == 1){
				fprintf(fp2, "\n");
							
			}
			if(rank == 2){
				fprintf(fp3, "\n");
							
			}
			if(rank == 3){
				fprintf(fp4, "\n");
							
			}
			
			

			

			
		}

MPI_Barrier(MPI_COMM_WORLD);
fp1 = fopen("process1.txt", "w+");
;
	
		if(rank == 0){
	
/*
				fgets(string, 20, fp1);	
				//string = {0};
				fputc(string, fpFinal);
				fgets(string, 20, fp2);
				fputc(string, fpFinal);				//string = {0};
				fgets(string, 20, fp3);
				//string = {0};
				fputc(string, fpFinal);			
				fgets(string, 20, fp4);	
				fputc(string, fpFinal);
				//string = {0};
				//fprintf(fpFinal, "%");
	
				*/

			
				
		
		}
		//printf("End of the altered matrix\n");
		  MPI_Finalize ();

	// Millie's stuff

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fpFinal);
	
  return 0;
}

