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
 
  MP2_RndObjLoad( &Uni->Obj, "Cruiser.g3d" );
} /* End of 'MP2_UnitInit' function */

static VOID MP2_UnitRender( mp2MODEL *Uni, mp2ANIM *Ani )
{
  glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(1, 0, 0, 0);
    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 1, 0, 0);
    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 0, 1, 0);
  glEnd();
  MP2_RndMatrWorld = MatrMulMatr(MP2_RndMatrWorld, MatrScale(VecSet(0.003, 0.003, 0.003)));
  MP2_RndObjDraw( &Uni->Obj );
}

static VOID MP2_UnitClose( mp2MODEL *Uni, mp2ANIM *Ani )
{
  MP2_RndObjFree( &Uni->Obj );
}


static VOID MP2_UnitResponse( mp2MODEL *Uni, mp2ANIM *Ani )
{
  
}

mp2UNIT * MP2_UnitCreateSTATICMODEL( FLT x, FLT y, FLT z )
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