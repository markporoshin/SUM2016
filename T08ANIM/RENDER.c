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

FLT  MP2_RndProjDist  =   0.1,
     MP2_RndFarClip   =  100,
     MP2_RndProjSize  =   0.1;

mp2MTL MP2_RndMaterials[MP2_MAX_MATERIALS];
INT MP2_RndNumOfMaterials;

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



VOID MP2_RndObjDraw( mp2OBJ *Obj )
{
  INT i;
  INT loc, mtl;
  MATR M, MSave;

  for (i = 0; i < Obj->NumOfPrims; i++)
  {
    /* Build transform matrix */
    MSave = MP2_RndMatrWorld;
    MP2_RndMatrWorld = MatrMulMatr(MP2_RndMatrWorld, Obj->Prims[i].M);
    M = MatrMulMatr(MP2_RndMatrWorld,
      MatrMulMatr(MP2_RndMatrView, MP2_RndMatrProj));
    glLoadMatrixf(M.A[0]);
    /*
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
    */

    glUseProgram(MP2_RndPrg);

    mtl = Obj->Prims[i].MtlNo;
    if (mtl != -1)
    {
      if (MP2_RndMaterials[mtl].TexNo != 0)
      {
        glBindTexture(GL_TEXTURE_2D, MP2_RndMaterials[mtl].TexNo);
        if ((loc = glGetUniformLocation(MP2_RndPrg, "IsTexture")) != -1)
          glUniform1i(loc, 1);
      }
      else
      {
        if ((loc = glGetUniformLocation(MP2_RndPrg, "IsTexture")) != -1)
          glUniform1i(loc, 0);
      }
      if ((loc = glGetUniformLocation(MP2_RndPrg, "Ka")) != -1)
        glUniform3fv(loc, 1, &MP2_RndMaterials[mtl].Ka.X);
      if ((loc = glGetUniformLocation(MP2_RndPrg, "Kd")) != -1)
        glUniform3fv(loc, 1, &MP2_RndMaterials[mtl].Kd.X);
      if ((loc = glGetUniformLocation(MP2_RndPrg, "Ks")) != -1)
        glUniform3fv(loc, 1, &MP2_RndMaterials[mtl].Ks.X);
      if ((loc = glGetUniformLocation(MP2_RndPrg, "Ph")) != -1)
        glUniform1f(loc, MP2_RndMaterials[mtl].Ph);
      if ((loc = glGetUniformLocation(MP2_RndPrg, "Trans")) != -1)
        glUniform1f(loc, MP2_RndMaterials[mtl].Trans);
    }

    /* Setup global variables */
    if ((loc = glGetUniformLocation(MP2_RndPrg, "MatrWVP")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);
    if ((loc = glGetUniformLocation(MP2_RndPrg, "MatrWorld")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, MP2_RndMatrWorld.A[0]);
    if ((loc = glGetUniformLocation(MP2_RndPrg, "MatrView")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, MP2_RndMatrView.A[0]);
    if ((loc = glGetUniformLocation(MP2_RndPrg, "MatrProj")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, MP2_RndMatrProj.A[0]);
    if ((loc = glGetUniformLocation(MP2_RndPrg, "Time")) != -1)
      glUniform1f(loc, MP2_Anim.Time);
    if ((loc = glGetUniformLocation(MP2_RndPrg, "PartNo")) != -1)
      glUniform1i(loc, i);

    /* Activete primitive vertex array */
    glBindVertexArray(Obj->Prims[i].VA);
    /* Activete primitive index buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Obj->Prims[i].IBuf);
    /* Draw primitive */
    glDrawElements(GL_TRIANGLES, Obj->Prims[i].NumOfI, GL_UNSIGNED_INT, NULL);
    glUseProgram(0);
    MP2_RndMatrWorld = MSave;
  }
} /* End of 'MP2_RndObjDraw' function */




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
BOOL MP2_RndObjLoad( mp2OBJ *Obj, CHAR *FileName )
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
  INT k;

  memset(Obj, 0, sizeof(mp2OBJ));

  F = fopen(FileName, "rb");
  if (F == NULL)
    return FALSE;

  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }
  fread(&NumOfPrimitives, 4, 1, F);
  fread(MtlFile, 1, 300, F);
  MP2_RndLoadMaterials(MtlFile);
  /* Allocate mnemory for primitives */
  if ((Obj->Prims = malloc(sizeof(mp2PRIM) * NumOfPrimitives)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Obj->NumOfPrims = NumOfPrimitives;

  for (p = 0; p < NumOfPrimitives; p++)
  {
    /* Read primitive info */
    fread(&NumOfV, 4, 1, F);
    fread(&NumOfI, 4, 1, F);
    fread(Mtl, 1, 300, F);

    /* Allocate memory for primitive */
    if ((V = malloc(sizeof(mp2VERTEX) * NumOfV + sizeof(INT) * NumOfI)) == NULL)
    {
      while (p-- > 0)
      {
        glBindVertexArray(Obj->Prims[p].VA);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &Obj->Prims[p].VBuf);
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &Obj->Prims[p].VA);
        glDeleteBuffers(1, &Obj->Prims[p].IBuf);
      }
      free(Obj->Prims);
      memset(Obj, 0, sizeof(mp2OBJ));
      fclose(F);
      return FALSE;
    }
    I = (INT *)(V + NumOfV);
    Obj->Prims[p].NumOfI = NumOfI;
    Obj->Prims[p].M = MatrIdentity();
    fread(V, sizeof(mp2VERTEX), NumOfV, F);
    fread(I, sizeof(INT), NumOfI, F);

    /* Create OpenGL buffers */
    glGenVertexArrays(1, &Obj->Prims[p].VA);
    glGenBuffers(1, &Obj->Prims[p].VBuf);
    glGenBuffers(1, &Obj->Prims[p].IBuf);

    /* Activate vertex array */
    glBindVertexArray(Obj->Prims[p].VA);
    /* Activate vertex buffer */
    glBindBuffer(GL_ARRAY_BUFFER, Obj->Prims[p].VBuf);
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Obj->Prims[p].IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);

    /* Disable vertex array */
    glBindVertexArray(0);
    Obj->Prims[p].Mtl.TexNo = MP2_RndFindMaterial(Mtl);
    free(V);
  }
  fclose(F);
  return TRUE;
} /* End of 'MP2_RndObjLoad' function */


VOID MP2_RndObjFree( mp2OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfPrims; i++)
  {
    glBindVertexArray(Obj->Prims[i].VA);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &Obj->Prims[i].VBuf);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Obj->Prims[i].VA);
    glDeleteBuffers(1, &Obj->Prims[i].IBuf);
  }
  free(Obj->Prims);
  memset(Obj, 0, sizeof(mp2OBJ));
} /* End of 'MP2_RndObjFree' function */


