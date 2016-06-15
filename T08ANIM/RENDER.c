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

  /* Build transform matrix */
  M = MatrMulMatr(MP2_RndMatrWorld,
    MatrMulMatr(MP2_RndMatrView, MP2_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  /* Draw all lines */
  glBegin(GL_TRIANGLES);
  for (i = 0; i < Pr->NumOfI; i++)
  {
    glColor3fv(&Pr->V[Pr->I[i]].C.X);
    glVertex3fv(&Pr->V[Pr->I[i]].P.X);
  }
  glEnd();
} /* End of 'VG4_RndPrimDraw' function */


/* Primitive free function.
 * ARGUMENTS:
 *   - primtive to free:
 *       vg4PRIM *Pr;
 * RETURNS: None.
 */
VOID MP2_RndPrimFree( mp2PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  if (Pr->I != NULL)
    free(Pr->I);
  memset(Pr, 0, sizeof(mp2PRIM));
} /* End of 'VG4_RndPrimFree' function */


/* Vertex representation type */


/* Load primitive from '*.g3d' file function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       mp2PRIM *Pr;
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
  INT NumOfI;
  CHAR Mtl[300];
  INT p;

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
   *     4b INT NumOfI; - index (triangles * 3) count
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
    /* Read primitive info */
    fread(&NumOfP, 4, 1, F);
    fread(&NumOfI, 4, 1, F);
    fread(Mtl, 1, 300, F);

    /* Allocate memory for primitive */
    if ((Pr->V = malloc(sizeof(mp2VERTEX) * NumOfP)) == NULL)
    {
      fclose(F);
      return FALSE;
    }
    if ((Pr->I = malloc(sizeof(INT) * NumOfI)) == NULL)
    {
      free(Pr->V);
      Pr->V = NULL;
      fclose(F);
      return FALSE;
    }
    Pr->NumOfV = NumOfP;
    Pr->NumOfI = NumOfI;
    fread(Pr->V, sizeof(mp2VERTEX), NumOfP, F);
    fread(Pr->I, sizeof(INT), NumOfI, F);
    if (Pr->NumOfV > 0)
    {
      INT i;

      for (i = 0; i < Pr->NumOfV; i++)
        Pr->V[i].C = Vec4Set(Pr->V[i].N.X / 2 + 0.5,
                             Pr->V[i].N.Y / 2 + 0.5,
                             Pr->V[i].N.Z / 2 + 0.5, 1); /* Vec4Set(Rnd0(), Rnd0(), Rnd0(), 1); */
    }
    break;
  }
  fclose(F);
  return TRUE;
} /* End of 'VG4_RndPrimLoad' function */