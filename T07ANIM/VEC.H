/*Poroshin Mark 10-2 07.08.2015*/
/* FILE NAME: VEC.H
 * PROGRAMMER: MP2
 * PURPOSE: VECTOR & MATR lib  */

//#include "def.H"
#include <math.h>
#include <windows.h>
#define PI 3.14159265358979323846
#define D2R(A) ((A) * PI / 180.0)

/* базовый вещественный тип */
typedef double DBL;

/* type for VEC in space */
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

/* type for MART  - mas in struct */
typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

/* begin of function "VecSet" */
__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC r = {X, Y, Z};

  return r;
}
/* end of function "VecSet"*/

/* begin of function "VecAddVec" */
__inline VEC VecAddVec( VEC A, VEC B )
{
  return VecSet(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}
/* end of function "VecAddVec" */

/* begin of function "VecAddVec" */
__inline VEC VecSubVec( VEC A, VEC B )
{
    return VecSet(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}
/* end of function "VecAddVec" */

/* begin of function "VecMulNum" */
__inline VEC  VecMulNum( VEC A, DBL N )
{
    return VecSet(A.X * N, A.Y * N, A.Z * N);
}
/* end of function "VecMulNum" */

/* begin of function "VecMulNum" */
__inline VEC VecDivNum( VEC A, DBL N )
{
    return VecSet(A.X / N, A.Y / N, A.Z / N);
}
/* end of function "VecMulNum" */

/* begin of function "VecNeg" */
__inline VEC VecNeg( VEC A )
{
  return VecSet(-A.X, -A.Y, -A.Z);
}
/* end of function "VecNeg" */

/* begin of function "VecDotVec" */
__inline DBL VecDotVec( VEC A, VEC B )
{
  return A.X * B.X + A.Y * B.Y + A.Z * B.Z; 
}
/* end of function "VecDotVec" */

/* begin of function "VecCrossVec" */
__inline VEC VecCrossVec( VEC A, VEC B )
{
   VEC V;
   V.X = A.Y * B.Z - A.Z * B.Y;
   V.Y = A.Z * B.X - A.X * B.Z;
   V.Z = A.X * B.Y - A.Y * B.X;
   return V;
}
/* end of function "VecCrossVec" */

/* begin of function "VecLen2" */
__inline DBL VecLen2( VEC V )
{
   return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}
/* end of function "VecLen2" */

/* begin of function "VecLen" */
__inline DBL VecLen( VEC V )
{
   return sqrt(V.X * V.X + V.Y * V.Y + V.Z * V.Z);
}
/* end of function "VecLen" */

/* begin of function "VecNormalize" */
__inline VEC VecNormalize( VEC V )
{
  DBL len = VecDotVec(V, V);

  if (len != 1 && len != 0)
    len = sqrt(len), V.X /= len, V.Y /= len, V.Z /= len;
  return V;
}
/* end of function "VecNormalize" */

/* begin of fuction "PointTransform" */
__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
}
/* begin of fuction "PointTransform" */

/* begin of fuction "PointTransform4" */
__inline VEC PointTransform4( VEC V, MATR M )
{
  DBL w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet(
    (V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
    (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
    (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
}
/* end of fuction "PointTransform4" */

__inline MATR MatrTranslate( VEC D )
{
  MATR M =
  {
    {
      {  1,   0,   0,  0},
      {  0,   1,   0,  0},
      {  0,   0,   1,  0},
      {D.X, D.Y, D.Z,  1},
    }
  };

  return M;
}


__inline MATR MatrScale( VEC S )
{
  MATR M = 
  {
    {
      {S.X,   0,   0,   0},
      {0,     S.Y, 0,   0},
      {0,     0,   S.Z, 0},
      {0,     0,   0,   1}
    }
   };
  return M;
}

__inline MATR MatrRotateX( DBL AngleDegree )
{
  MATR M =
  {
    {
      {1,                0,                0, 0},
      {0, cos(AngleDegree), sin(AngleDegree), 0},
      {0,-sin(AngleDegree), cos(AngleDegree), 0},
      {0,                0,                0, 1}
    }
  };
  return M;
}
__inline MATR MatrRotateY( DBL AngleDegree )
{
  MATR M =
  {
    {
      { cos(AngleDegree),  0, -sin(AngleDegree), 0},
      {                0,  1,                 0, 0},
      { sin(AngleDegree),  0,  cos(AngleDegree), 0},
      {                0,  0,                 0, 1}
    }
  };
  return M;
}
__inline MATR MatrRotateZ( DBL AngleDegree )
{
  MATR M =
  {
    {
      { cos(AngleDegree),-sin(AngleDegree),   0, 0},
      { sin(AngleDegree), cos(AngleDegree),   0, 0},
      {                0,                0,   1, 0},
      {                0,                0,   0, 1}
    }
  };
   return M;
}
__inline MATR MatrRotate( DBL AngleDegree, VEC R )
{
  DBL A = D2R(AngleDegree), si = sin(A), co = cos(A);
  VEC V = VecNormalize(R);
  MATR M =
  {
    {
      {co + V.X * V.X * (1 - co),        V.X * V.Y * (1 - co) - V.Z * si,  V.X * V.Z * (1 - co) + V.Y * si, 0},
      {V.Y * V.X * (1 - co) + V.Z * si,  co + V.Y * V.Y * (1 - co),        V.Y * V.Z * (1 - co) - V.X * si, 0},
      {V.Z * V.X * (1 - co) - V.Y * si,  V.Z * V.Y * (1 - co) + V.X * si,  co + V.Z * V.Z * (1 - co),       0},
      {0,                                0,                                0,                               1}
    }
  };

  return M;
}

__inline MATR MatrIdentity( VOID )
{
  MATR M =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };
  return M;
}

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r;
  INT i, j, k;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0, r.A[i][j] = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
}
__inline MATR MatrTranspose( MATR M )
{
  int i, j;
  MATR M0;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
    {
      M0.A[i][j] = M.A[j][i];
    }
    return M0;
}

__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}

__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
}

__inline MATR MatrInverse( MATR M )
{  
  DBL det = MatrDeterm(M);
  MATR r;
  INT i, j, p[4][3] =
  {
    {1, 2, 3},
    {0, 2, 3},
    {0, 1, 3},
    {0, 1, 2}
  };

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[j][i] =
        (1 - (i + j) % 2 * 2) * 
        MatrDeterm3x3(
          M.A[p[i][0]][p[j][0]], M.A[p[i][0]][p[j][1]], M.A[p[i][0]][p[j][2]],
          M.A[p[i][1]][p[j][0]], M.A[p[i][1]][p[j][1]], M.A[p[i][1]][p[j][2]],
          M.A[p[i][2]][p[j][0]], M.A[p[i][2]][p[j][1]], M.A[p[i][2]][p[j][2]]) / det;
  return r;
}

__inline MATR MatrFrustum( DBL l, DBL r, DBL b, DBL t, DBL n, DBL f )
{
  MATR m =
  {
    {
      {    2*n/(r-l),             0,              0,   0  },
      {            0,     2*n/(t-b),              0,   0  },
      {  (r+l)/(r-l),   (t+b)/(t-b),   -(f+n)/(f-n),  -1  },
      {            0,             0,   -2*n*f/(f-n),   0  }
    }
  };
  return m;
}