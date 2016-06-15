#include "ANIM.H"

VOID MP2_RndPrimDraw( mp2PRIM *Pr );
typedef struct
{
  mp2UNIT;
  VEC Pos;
  mp2PRIM p;
}mp2CUBE;


static VOID MP2_UnitInit( mp2CUBE *Uni, mp2ANIM *Ani )
{  
 
  MP2_RndPrimLoad( &Uni->p, "cow.g3d" ); 
} /* End of 'MP2_UnitInit' function */

static VOID MP2_UnitRender( mp2CUBE *Uni, mp2ANIM *Ani )
{
  MP2_RndMatrWorld = MatrMulMatr(MP2_RndMatrWorld, MatrTranslate(Uni->Pos));
  MP2_RndPrimDraw( &Uni->p );
}

static VOID MP2_UnitClose( mp2CUBE *Uni, mp2ANIM *Ani )
{
  MP2_RndPrimFree( &Uni->p );
}


static VOID MP2_UnitResponse( mp2CUBE *Uni, mp2ANIM *Ani )
{
  
}

mp2UNIT * MP2_UnitCreateCube( FLT x, FLT y, FLT z )
{
  mp2CUBE *Uni;

  if ((Uni = (mp2CUBE *)MP2_AnimUnitCreate( sizeof(mp2CUBE) )) == NULL)
    return NULL;
  Uni->Pos = VecSet(x, y, z); 
  /* Setup unit methods */
  Uni->Init = (VOID *)MP2_UnitInit;
  Uni->Render = (VOID *)MP2_UnitRender;
  Uni->Response = (VOID *)MP2_UnitResponse;
  Uni->Close = (VOID *)MP2_UnitClose;
  return (mp2UNIT *)Uni;
} /* End of 'MP2_UnitCreateBall' function */


/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *       mp2PRIM *Pr;
 * RETURNS: None.
 */



