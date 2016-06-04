
/*Poroshin Mark 10-2 03.08.2015*/
/* FILE NAME: T02CLOCK.c
 * PROGRAMMER: MP2
 * PURPOSE:-                   */

#include <math.h>
#include <windows.h>
#define WND_CLASS_NAME "My window class"
#define MP2_PI 3.14159265358979323846




/* Begin of function "arrow" */
VOID arrow( HWND hWnd, HDC hDC, INT x, INT y, POINT pt, INT r )
{
  INT i;
  DOUBLE len, sina, cosa; 
  HPEN hPen, hOldPen;
  HBRUSH hBr,hOldBr;
  POINT pte[100], ptb[3] = {{r, 0}, {0, r*0.5}, {0, -r * 0.5}};/*red part of the arrow*/
  POINT pt2e[100], pt2b[4] = {{0, r*0.25},{-r, r*0.25}, {-r, -r*0.25}, {0, -r * 0.25}};/*blue part of the arrow*/
  POINT ptc[100];
  len = sqrt((pt.x - x) * (pt.x - x) + (pt.y - y) * (pt.y - y));
  cosa = (pt.x - x) / len;
  sina = (pt.y - y) / len;
 
  for(i=0; i<100; i++)
  {
    ptc[i].x =sqrt(-(i-r)*(i-r) + r*r);
    ptc[i].y =i-r;
  }

 
  hPen = CreatePen(PS_SOLID, 1,RGB(0, 0, 0));
  hOldPen = SelectObject(hDC, hPen);
  hBr = CreateSolidBrush(RGB(255, 0, 0));
  hOldBr = SelectObject(hDC, hBr);
  for(i=0; i<100; i++)
  {
    pte[i].x = x + ptc[i].x*cosa - ptc[i].y*sina;
    pte[i].y = y + ptc[i].x*sina + ptc[i].y*cosa;
  } 
  Polygon(hDC,pte,100);
  DeleteObject(hBr);
  DeleteObject(hPen);

  hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
  SelectObject(hDC, hPen);
  hBr = CreateSolidBrush(RGB(0, 0, 255));
  SelectObject(hDC, hBr);

  for(i=0; i<100; i++)
  {
    pt2e[i].x = x - ptc[i].x*cosa - (ptc[i].y)*sina;
    pt2e[i].y = y - ptc[i].x*sina + (ptc[i].y)*cosa;
  } 
  Polygon(hDC,pt2e,100);
  DeleteObject(hBr);
  DeleteObject(hPen);

  SelectObject(hDC, hOldBr);
  SelectObject(hDC, hOldPen);
} /* End of function "arrow" */

/* Begin of function "MyWindowFunc" */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,WPARAM wParam, LPARAM lParam)
{         


  LOGFONT font;
  char str[18];
  INT i=0;
  DOUBLE x, y;
  HPEN hPen;
  SYSTEMTIME Time;
  static BITMAP bm;
  static HBITMAP hBmLogo, hBm;
  static HDC hMemDCLogo, hMemDC;
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
    hPen= CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
    SelectObject(hMemDC, hPen);
    Rectangle(hMemDC, 0, 0, w+1, h+1);
    DeleteObject(hPen);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    srand(50);
    if(flag)
    for(i=0; i<10; i++)
    {
      x=rand() % (w+1);
      y=rand() % (h+1); 
      arrow(hWnd,hMemDC, x, y, pt, 50);
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
    flag=!flag;
    return 0;

  case WM_DESTROY:
    DeleteDC(hMemDCLogo);
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 0);
    PostQuitMessage(0);
    return 0;
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

/*End of T02CLOCK.c*/