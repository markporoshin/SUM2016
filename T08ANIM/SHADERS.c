/* FILE NAME: SHADERS.C
 * PROGRAMMER: MP2
 * DATE: 13.06.2016
 * PURPOSE: Render handle functions.
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"

/* Save text to log file function.
 * ARGUMENTS:
 *   - text 1 to save:
 *       CHAR *Stage;
 *   - text 2 to save:
 *       CHAR *Text;
 * RETURNS: None.
 */
static VOID MP2_RndShaderLog( CHAR *Stage, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("MP2{SHAD}30.LOG", "a")) != NULL)
  {
    fprintf(F, "%s: %s\n", Stage, Text);
    fclose(F);
  }
} /* End of 'VG4_RndShaderLoadTextFile' function */

/* Text file load to memory function.
 * ARGUMENTS:
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text or NULL if error is occured.
 */
static CHAR * MP2_RndShaderLoadTextFile( CHAR *FileName )
{
  FILE *F;
  CHAR *text;
  INT flen;

  if ((F = fopen(FileName, "r")) == NULL)
    return NULL;

  /* Obtain file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);

  /* Allocate memory */
  if ((text = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  memset(text, 0, flen + 1);

  /* Read text */
  fseek(F, 0, SEEK_SET);
  fread(text, 1, flen, F);

  fclose(F);
  return text;
} /* End of 'MP2_RndShaderLoadTextFile' function */

/* Shader program load function.
 * ARGUMENTS:
 *   - shader files prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (UINT) shader program index or 0 if error is occured.
 */
UINT MP2_RndShaderLoad( CHAR *FileNamePrefix )
{
  INT i, res;
  CHAR *txt;
  CHAR *suff[] = {"VERT", "FRAG"};
  INT NS = sizeof(suff) / sizeof(suff[0]);
  CHAR buf[500];
  INT shd_type[2] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
  UINT
    prg = 0,
    shd[2] = {0};
  BOOL isok = TRUE;

  /* Load shaders */
  for (i = 0; isok && i < NS; i++)
  {
    /* Create shader */
    if ((shd[i] = glCreateShader(shd_type[i])) == 0)
    {
      MP2_RndShaderLog(suff[i], "Error create shader");
      isok = FALSE;
      break;
    }
    /* Load text file */
    sprintf(buf, "%s.%s", FileNamePrefix, suff[i]);
    txt = MP2_RndShaderLoadTextFile(buf);
    if (txt == NULL)
    {
      MP2_RndShaderLog(suff[i], "No file");
      isok = FALSE;
      break;
    }

    /* Attach text to shader  */
    glShaderSource(shd[i], 1, &txt, NULL);
    free(txt);

    /* Compile shader */
    glCompileShader(shd[i]);
    glGetShaderiv(shd[i], GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shd[i], sizeof(buf), &res, buf);
      MP2_RndShaderLog(suff[i], buf);
      isok = FALSE;
      break;
    }
  }

  if (isok)
    if ((prg = glCreateProgram()) == 0)
      isok = FALSE;
    else
    {
      /* Attach shaders to program */
      for (i = 0; i < NS; i++)
        glAttachShader(prg, shd[i]);
      /* Link program */
      glLinkProgram(prg);
      glGetProgramiv(shd[i], GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(buf), &res, buf);
        MP2_RndShaderLog("LINK", buf);
        isok = FALSE;
      }
    }

  if (!isok)
  {
    for (i = 0; i < NS; i++)
      if (shd[i] != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shd[i]);
        glDeleteShader(shd[i]);
      }
    if (prg != 0)
      glDeleteProgram(prg);
  }
  return prg;
} /* End of 'MP2_RndShaderLoad' function */

/* Shader program load function.
 * ARGUMENTS:
 *   - shader program Id:
 *       UINT Prg;
 * RETURNS: None.
 */
VOID MP2_RndShaderFree( UINT Prg )
{
  UINT i, n, shds[5];

  if (Prg == 0)
    return;

  /* Obtain program shaders count */
  glGetAttachedShaders(Prg, 5, &n, shds);

  for (i = 0; i < n; i++)
  {
    glDetachShader(Prg, shds[i]);
    glDeleteShader(shds[i]);
  }
  glDeleteProgram(Prg);
} /* End of 'MP2_RndShaderFree' function */

/* END OF 'SHADERS.C' FILE */