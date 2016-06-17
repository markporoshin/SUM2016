#include "ANIM.H"

VOID MP2_RndPrimDraw( mp2PRIM *Pr );
typedef struct
{
  mp2UNIT;
  VEC Pos;
  mp2OBJ Obj;
}mp2MODEL;


static VOID MP2_UnitInit( mp2MODEL *Uni, mp2ANIM *Ani )
{  
 
  MP2_RndObjLoad( &Uni->Obj, "boat_fishing_01.g3d" );
} /* End of 'MP2_UnitInit' function */

static VOID MP2_UnitRender( mp2MODEL *Uni, mp2ANIM *Ani )
{ 
  static DBL angle;
  MP2_RndMatrWorld = MatrMulMatr(MP2_RndMatrWorld, MatrTranslate(VecSet(0, 100, 0)));
  if (Ani->JZ != 0 && Ani->JR != 0)
  {
    angle = asin(Ani->JR / sqrt(Ani->JR * Ani->JR + Ani->JZ * Ani->JZ));
    if (Ani->JZ < 0)
      ;/*its okey*/
    if (Ani->JZ > 0)
      angle = 3 * PI - angle;
    if (Ani->JZ == 1)
      angle == PI;
  }
  //MP2_RndMatrWorld = MatrMulMatr(MP2_RndMatrWorld, MatrRotateY(90 ));
  MP2_RndMatrWorld = MatrMulMatr(MP2_RndMatrWorld, MatrRotateY(angle + 270 + 90));
  MP2_RndMatrWorld = MatrMulMatr(MP2_RndMatrWorld, MatrScale(VecSet(0.030, 0.030, 0.030)));
  MP2_RndMatrWorld = MatrMulMatr(MP2_RndMatrWorld, MatrTranslate(View));
  
  MP2_RndObjDraw( &Uni->Obj );
}

static VOID MP2_UnitClose( mp2MODEL *Uni, mp2ANIM *Ani )
{
  MP2_RndObjFree( &Uni->Obj );
}


static VOID MP2_UnitResponse( mp2MODEL *Uni, mp2ANIM *Ani )
{
  
}

mp2UNIT * MP2_UnitCreateCube( FLT x, FLT y, FLT z )
{
  mp2MODEL *Uni;

  if ((Uni = (mp2MODEL *)MP2_AnimUnitCreate( sizeof(mp2MODEL) )) == NULL)
    return NULL;
  Uni->Pos = VecSet(x, y, z); 
  /* Setup unit methods */
  Uni->Init = (VOID *)MP2_UnitInit;
  Uni->Render = (VOID *)MP2_UnitRender;
  Uni->Response = (VOID *)MP2_UnitResponse;
  Uni->Close = (VOID *)MP2_UnitClose;
  return (mp2UNIT *)Uni;
} /* End of 'MP2_UnitCreateBall' function */






