/*Poroshin Mark 10-2 07.08.2015*/
/* FILE NAME: MAIN.c
 * PROGRAMMER: MP2
 * PURPOSE:function WinMAIN    */
#include "ANIM.H"

#define MP2_WND_CLASS_NAME "My window class"



LRESULT CALLBACK MP2_MyWindowFunc( HWND hWnd, UINT Msg,WPARAM wParam, LPARAM lParam)
{         
  static BITMAP bm;
  static HBITMAP hBm;
  static HDC hMemDC;
  static INT h, w, flag=1;
  HDC hDC;
  PAINTSTRUCT ps;
  SetDbgMemHooks();
  switch(Msg)
  {
  case WM_CREATE:
    MP2_AnimInit(hWnd);
    return 0;

  case WM_ERASEBKGND:
    return 0;

  case WM_TIMER: 
    MP2_AnimRender();
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;

  case WM_MOUSEMOVE:
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    MP2_AnimResize(w, h);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_PAINT: 
    hDC = BeginPaint(hWnd, &ps);
    MP2_AnimCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_COMMAND:
    flag = !flag;
    return 0;

  case WM_DESTROY:
    MP2_AnimClose();
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} 


/* Main program function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  INT i, j;
  

  SetDbgMemHooks();

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MP2_MyWindowFunc;
  wc.lpszClassName = MP2_WND_CLASS_NAME;
  
  if(!RegisterClass(&wc))
  {
    MessageBox(NULL,"Error register window class","Error",MB_OK);
    return 0;
  }

  hWnd = CreateWindow(MP2_WND_CLASS_NAME,"Anim",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT,CW_USEDEFAULT,
                      CW_USEDEFAULT,CW_USEDEFAULT,
                      NULL,
                      NULL,
                      hInstance,
                      NULL);
  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  for(i=0; i<3; i++)
    for(j=0; j<3; j++)    
       MP2_AnimAddUnit(MP2_UnitCreateBall(i * (MP2_Anim.W / 3.0), j * (MP2_Anim.H / 3.0), 0 ));
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
} /* End of Main function */

/* End of file "MAIN.c" */