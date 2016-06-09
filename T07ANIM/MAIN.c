/*Poroshin Mark 10-2 07.08.2015*/
/* FILE NAME: MAIN.c
 * PROGRAMMER: MP2
 * PURPOSE:function WinMAIN    */
#include <windows.h>
#include "VEC.H"

#define WND_CLASS_NAME "My window class"



/*LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,WPARAM wParam, LPARAM lParam)
{         
  static BITMAP bm;
  static HBITMAP hBm;
  static HDC hMemDC;
  static INT h, w, flag=1;
  HDC hDC;
  PAINTSTRUCT ps;
  switch(Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd,0,10,NULL);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_ERASEBKGND:
    return 0;

  case WM_TIMER: 
    SelectObject(hDC, GetStockObject(DC_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCPenColor(hDC, RGB(100,150,0));
    SetDCBrushColor(hDC, RGB(100,150,0));
    Rectangle(hMemDC, 0, 0, w+1, h+1);
    
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;

  case WM_MOUSEMOVE:
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if(hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    DeleteObject(hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_PAINT:                                            
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_COMMAND:
    flag = !flag;
    return 0;

  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 0);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}*/ 


/* Main program function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
 DBL sum;
 MATR m;
 VEC A, B, C;

 A = VecSet(1, 0, 0);
 B = VecSet(0, 1, 0);
 C = VecCrossVec(A, B);
 C = VecCrossVec(B, A);
 sum = VecDotVec(A, B);



 m = MatrIdentity();
 m = MatrMulMatr(MatrTranslate(VecSet(1, 2, 3)), MatrTranslate(VecSet(1, 2, 3)));
 m = MatrTranspose(m);
 //m = MatrMulMatr(m, MatrInverse(m));
 m = MatrMulMatr(m, MatrRotateX(0));
} /* End of Main function */

/* End of file "MAIN.c" */