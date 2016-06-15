/*Poroshin Mark 10-2 10.08.2015*/
/* FILE NAME: ANIM.c
 * PROGRAMMER: MP2
 * PURPOSE:function anim    */

#include "ANIM.H"
#include <stdio.h>
#pragma comment(lib, "winmm")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32s")
#define MP2_GET_JOYSTIC_AXIS(A)  (2.0 * (ji.dw##A##pos - jc.w##A##min) / (jc.w##A##max - jc.w##A##min - 1) - 1)


INT MP2_MOUSEWHEEL;
mp2ANIM MP2_Anim;
static UINT64
  MP2_StartTime,    /* Start program time */
  MP2_OldTime,      /* Time from program start to previous frame */
  MP2_OldTimeFPS,   /* Old time FPS measurement */
  MP2_PauseTime,    /* Time during pause period */
  MP2_TimePerSec,   /* Timer resolution */
  MP2_FrameCounter; /* Frames counter */

VOID MP2_AnimInit( HWND hWnd )
{ 
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  LARGE_INTEGER t;
  
 
  MP2_Anim.NumOfUnits = 0;
  
  memset(&MP2_Anim, 0, sizeof(mp2ANIM));
  MP2_Anim.hWnd = hWnd;
  MP2_Anim.hDC = GetDC(hWnd);

    /*** openGl ***/
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(MP2_Anim.hDC, &pfd);
  DescribePixelFormat(MP2_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(MP2_Anim.hDC, i, &pfd);
    /* OpenGL init: setup rendering context */
  MP2_Anim.hGLRC = wglCreateContext(MP2_Anim.hDC);
  wglMakeCurrent(MP2_Anim.hDC, MP2_Anim.hGLRC);
  /* OpenGL init: setup extensions: GLEW library */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(MP2_Anim.hGLRC);
    ReleaseDC(MP2_Anim.hWnd, MP2_Anim.hDC);
    exit(0);
  }

    /*** init timer ***/
  QueryPerformanceFrequency(&t);
  MP2_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  MP2_StartTime = MP2_OldTime = MP2_OldTimeFPS = t.QuadPart;
  MP2_PauseTime = 0;

  MP2_RndMatrProj = MatrFrustum(-1, 1, -1, 1, 1, 100);
  MP2_RndMatrWorld = MatrIdentity();
  MP2_RndMatrView  = MatrMulMatr(MatrIdentity(), MatrTranslate(VecSet(-1, -1, 0)));

  /* OpenGL specific initialization */
  glClearColor(0.3, 0.5, 0.7, 1);
  glEnable(GL_DEPTH_TEST );
  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
}

VOID MP2_AnimResize( INT w,INT h )
{
  MP2_Anim.W = w;
  MP2_Anim.H = h;
  glViewport(0, 0, w, h);
  MP2_RndProj();
}

VOID MP2_AnimCopyFrame( VOID )
{
  SwapBuffers(MP2_Anim.hDC);
}

VOID MP2_AnimAddUnit( mp2UNIT *Uni )
{
  if(MP2_Anim.NumOfUnits < MP2_MAX_UNITS)
  {
    MP2_Anim.Units[MP2_Anim.NumOfUnits++] = Uni; 
    Uni->Init(Uni, &MP2_Anim);
  }
}

VOID MP2_AnimClose( VOID )
{
  INT i;
  for (i = 0; i < MP2_Anim.NumOfUnits; i++)
  {
    MP2_Anim.Units[i]->Close(MP2_Anim.Units[i], &MP2_Anim);
    free(MP2_Anim.Units[i]);
  }
  /* Delete rendering context */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(MP2_Anim.hGLRC);

  /* Delete GDI data */
  ReleaseDC(MP2_Anim.hWnd, MP2_Anim.hDC);
  DeleteDC(MP2_Anim.hDC);
  KillTimer(MP2_Anim.hWnd, 0);
  memset(&MP2_Anim, 0, sizeof(mp2ANIM));
  PostQuitMessage(0);
}

