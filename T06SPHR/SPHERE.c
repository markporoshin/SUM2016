/*Poroshin Mark 10-2 07.08.2015*/
/* FILE NAME: SPHERE.c
 * PROGRAMMER: MP2
 * PURPOSE:-                   */



#include <math.h>
#include <windows.h>
#include "SPHERE.h"

#define MP2_PI 3.14159265358979323846
#define N 15
#define M 30
typedef DOUBLE DBL;
typedef FLOAT FLT;

typedef struct
{
  DBL x,y,z;
} VEC;

VOID sphere( HDC hDC, INT xc, INT yc )
{
  DOUBLE phi, theta, x, y, z;
  INT i, j, k, r1 = 100, r2 = 5;
  HPEN hPen;
  SYSTEMTIME Time;
  hPen= CreatePen(PS_SOLID, 1, RGB(100, 150, 100));
  SelectObject(hDC, hPen);
  GetLocalTime(&Time);
    for(i = 0; i <= N; i++)
    {
      theta = i * MP2_PI / N;
      
      for(j = 0; j <= M; j++)
      {
        
        phi = j * 2.0 * MP2_PI / M + Time.wMilliseconds / 100;
        x = r1 * sin(theta) * cos(phi);
        z = r1 * sin(theta) * sin(phi);
        y = r1 * cos(theta);
 
        //if(z > 0)
        if (i == 0)
          MoveToEx(hDC, xc, yc + r1 * cos(theta), NULL);
        else 
            LineTo(hDC, xc + (INT)x, yc + y + z);
      }
    }

    for(i = 0; i <= N; i++)
    { 
      phi = i * 2.0 * MP2_PI / N + Time.wMilliseconds / 100; 
      MoveToEx(hDC, xc, yc + r1, NULL);
      for(j = 0; j <= M; j++)
      {  
        theta = j * MP2_PI / M;
        x = r1 * sin(theta) * cos(phi);
        z = r1 * sin(theta) * sin(phi);
        y = r1 * cos(theta);
             
          LineTo(hDC, xc + (INT)x, yc + y + z);
      }
    }
   
  DeleteObject(hPen);
}