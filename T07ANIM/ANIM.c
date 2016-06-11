/*Poroshin Mark 10-2 10.08.2015*/
/* FILE NAME: ANIM.c
 * PROGRAMMER: MP2
 * PURPOSE:function anim    */

#include "ANIM.H"
#include <stdio.h>
#pragma comment(lib, "winmm")
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
  HDC hDC;
  LARGE_INTEGER t;
  MP2_Anim.hWnd = hWnd;
  SetTimer(hWnd, 0, 10, NULL);
  hDC = GetDC(hWnd);
  MP2_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  MP2_Anim.NumOfUnits = 0;
  
    /*** init timer ***/
  QueryPerformanceFrequency(&t);
  MP2_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  MP2_StartTime = MP2_OldTime = MP2_OldTimeFPS = t.QuadPart;
  MP2_PauseTime = 0;


}

VOID MP2_AnimResize( INT w,INT h )
{
  HDC hDC;
  if(MP2_Anim.hFrame != NULL)
    DeleteObject(MP2_Anim.hFrame);
  MP2_Anim.W = w;
  MP2_Anim.H = h;
  hDC = GetDC(MP2_Anim.hWnd);
  MP2_Anim.hFrame = CreateCompatibleBitmap(hDC, w, h);
  ReleaseDC(MP2_Anim.hWnd, hDC);
  SelectObject(MP2_Anim.hDC, MP2_Anim.hFrame);
}

VOID MP2_AnimCopyFrame( HDC hDC )
{
   BitBlt(hDC, 0, 0, MP2_Anim.W, MP2_Anim.H, MP2_Anim.hDC, 0, 0, SRCCOPY);
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
  DeleteDC(MP2_Anim.hDC);
  DeleteObject(MP2_Anim.hFrame);
  KillTimer(MP2_Anim.hWnd, 0);
  memset(&MP2_Anim, 0, sizeof(mp2ANIM));
  PostQuitMessage(0);
}

VOID MP2_AnimRender( VOID )
{
  INT i;
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
  MP2_Anim.GlobalTime = (DBL)(t.QuadPart - MP2_StartTime) / MP2_TimePerSec;
  MP2_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - MP2_OldTime) / MP2_TimePerSec;
  /* Time with pause */
  if (MP2_Anim.IsPause)
  {
    MP2_Anim.DeltaTime = 0;
    MP2_PauseTime += t.QuadPart - MP2_OldTime;
  }
  else
  {
    MP2_Anim.DeltaTime = MP2_Anim.GlobalDeltaTime;
    MP2_Anim.Time = (DBL)(t.QuadPart - MP2_PauseTime - MP2_StartTime) / MP2_TimePerSec;
  }
  /* FPS */
  if (t.QuadPart - MP2_OldTimeFPS > MP2_TimePerSec)
  {
    CHAR str[100];

    MP2_Anim.FPS = MP2_FrameCounter * MP2_TimePerSec /
                                         (DBL)(t.QuadPart - MP2_OldTimeFPS);
    MP2_OldTimeFPS = t.QuadPart;
    MP2_FrameCounter = 0;
    sprintf(str, "Anim FPS: %.5f Mouse Coord:  %i, %i JoyStick Coord: %f %f", MP2_Anim.FPS, MP2_Anim.Mx, MP2_Anim.My, MP2_Anim.JX,MP2_Anim.JX);
    SetWindowText(MP2_Anim.hWnd, str);
  }
  MP2_OldTime = t.QuadPart;
  /*. . . опросили все (kbd, mouse, joystick)*/   
  SelectObject(MP2_Anim.hDC, GetStockObject(DC_PEN));
  SelectObject(MP2_Anim.hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(MP2_Anim.hDC, RGB(100,150,200));
  SetDCBrushColor(MP2_Anim.hDC, RGB(100,150,200)); 
  Rectangle(MP2_Anim.hDC, 0, 0, MP2_Anim.W + 1, MP2_Anim.H + 1);
  for (i = 0; i < MP2_Anim.NumOfUnits; i++)
    MP2_Anim.Units[i]->Response(MP2_Anim.Units[i], &MP2_Anim);

  
  for (i = 0; i < MP2_Anim.NumOfUnits; i++)
  {
    SelectObject(MP2_Anim.hDC, GetStockObject(DC_PEN));
    SelectObject(MP2_Anim.hDC, GetStockObject(DC_BRUSH));
    SetDCPenColor(MP2_Anim.hDC, RGB(200,150,100));
    SetDCBrushColor(MP2_Anim.hDC, RGB(200,150,100));
    /* можно сбросить все кисти и перья */
    MP2_Anim.Units[i]->Render(MP2_Anim.Units[i], &MP2_Anim);
  }

}