
#include "ANIM.H"

VEC View = {0, 0, 0};

VOID MP2_RndPrimDraw( mp2PRIM *Pr );
typedef struct
{
  mp2UNIT;       
  VEC Pos;
}mp2CONTROL;


static VOID MP2_UnitInit( mp2CONTROL *Uni, mp2ANIM *Ani )
{   
  Uni->Pos = VecSet(2, 2, 2);
} /* End of 'MP2_UnitInit' function */

static VOID MP2_UnitRender( mp2CONTROL *Uni, mp2ANIM *Ani )
{

}

static VOID MP2_UnitResponse( mp2CONTROL *Uni, mp2ANIM *Ani )
{
  DBL r;
  if (Ani->Keys['T'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (Ani->Keys['Y'])
    glPolygonMode(GL_FRONT, GL_LINE);
  if (Ani->Keys['U'])
    glPolygonMode(GL_BACK, GL_LINE);
  /*if (Ani->Keys[VK_SPACE])
    MP2_AnimAddUnit(MP2_UnitCreateBall()); */
  if (Ani->KeysClick['C'])
    MP2_AnimAddUnit(MP2_UnitCreateCube(/*Rnd1() * 4, Rnd1() * 4, Rnd1() * 4*/0, 0, 0));
  if (Ani->KeysClick['V'])
    MP2_AnimAddUnit(MP2_UnitCreateSTATICMODEL(/*Rnd1() * 4, Rnd1() * 4, Rnd1() * 4*/0, 0, 0));
  if (Ani->KeysClick[VK_RETURN] && Ani->Keys[VK_MENU])
    MP2_FlipFullScreen(MP2_Anim.hWnd);
  /*if (Ani->KeysClick[VK_ESCAPE])
    MP2_AnimDoExit();*/
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  /* Uni->Pos.Y += Ani->JY * Ani->GlobalDeltaTime; */
  Uni->Pos = PointTransform(Uni->Pos, MatrRotateX(10 * Ani->JY * Ani->GlobalDeltaTime));
  Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(10 * Ani->JX * Ani->GlobalDeltaTime));
   

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(10 * Ani->Mdx * Ani->GlobalDeltaTime));
    Uni->Pos = PointTransform(Uni->Pos, MatrRotateX(10 * Ani->Mdy * Ani->GlobalDeltaTime));
  }
  View.X -= Ani->JZ / 10;
  View.Z += Ani->JR / 10;
  Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(10 * Ani->Keys[VK_RIGHT] * Ani->GlobalDeltaTime));
  Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(-10 * Ani->Keys[VK_LEFT] * Ani->GlobalDeltaTime));

  r = VecLen(Uni->Pos);
  Uni->Pos = VecMulNum(Uni->Pos, (r + (-Ani->Mdz) * Ani->DeltaTime * 1.0) / r);
  
  MP2_RndMatrView = MatrView(VecAddVec(Uni->Pos, View), View, VecSet(0, 1, 0));
}

mp2UNIT * MP2_UnitCreateControl( FLT x, FLT y, FLT z )
{
  mp2CONTROL *Uni;

  if ((Uni = (mp2CONTROL *)MP2_AnimUnitCreate( sizeof(mp2CONTROL) )) == NULL)
    return NULL;
  Uni->Pos = VecSet(x, y, z); 
  /* Setup unit methods */
  Uni->Init = (VOID *)MP2_UnitInit;
  Uni->Render = (VOID *)MP2_UnitRender;
  Uni->Response = (VOID *)MP2_UnitResponse;
  
  return (mp2UNIT *)Uni;
} /* End of 'SA2_UnitCreateCONTROL' function */
