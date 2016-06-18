/*Poroshin Mark 10-2 13.08.2015*/
/* FILE NAME: RENDER.c
 * PROGRAMMER: MP2
 * PURPOSE: working with matr  */

#include "ANIM.H"
#include <stdio.h>
#include <string.h>
MATR MP2_RndMatrWorld/* = MatrIdentity()*/,
     MP2_RndMatrView /* = MatrMulMatr(MatrIdentity, MatrTranslate(VecSet(-1, -1, 0)))*/,
     MP2_RndMatrProj;

FLT  MP2_RndProjDist  =   1 ,
     MP2_RndFarClip   =  5000,
     MP2_RndProjSize  =   1;

UINT MP2_RndPrg;

VOID MP2_RndProj( VOID )
{
  FLT ratio_x = 1, ratio_y = 1;
  if (MP2_Anim.W >= MP2_Anim.H)
    ratio_x = (FLT)MP2_Anim.W / MP2_Anim.H;
  else
    ratio_y = (FLT)MP2_Anim.H / MP2_Anim.W;
  MP2_RndMatrProj = MatrFrustum(-ratio_x * MP2_RndProjSize / 2,
                                 ratio_x * MP2_RndProjSize / 2,
                                -ratio_y * MP2_RndProjSize / 2,
                                 ratio_y * MP2_RndProjSize / 2,
                                 MP2_RndProjDist, MP2_RndFarClip);
}






/* END OF 'RENDER.C' FILE */