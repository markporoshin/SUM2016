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
  INT loc;
  MATR M;

  /* Build transform matrix */
  M = MatrMulMatr(MP2_RndMatrWorld,
    MatrMulMatr(MP2_RndMatrView, MP2_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  glUseProgram(MP2_RndPrg);

  /* Setup global variables */
  if ((loc = glGetUniformLocation(MP2_RndPrg, "MatrWorld")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MP2_RndMatrWorld.A[0]);
  if ((loc = glGetUniformLocation(MP2_RndPrg, "MatrView")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MP2_RndMatrView.A[0]);
  if ((loc = glGetUniformLocation(MP2_RndPrg, "MatrProj")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MP2_RndMatrProj.A[0]);
  if ((loc = glGetUniformLocation(MP2_RndPrg, "Time")) != -1)
    glUniform1f(loc, MP2_Anim.Time);


  /* Activete primitive vertex array */
  glBindVertexArray(Pr->VA);
  /* Activete primitive index buffer */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  /* Draw primitive */
  glDrawElements(GL_TRIANGLES, Pr->NumOfI, GL_UNSIGNED_INT, NULL);
  glUseProgram(0);
} /* End of 'VG4_RndPrimDraw' function */


VOID MP2_RndPrimFree( mp2PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->IBuf);
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
  INT NumOfV;
  INT NumOfI;
  CHAR Mtl[300];
  INT p;
  mp2VERTEX *V;
  INT *I;

  memset(Pr, 0, sizeof(mp2PRIM));

  F = fopen(FileName, "rb");
  if (F == NULL)
    return FALSE;

  /* File structure:
   *   4b Signature: "G3D\0"    CHAR Sign[4];
   *   4b NumOfPrimitives       INT NumOfPrimitives;
   *   300b material file name: CHAR MtlFile[300];
   *   repeated NumOfPrimitives times:
   *     4b INT NumOfV; - vertex count
   *     4b INT NumOfI; - index (triangles * 3) count
   *     300b material name: CHAR Mtl[300];
   *     repeat NumOfV times - vertices:
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
    fread(&NumOfV, 4, 1, F);
    fread(&NumOfI, 4, 1, F);
    fread(Mtl, 1, 300, F);

    /* Allocate memory for primitive */
    if ((V = malloc(sizeof(mp2VERTEX) * NumOfV)) == NULL)
    {
      fclose(F);
      return FALSE;
    }
    if ((I = malloc(sizeof(INT) * NumOfI)) == NULL)
    {
      free(V);
      V = NULL;
      fclose(F);
      return FALSE;
    }
    Pr->NumOfI = NumOfI;
    fread(V, sizeof(mp2VERTEX), NumOfV, F);
    fread(I, sizeof(INT), NumOfI, F);

    /* Create OpenGL buffers */
    glGenVertexArrays(1, &Pr->VA);
    glGenBuffers(1, &Pr->VBuf);
    glGenBuffers(1, &Pr->IBuf);

    /* Activate vertex array */
    glBindVertexArray(Pr->VA);
    /* Activate vertex buffer */
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    /* Store vertex data */
    glBufferData(GL_ARRAY_BUFFER, sizeof(mp2VERTEX) * NumOfV, V, GL_STATIC_DRAW);

    /* Setup data order */
    /*                    layout,
     *                      components count,
     *                          type
     *                                    should be normalize,
     *                                           vertex structure size in bytes (stride),
     *                                               offset in bytes to field start */
    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(mp2VERTEX),
                          (VOID *)0); /* position */
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(mp2VERTEX),
                          (VOID *)sizeof(VEC)); /* texture coordinates */
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(mp2VERTEX),
                          (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normal */
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(mp2VERTEX),
                          (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color */

    /* Enable used attributes */
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    /* Indices */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);

    /* Disable vertex array */
    glBindVertexArray(0);

    free(V);
    free(I);
    break;
  }
  fclose(F);
  return TRUE;
} /* End of 'MP2_RndPrimLoad' function */

/* END OF 'LOADPRIM.C' FILE */