VOID MP2_AnimRender( VOID )
{
  INT i;
  static FLT dx = 0, dy = 0;
  LARGE_INTEGER t;
  POINT pt;
  srand(1);

  /* Mouse wheel */
  MP2_Anim.Mdz = MP2_MOUSEWHEEL;
  MP2_Anim.Mz += MP2_MOUSEWHEEL;
  MP2_MOUSEWHEEL = 0;

  /* Mouse */
  GetCursorPos(&pt);
  ScreenToClient(MP2_Anim.hWnd, &pt);
  MP2_Anim.Mdx = pt.x - MP2_Anim.Mx;
  MP2_Anim.Mdy = pt.y - MP2_Anim.My;
  MP2_Anim.Mx = pt.x;
  MP2_Anim.My = pt.y;

  /*Keyboard*/
  GetKeyboardState(MP2_Anim.Keys);
  for ( i = 0; i < 256; i++ )
  {
    MP2_Anim.Keys[i] >>= 7;
    if (!MP2_Anim.OldKeys[i] && MP2_Anim.Keys[i])
      MP2_Anim.KeysClick[i] = 1;
    else
      MP2_Anim.KeysClick[i] = 0;
  }
  memcpy(MP2_Anim.OldKeys, MP2_Anim.Keys, 256);

  /* joystick */
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
          MP2_Anim.JBut[i] = (ji.dwButtons >> i) & 1;

        /* Axes */
        MP2_Anim.JX = MP2_GET_JOYSTIC_AXIS(X);
        MP2_Anim.JY = MP2_GET_JOYSTIC_AXIS(Y);
        MP2_Anim.JZ = MP2_GET_JOYSTIC_AXIS(Z);
        MP2_Anim.JR = MP2_GET_JOYSTIC_AXIS(R);

        /* Point of view */
        MP2_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;
      }
    }
  }

  /*** Handle timer ***/
  MP2_FrameCounter++;
  QueryPerformanceCounter(&t);
  
  /* Global time */
  MP2_Anim.GlobalTime = (FLT)(t.QuadPart - MP2_StartTime) / MP2_TimePerSec;
  MP2_Anim.GlobalDeltaTime = (FLT)(t.QuadPart - MP2_OldTime) / MP2_TimePerSec;
  
  /* Time with pause */
  if (MP2_Anim.IsPause)
  {
    MP2_Anim.DeltaTime = 0;
    MP2_PauseTime += t.QuadPart - MP2_OldTime;
  }
  else
  {
    MP2_Anim.DeltaTime = MP2_Anim.GlobalDeltaTime;
    MP2_Anim.Time = (FLT)(t.QuadPart - MP2_PauseTime - MP2_StartTime) / MP2_TimePerSec;
  }

  /* FPS */
  if (t.QuadPart - MP2_OldTimeFPS > MP2_TimePerSec)
  {
    CHAR str[100];

    MP2_Anim.FPS = MP2_FrameCounter * MP2_TimePerSec /
                                         (FLT)(t.QuadPart - MP2_OldTimeFPS);
    MP2_OldTimeFPS = t.QuadPart;
    MP2_FrameCounter = 0;
    sprintf(str, "Anim FPS: %.5f Mouse Coord:  %i, %i JoyStick Coord: %f %f", MP2_Anim.FPS, MP2_Anim.Mx, MP2_Anim.My, MP2_Anim.JX,MP2_Anim.JX);
    SetWindowText(MP2_Anim.hWnd, str);
  }
  MP2_OldTime = t.QuadPart;  

  for (i = 0; i < MP2_Anim.NumOfUnits; i++)  
    MP2_Anim.Units[i]->Response(MP2_Anim.Units[i], &MP2_Anim);
   /*** Clear frame ***/
  /* Clear background */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (i = 0; i < MP2_Anim.NumOfUnits; i++)  
  {
    MP2_RndMatrWorld = MatrIdentity();
    MP2_Anim.Units[i]->Render(MP2_Anim.Units[i], &MP2_Anim);
  }

  dx += MP2_Anim.JX / 10;
  dy += MP2_Anim.JY / 10;
  MP2_RndMatrView = MatrView(VecSet(MP2_Anim.JX * 30, MP2_Anim.JY * 30, MP2_Anim.JZ * 30), VecSet(0,0,0), VecSet(0,1,0));
  glLoadMatrixf(&MP2_RndMatrView.A[0][0]);
  glFinish();
}