INT MP2_RndFindMaterial( CHAR *Name )
{
  INT i;

  for (i = 0; i < MP2_RndNumOfMaterials; i++)
    if (strcmp(Name, MP2_RndMaterials[i].Name) == 0)
      return i;
  return -1;
} /* End of 'MP2_RndFindMaterial' function */

VOID MP2_RndLoadMaterials( CHAR *FileName )
{
  INT i, NumOfMaterials;
  UINT t;
  DWORD Sign;
  FILE *F;
  mp2MTL M;
  BYTE *Image;

  if ((F = fopen(FileName, "rb")) == NULL)
    return;
  /* Read and check file signature */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"GMT")
  {
    fclose(F);
    return;
  }

  /* Read all materials */
  fread(&NumOfMaterials, 4, 1, F);
  for (i = 0; i < NumOfMaterials; i++)
  {
    if (MP2_RndNumOfMaterials >= MP2_MAX_MATERIALS)
      break;

    /* Read illumination coefficients and texture parameters */
    fread(&M, sizeof(mp2MTL), 1, F);
    
    /* Read image */
    if (M.TexW != 0 && M.TexH != 0 && M.TexNo != 0)
    {
      /* Allocate memory for texture image */
      if ((Image = malloc(M.TexW * M.TexH * M.TexNo)) == NULL)
      {
        fclose(F);
        return;
      }
      fread(Image, M.TexNo, M.TexW * M.TexH, F);
      glGenTextures(1, &t);
      glBindTexture(GL_TEXTURE_2D, t);
      gluBuild2DMipmaps(GL_TEXTURE_2D, M.TexNo, M.TexW, M.TexH,
        M.TexNo == 3 ? GL_BGR_EXT : GL_BGRA, GL_UNSIGNED_BYTE, Image);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBindTexture(GL_TEXTURE_2D, 0);
      M.TexNo = t;
    }
    /* Add material to animation and OpenGL */
    MP2_RndMaterials[MP2_RndNumOfMaterials++] = M;
  }
  fclose(F);
} /* End of 'VG4_RndLoadMaterials' function */
/* END OF 'RENDER.C' FILE */