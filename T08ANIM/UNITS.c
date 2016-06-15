/*Poroshin Mark 10-2 10.08.2015*/
/* FILE NAME: UNITS.c
 * PROGRAMMER: MP2
 * PURPOSE: static Unit function   */

#include "ANIM.H"





/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID MP2_UnitInit( mp2UNIT *Uni, mp2ANIM *Ani )
{
} /* End of 'MP2_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID MP2_UnitClose( mp2UNIT *Uni, mp2ANIM *Ani )
{
} /* End of 'MP2_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID MP2_UnitResponse( mp2UNIT *Uni, mp2ANIM *Ani )
{
} /* End of 'VG4_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID MP2_UnitRender( mp2UNIT *Uni, mp2ANIM *Ani )
{
} /* End of 'MP2_UnitRender' function */

/*- основная функция создания объекта анимации по умолчанию:*/

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
mp2UNIT * MP2_AnimUnitCreate( INT Size )
{
  mp2UNIT *Uni;
  /* Memory allocation */
  if (Size < sizeof(mp2UNIT) ||
      (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = MP2_UnitInit;
  Uni->Close = MP2_UnitClose;
  Uni->Response = MP2_UnitResponse;
  Uni->Render = MP2_UnitRender;
  return Uni;
} /* End of 'MP2_AnimUnitCreate' function */