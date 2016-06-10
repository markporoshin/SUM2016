/*Poroshin Mark 10-2 10.08.2015*/
/* FILE NAME: ANIM.c
 * PROGRAMMER: MP2
 * PURPOSE:function anim    */

#include "ANIM.H"

mp2ANIM MP2_Anim;

VOID MP2_AnimInit( HWND hWnd )
{ 
  HDC hDC;
  MP2_Anim.hWnd = hWnd;
  SetTimer(hWnd,0,10,NULL);
  hDC = GetDC(hWnd);
  MP2_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
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
  DeleteObject(hDC);
}

VOID MP2_AnimCopyFrame( HDC hDC )
{
   BitBlt(hDC, 0, 0, MP2_Anim.W, MP2_Anim.H, MP2_Anim.hDC, 0, 0, SRCCOPY);
}

VOID MP2_AnimAddUnit( mp2ANIM *Ani )
{

}

VOID MP2_AnimClose( VOID )
{
  INT i;
  for (i = 0; i < MP2_Anim.NumOfUnits; i++)
  {
    MP2_Anim.Units[i]->Close(MP2_Anim.Units[i], &MP2_Anim);
    free(MP2_Anim.Units[i]);
  }
  MP2_Anim.NumOfUnits = 0;
  DeleteObject(MP2_Anim.hFrame);
  DeleteDC(MP2_Anim.hDC);
  KillTimer(MP2_Anim.hWnd, 0);
  PostQuitMessage(0);
}

VOID MP2_AnimRender( VOID )
{
  INT i;
  /*. . . опросили все (kbd, mouse, joystick)*/
  for (i = 0; i < MP2_Anim.NumOfUnits; i++)
    MP2_Anim.Units[i]->Response(MP2_Anim.Units[i], &MP2_Anim);

  
  for (i = 0; i < MP2_Anim.NumOfUnits; i++)
  {
    /* можно сбросить все кисти и перья */
    MP2_Anim.Units[i]->Render(MP2_Anim.Units[i], &MP2_Anim);
  }
  SelectObject(MP2_Anim.hDC, GetStockObject(DC_PEN));
  SelectObject(MP2_Anim.hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(MP2_Anim.hDC, RGB(100,150,0));
  SetDCBrushColor(MP2_Anim.hDC, RGB(100,150,0)); 
  Rectangle(MP2_Anim.hDC, 0, 0, MP2_Anim.W + 1, MP2_Anim.H + 1);
}