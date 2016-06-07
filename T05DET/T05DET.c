/*Porosh in Mark 10-2 06.08.2015*/
/* FILE NAME: T05DET.c
 * PROGRAMMER: MP2
 * PURPOSE:-
 */

#include <stdio.h>
#define MAX 30
int P[MAX], N;
int parity = 1; 
double  sum = 0, A[MAX][MAX];

/* Begin of "Swap" */
void Swap( int *a, int *b )
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}
/* End of "Swap" */

/* Begin of "LoadMatrix" */
void LoadMatrix( char *FileName )
{

  int i,j;
  FILE *F;
  F = fopen(FileName, "r");
  if (F == NULL)
    return;
  fscanf(F, "%d", &N);
  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
      fscanf(F, "%lf", &A[i][j]);
}
/* End of "LoadMatrix" */

/* Begin of "FuncZ" */
void FuncZ ()
{
  int i, j, bi, bj, buf, parity = 1;
  double max = A[0][0], x, y;
  for(i=0; i<N; i++)
  {
    for(j=0; j<N; j++)
    {
      if(A[i][j] > max)
      {
        max = A[i][j];
        bi = i, bj = j;
      }
    }
  }
  for(i=0; i<N; i++,printf("\n"))
  {
    for(j=0; j<N; j++)
    {
      printf("%4.4lf ",A[i][j]);
    }
  }
  printf("\n");
  if (bj != 0)
  {
    parity = !parity;
    for(i=0; i<N; i++)
    {
      buf = A[i][0];
      A[i][0] = A[i][bj];
      A[i][bj] = buf;
    }
  }
  /*for(i=0; i<N; i++,printf("\n"))
  {
    for(j=0; j<N; j++)
    {
      printf("%4.4lf ",A[i][j]);
    }
  }*/
  printf("\n");
  if (bi != 0)
  {
    parity = !parity;
    for(j=0; j<N; j++)
    {
      buf = A[0][j];
      A[0][j] = A[bi][j];
      A[bi][j] = buf;
    }
  }
    for(i=0; i<N; i++,printf("\n"))
  {
    for(j=0; j<N; j++)
    {
      printf("%4.4lf ",A[i][j]);
    }
  } 
    printf("\n");
  for(i=0; i<N; i++)
  {
    x = A[0][i] / A[i+1][i];
    for(j=0; j<N; j++)
    {
      if(A[i+1][i] == 0)
        break;
      A[i+1][j]-= x * A[j][i];
    }
  }
  for(i=0; i<N; i++,printf("\n"))
  {
    for(j=0; j<N; j++)
    {
      printf("%4.4lf ",A[i][j]);
    }
  } 
  printf("\n");
}
/* End of "FuncZ" */

/* Begin of "Go" */
void Go( int Pos )
{
  int i; 
  double prod = 1;
  if (Pos == N)
  {
    for(i=0; i<N; i++)
    {
      if (parity == 1)
        prod *= A[i][P[i]];
      else 
        prod *= -A[i][P[i]];
    }
    sum += prod;
    return;
  }
  for (i=Pos; i<N; i++)
  {
    
    if (Pos != i)
    {
      parity = !parity; 
      Swap(&P[Pos],&P[i]);
    }
    Go(Pos + 1);
    if (Pos != i)
    {
      parity = !parity;
      Swap(&P[Pos],&P[i]);
    } 
  }
}
/* End of "Go" */

/* Begin of "EvalDeterminant" */
double EvalDeterminant( char *FileName )
{
  int i;
  LoadMatrix(FileName);
  FuncZ();
  sum = 0;
  for(i=0; i<N; i++)
    P[i] = i;
  Go(0);
  return sum;

}
/* End of "EvalDeterminant" */

/* Begin of "Main" */
void main( void )
{ 
  int i;
  char *M[] = {"m1.txt","m2.txt","mat1.txt","mat2.txt","mat3.txt","mat4.txt"};

  for(i=0; i < (sizeof(M) / sizeof(M[0])); i++)
    printf("File: %s sum: %4.2lf\n",M[i],EvalDeterminant(M[i]));
}
/* End of "main" */

/* End of FILE "T05DET" */
