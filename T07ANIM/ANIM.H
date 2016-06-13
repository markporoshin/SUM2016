/*Poroshin Mark 10-2 10.08.2015*/
/* FILE NAME: ANIM.H
 * PROGRAMMER: MP2
 * PURPOSE: main header of project    */

#include "def.H"
#include "UNITS.H"
#include "IMAGE.h"
#define MP2_MAX_UNITS 100

  /* Timer data */



typedef struct tagmp2UNIT mp2UNIT;
typedef struct
{
  CREATESTRUCT *cs;
  HWND hWnd; //- окно
  HDC hDC;   //- контекст в памяти
  INT W, H;  //- размер окна
  HBITMAP hFrame; //- картинка кадра
  mp2UNIT *Units[MP2_MAX_UNITS]; //- массив объектов анимации
  INT NumOfUnits; //- текущее количество объектов анимации
  /*** Timer ***/
  DBL GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
      Time, DeltaTime,             /* Time with pause and interframe interval */
      FPS;                         /* Frames per seond value */
  BOOL IsPause;                     /* Pause flag */

    /***  Mouse   ***/
  INT Mdx, Mdy, Mdz,
      Mx,  My,  Mz;

  /*** KeyBoard ***/
  BYTE Keys[256], OldKeys[256], KeysClick[256];

  /*** JoyStick ***/
  BYTE JBut[32];                    /* Joystick button state */
  INT JPov;                      /* Joystick point-of-view control [0,1..8] */
  DBL JX, JY, JZ, JR;              /* Joystick axes */
} mp2ANIM;




struct tagmp2UNIT
{
  /* Unit initialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (* Init)( mp2UNIT *Uni, mp2ANIM *Ani );

  /* Unit deinitialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (* Close)( mp2UNIT *Uni, mp2ANIM *Ani );

  /* Unit inter frame events handle function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (* Response)( mp2UNIT *Uni, mp2ANIM *Ani );

  /* Unit render function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (* Render)( mp2UNIT *Uni, mp2ANIM *Ani );
};


typedef struct
{
  VEC *p;
  INT NumOfP;
  INT(*Edges)[2];
  INT NumOfE;
}mp2PRIM;

extern MATR MP2_RndMatrWorld/* = MatrIdentity()*/,
            MP2_RndMatrView /* = MatrMulMatr(MatrIdentity, MatrTranslate(VecSet(-1, -1, 0)))*/,
            MP2_RndMatrProj;
extern DBL  MP2_RndProjDist,
     MP2_RndFarClip,
     MP2_RndProjSize;
extern INT MP2_MOUSEWHEEL;
extern mp2ANIM MP2_Anim;
VOID MP2_AnimInit( HWND hWnd );
VOID MP2_AnimResize( INT w,INT h );
VOID MP2_AnimCopyFrame( HDC hDC);
VOID MP2_AnimAddUnit( mp2UNIT *Uni );
VOID MP2_AnimClose( VOID );
VOID MP2_AnimRender( VOID );

mp2UNIT * MP2_AnimUnitCreate( INT Size );
mp2UNIT * MP2_UnitCreateBall( DBL x, DBL y, DBL z );
mp2UNIT * MP2_UnitCreateCube( DBL x, DBL y, DBL z );
VOID LoadSphere( VOID );
MATR MatrView( VEC Loc, VEC At, VEC Up1 );
VOID MP2_RndProj( VOID );
