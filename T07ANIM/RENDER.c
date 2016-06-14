/*Poroshin Mark 10-2 13.08.2015*/
/* FILE NAME: RENDER.c
 * PROGRAMMER: MP2
 * PURPOSE: working with matr  */

#include "ANIM.H"
#include <stdio.h>
MATR MP2_RndMatrWorld/* = MatrIdentity()*/,
     MP2_RndMatrView /* = MatrMulMatr(MatrIdentity, MatrTranslate(VecSet(-1, -1, 0)))*/,
     MP2_RndMatrProj;

FLT  MP2_RndProjDist  =   0.1,
     MP2_RndFarClip   =  100,
     MP2_RndProjSize  =   0.1;

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


/* Primitive free function.
 * ARGUMENTS:
 *   - primtive to free:
 *       vg4PRIM *Pr;
 * RETURNS: None.
 */
VOID MP2_RndPrimFree( mp2PRIM *Pr )
{
  if (Pr->p != NULL)
    free(Pr->p);
  if (Pr->Edges != NULL)
    free(Pr->Edges);
  memset(Pr, 0, sizeof(mp2PRIM));
} /* End of 'VG4_RndPrimFree' function */


/* Vertex representation type */


/* Load primitive from '*.g3d' file function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       vg4PRIM *Pr;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL MP2_RndPrimLoad( mp2PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  DWORD Sign;
  INT NumOfPrimitives;
  CHAR MtlFile[300];
  INT NumOfP;
  INT NumOfF;
  CHAR Mtl[300];
  mp2VERTEX V;
  INT p, N0, N1, N2;

  memset(Pr, 0, sizeof(mp2PRIM));

  F = fopen(FileName, "rb");
  if (F == NULL)
    return FALSE;

  /* File structure:
   *   4b Signature: "G3D\0"    CHAR Sign[4];
   *   4b NumOfPrimitives       INT NumOfPrimitives;
   *   300b material file name: CHAR MtlFile[300];
   *   repeated NumOfPrimitives times:
   *     4b INT NumOfP; - vertex count
   *     4b INT NumOfF; - facet (triangles) count
   *     300b material name: CHAR Mtl[300];
   *     repeat NumOfP times - vertices:
   *         !!! float point -> FLT
   *       typedef struct
   *       {
   *         VEC  P;  - Vertex position
   *         VEC2 T;  - Vertex texture coordinates
   *         VEC  N;  - Normal at vertex
   *         VEC4 C;  - Vertex color
   *       } VERTEX;
   *     repeat (NumOfF / 3) times - facets (triangles):
   *       INT N0, N1, N2; - for every triangle (N* - vertex number)
   */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }
  fread(&NumOfPrimitives, 4, 1, F);
  fread(MtlFile, 1, 300, F);
  for (p = 0; p < NumOfPrimitives; p++)
  {
    INT i;

    /* Read primitive info */
    fread(&NumOfP, 4, 1, F);
    fread(&NumOfF, 4, 1, F);
    fread(Mtl, 1, 300, F);

    /* Allocate memory for primitive */
    if ((Pr->p = malloc(sizeof(VEC) * NumOfP)) == NULL)
    {
      fclose(F);
      return FALSE;
    }
    if ((Pr->Edges = malloc(sizeof(INT [2]) * NumOfF)) == NULL)
    {
      free(Pr->p);
      Pr->p = NULL;
      fclose(F);
      return FALSE;
    }
    Pr->NumOfP = NumOfP;
    Pr->NumOfE = NumOfF;
    for (i = 0; i < NumOfP; i++)
    {
      fread(&V, sizeof(mp2VERTEX), 1, F);
      Pr->p[i] = V.P;
    }
    for (i = 0; i < NumOfF / 3; i++)
    {
      fread(&N0, 4, 1, F);
      fread(&N1, 4, 1, F);
      fread(&N2, 4, 1, F);
      Pr->Edges[i * 3 + 0][0] = N0;
      Pr->Edges[i * 3 + 0][1] = N1;
      Pr->Edges[i * 3 + 1][0] = N1;
      Pr->Edges[i * 3 + 1][1] = N2;
      Pr->Edges[i * 3 + 2][0] = N2;
      Pr->Edges[i * 3 + 2][1] = N0;
    }
    break;
  }
  fclose(F);
  return TRUE;
} /* End of 'VG4_RndPrimLoad' function */
