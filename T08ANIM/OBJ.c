/* FILE NAME: OBJ3D.C
 * PROGRAMMER: MP2
 * DATE: 18.06.2016
 * PURPOSE: Object handle functions.
 */

#include "anim.h"
#include <stdio.h>

/* Object create function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       mp2OBJ *Obj;
 * RETURNS: None.
 */
VOID MP2_RndObjCreate( mp2OBJ *Obj )
{
  memset(Obj, 0, sizeof(mp2OBJ));
  Obj->M = MatrIdentity();
} /* End of 'MP2_RndObjCreate' function */

/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       mp2OBJ *Obj;
 * RETURNS: None.
 */
VOID MP2_RndObjDraw( mp2OBJ *Obj )
{
  INT i;
  MATR MSave;

  MSave = MP2_RndMatrWorld;
  MP2_RndMatrWorld = MatrMulMatr(MP2_RndMatrWorld, Obj->M);
  for (i = 0; i < Obj->NumOfPrims; i++)
    MP2_RndPrimDraw(&Obj->Prims[i]);
  MP2_RndMatrWorld = MSave;
} /* End of 'MP2_RndObjDraw' function */

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       mp2OBJ *Obj;
 * RETURNS: None.
 */
VOID MP2_RndObjFree( mp2OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfPrims; i++)
    MP2_RndPrimFree(&Obj->Prims[i]);
  free(Obj->Prims);
  memset(Obj, 0, sizeof(mp2OBJ));
} /* End of 'MP2_RndObjFree' function */

/* Load object from '*.g3d' file function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       mp2OBJ *Obj;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL MP2_RndObjLoad( mp2OBJ *Obj, CHAR *FileName )
{
  FILE *F;
  DWORD Sign, size;
  INT NumOfPrimitives;
  CHAR MtlFile[300];
  INT NumOfV;
  INT NumOfI;
  CHAR Mtl[300];
  INT p;
  mp2VERTEX *V;
  INT *I;

  MP2_RndObjCreate(Obj);

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
   *         float point -> FLT
   *       typedef struct
   *       {
   *         VEC  P;  - Vertex position
   *         VEC2 T;  - Vertex texture coordinates
   *         VEC  N;  - Normal at vertex
   *         VEC4 C;  - Vertex color
   *       } mp2VERTEX;
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
  MP2_RndLoadMaterials(MtlFile);

  /* Allocate memory for primitives */
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
    size = sizeof(mp2VERTEX) * NumOfV + sizeof(INT) * NumOfI;
    if ((V = malloc(size)) == NULL)
    {
      while (p-- > 0)
        MP2_RndPrimFree(&Obj->Prims[p]);
      free(Obj->Prims);
      memset(Obj, 0, sizeof(mp2OBJ));
      fclose(F);
      return FALSE;
    }
    memset(V, 0, size);
    I = (INT *)(V + NumOfV);
    /* Read primitive data */
    fread(V, 1, size, F);

    MP2_RndPrimCreate(&Obj->Prims[p], V, NumOfV, I, NumOfI);
    Obj->Prims[p].MtlNo = MP2_RndFindMaterial(Mtl);
    Obj->Prims[p].Id = p;

    free(V);
  }
  fclose(F);
  return TRUE;
} /* End of 'MP2_RndObjLoad' function */
/* END OF 'OBJ3D.C' FILE */