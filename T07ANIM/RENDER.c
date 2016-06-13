/*Poroshin Mark 10-2 13.08.2015*/
/* FILE NAME: RENDER.c
 * PROGRAMMER: MP2
 * PURPOSE: working with matr  */

#include "ANIM.H"

MATR MP2_RndMatrWorld/* = MatrIdentity()*/,
     MP2_RndMatrView /* = MatrMulMatr(MatrIdentity, MatrTranslate(VecSet(-1, -1, 0)))*/,
     MP2_RndMatrProj;

DBL  MP2_RndProjDist  =   0.1,
     MP2_RndFarClip   =  100,
     MP2_RndProjSize  =   0.1;

VOID MP2_RndProj( VOID )
{
  DBL ratio_x = 1, ratio_y = 1;
  if (MP2_Anim.W >= MP2_Anim.H)
    ratio_x = (DBL)MP2_Anim.W / MP2_Anim.H;
  else
    ratio_y = (DBL)MP2_Anim.H / MP2_Anim.W;
  MP2_RndMatrProj = MatrFrustum(-ratio_x * MP2_RndProjSize / 2,
                                 ratio_x * MP2_RndProjSize / 2,
                                -ratio_y * MP2_RndProjSize / 2,
                                 ratio_y * MP2_RndProjSize / 2,
                                 MP2_RndProjDist, MP2_RndFarClip);
}

MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC   Dir   = VecNormalize(VecSubVec(At, Loc)),
        Right = VecNormalize(VecCrossVec(Dir, Up1)),
        Up    = VecNormalize(VecCrossVec(Right, Dir));

  MATR m = 
  {
    {
      {               Right.X,                Up.X,              -Dir.X, 0},
      {               Right.Y,                Up.Y,              -Dir.Y, 0},
      {               Right.Z,                Up.Z,              -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };
  return m;
}
