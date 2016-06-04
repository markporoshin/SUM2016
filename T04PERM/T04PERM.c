/*Poroshin Mark 10-2 02.08.2015*/
/* FILE NAME: T04PERM.c
 * PROGRAMMER: MP2
 * PURPOSE:-
 */

#include <stdio.h>
#define N 5
int P[N];
int parity = 1;

void fact( int sum )
{
  int i=0;
  for(i=sum; i > 1; i++)
    sum *= (i-1);
}

/* Begin of "Swap" */
void Swap( int *a, int *b )
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}
/* End of "Swap" */

/* Begin of "SavePerm" */
void SavePerm( int p )
{
  int i;
  FILE *F; 
  F = fopen("perm.log","a");
  if (F == NULL)
    return;
  for(i=0; i<N-1; i++)
    fprintf(F, "%2i", P[i]);
  fprintf(F, "%2i parity: %i\n", P[N-1],p);
  fclose(F);
}
/* End of "SavePerm" */

void went( int Pos )
{
  int i;
  if(Pos = N)
  {
    SavePerm(1);
    went(Pos-1);
  }
  for(i = N; i > Pos; i--)                                                                                                
  {
     Swap(&P[i],&P[i+1]);
     
     Swap(&P[i],&P[i+1]);
  }
}

/* Begin of "Go" */
void Go( int Pos )
{
  int i; 
  if (Pos == N)
  {
    SavePerm(parity);
    return;
  }
  for (i=Pos; i<N; i++)
  {
    Swap(&P[Pos],&P[i]);
    if(Pos != i)
      parity = !parity; 
    Go(Pos+1);
    if(Pos != i)
      parity = !parity;
    Swap(&P[Pos],&P[i]);
  }
}
/* End of "Go" */

/* Begin of "Main" */
void main( void )
{
  FILE *F; 
  int i;
  F = fopen("perm.log","wb");
  fclose(F); 
  for(i=1; i<N+1; i++)
    P[i] = i;
  went(5);
}
/* End of "main" */

/* End of FILE "T04Perm" */
