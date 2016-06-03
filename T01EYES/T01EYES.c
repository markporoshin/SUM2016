/*Poroshin Mark 10-2 01.08.2015*/
/* FILE NAME: T01EYES.c
 * PROGRAMMER: MP2
 * PURPOSE:-
 */
#include <math.h>
#include <windows.h>
#define WND_CLASS_NAME "My window class"


/*VOID DrawEyes2(HWND hWnd,HDC hDC, INT x,INT y, INT r11,INT r12,INT r2,POINT pt)
{
  DOUBLE x1,y1,len;
  x1=(pt.x-x)/r11+x;
  y1=(pt.y-y)/r12+y;
  len = sqrt(pow(x1-x,2)+pow(y1-y,2));
  if(len > 1)
  {
    x = (x1-x)*r11+x;
    y = (x1-x)*r12+x;
  }
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCBrushColor(hDC, RGB(250,250,250));
  Ellipse(hDC,x-r11,y+r12,x+r11,y-r12);

  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(250,1,1));
  Ellipse(hDC,x1,y1,x1+r2,y1+r2);
}   */

/* Draw eyes function */
VOID DrawEyes(HWND hWnd,HDC hDC, INT x,INT y, INT r1,INT r2,POINT pt)
{
  INT len, x1, y1;
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCBrushColor(hDC, RGB(250,250,250));
  Ellipse(hDC,x-r1,y+r1,x+r1,y-r1);
  len = sqrt(pow(pt.x-x,2)+pow(pt.y-y,2));
  if( len +r2/2> r1 )
  {
    x1 =  (pt.x-x) * (r1-r2/2) / len+x-r2/2;
    y1 =  (pt.y-y) * (r1-r2/2) / len+y-r2/2;
  }                     
  else
  {
    x1 =  pt.x;
    y1 =  pt.y;
  }
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(250,1,1));
  Ellipse(hDC,x1,y1,x1+r2,y1+r2);
} /*End of "DrawEyes" */


/*Main window message handle function*/
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,WPARAM wParam, LPARAM lParam)
{
  INT i=0, x, y;
  static HBITMAP hBm;
  static HDC hMemDC;
  static INT h=500, w=700, flag=1;
  HDC hDC;
  POINT pt;
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
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(255,255,255));
    Rectangle(hMemDC,0,0,w+1,h+1);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    srand(40);
    if(flag)
    {
      for(i=0; i<10; i++)
      {
        x=rand() % w;
        y=rand() % h;
        DrawEyes
             
          
           
          (hWnd,hMemDC, x, y, 100,200, 30, pt);
      }
    }
 
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
    ReleaseDC(hWnd,hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER,0, 0);
    return 0;

  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd,0);
    PostQuitMessage(0);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC,0,0,w,h,hMemDC,0,0,SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_COMMAND:
    flag=!flag;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}/* End of "MyWindowFunc" */


/* Main program function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;
  
  if(!RegisterClass(&wc))
  {
    MessageBox(NULL,"Error register window class","Error",MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME,"EYES",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT,CW_USEDEFAULT,
                      CW_USEDEFAULT,CW_USEDEFAULT,
                      NULL,
                      NULL,
                      hInstance,
                      NULL);
   CreateWindow("Button","ON/OFF",
                      WS_CHILDWINDOW | WS_VISIBLE,
                      CW_USEDEFAULT,CW_USEDEFAULT,
                      60,30,
                      hWnd,
                      NULL,
                      hInstance,
                      NULL);
  ShowWindow(hWnd, SW_SHOWNORMAL);
  
  UpdateWindow(hWnd);
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
} /*End of Main function*/

/*End of T01EYES.c*/

