#include "ANIM.H"

VOID MP2_RndPrimDraw( mp2PRIM *Pr );
typedef struct
{
  mp2UNIT;
  VEC Pos;

}mp2CUBE;
VEC CubeP[] =
{
  {-1, -1,  1},
  { 1, -1,  1},
  { 1, -1, -1},
  {-1, -1, -1},
  {-1,  1,  1},
  { 1,  1,  1},
  { 1,  1, -1},
  {-1,  1, -1}
};

INT CubeE[][2] =
{
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
};
mp2PRIM Cube =
{
  CubeP, sizeof(CubeP) / sizeof(CubeP[0]),
  CubeE, sizeof(CubeE) / sizeof(CubeE[0])
};

static VOID MP2_UnitInit( mp2PRIM *Uni, mp2ANIM *Ani )
{
   
} /* End of 'MP2_UnitInit' function */

static VOID MP2_UnitRender( mp2PRIM *Uni, mp2ANIM *Ani )
{
  MP2_RndPrimDraw( &Cube );
}

mp2UNIT * MP2_UnitCreateCube( DBL x, DBL y, DBL z )
{
  mp2CUBE *Uni;

  if ((Uni = (mp2CUBE *)MP2_AnimUnitCreate( sizeof(mp2CUBE) )) == NULL)
    return NULL;
  Uni->Pos = VecSet(x, y, z); 
  /* Setup unit methods */
  Uni->Init = (VOID *)MP2_UnitInit;
  Uni->Render = (VOID *)MP2_UnitRender;
  
  return (mp2UNIT *)Uni;
} /* End of 'MP2_UnitCreateBall' function */


/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *       mp2PRIM *Pr;
 * RETURNS: None.
 */
VOID MP2_RndPrimDraw( mp2PRIM *Pr )
{
  INT i;
  MATR M;
  POINT *pts;

  /* Build transform matrix */
  M = MatrMulMatr(MP2_RndMatrWorld,
    MatrMulMatr(MP2_RndMatrView, MP2_RndMatrProj));

  /* Transform all points */
  pts = malloc(sizeof(POINT) * Pr->NumOfP);
  if (pts == NULL)
    return;
  for (i = 0; i < Pr->NumOfP; i++)
  {
    /* NDC */
    VEC p = PointTransform4(Pr->p[i], M);

    pts[i].x = (p.X + 1) * MP2_Anim.W / 2;
    pts[i].y = (-p.Y + 1) * MP2_Anim.H / 2;
  }

  /* Draw all lines */
  for (i = 0; i < Pr->NumOfE; i++)
  {
    INT n0 = Pr->Edges[i][0], n1 = Pr->Edges[i][1];
    SelectObject(MP2_Anim.hDC, GetStockObject(DC_PEN));
    SetDCPenColor(MP2_Anim.hDC, RGB(i * 12, i * 255 / Pr->NumOfE, 255 - (i) * 255 / Pr->NumOfE));
    MoveToEx(MP2_Anim.hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(MP2_Anim.hDC, pts[n1].x, pts[n1].y);
  }
  free(pts);
} /* End of 'MP2_RndPrimDraw' function */
