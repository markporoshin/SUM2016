/*Poroshin Mark 10-2 02.08.2015*/
/* FILE NAME: T02CLOCK.c
 * PROGRAMMER: MP2
 * PURPOSE:-
 */
#include <math.h>
#include <windows.h>
#define WND_CLASS_NAME "My window class"
#define MP2_PI 3.14159265358979323846 






LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,WPARAM wParam, LPARAM lParam)
{
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
    hBmLogo = LoadImage(NULL,"CLOCK.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    GetObject(hBmLogo,sizeof(bm),&bm);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    SelectObject(hMemDCLogo,hBmLogo);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_ERASEBKGND:
    return 0;

  case WM_TIMER: 
    hPen= CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
    SelectObject(hMemDC, hPen);
    Rectangle(hMemDC, 0, 0, w+1, h+1);
    DeleteObject(hPen);
    GetLocalTime(&Time);
    /*BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDCLogo, 0, 0, SRCCOPY);*/
    if(flag)
    {
      StretchBlt(hMemDC, w/2-h/2, 0, h, h, hMemDCLogo, 0, 0,bm.bmWidth,bm.bmHeight,SRCCOPY);

      hPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
      SelectObject(hMemDC, hPen);
      MoveToEx(hMemDC,w/2,h/2,NULL); 
      LineTo(hMemDC, sin((Time.wSecond) * 6.0 / 180.0 * MP2_PI)*(h/4) + w/2, -cos(Time.wSecond * 6.0 / 180.0 * MP2_PI)*(h/3) + h/2); 
      DeleteObject(hPen);

      hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
      SelectObject(hMemDC, hPen);
      MoveToEx(hMemDC,w/2,h/2,NULL); 
      LineTo(hMemDC, sin(Time.wMinute * 6.0 / 180.0 * MP2_PI)*(h/4) + w/2, -cos((Time.wMinute) * 6.0 / 180.0 * MP2_PI)*(h/4) + h/2); 
      DeleteObject(hPen);

      hPen = CreatePen(PS_SOLID, 10, RGB(0, 0, 255));
      SelectObject(hMemDC, hPen);
      MoveToEx(hMemDC,w/2,h/2,NULL); 
      LineTo(hMemDC, sin((Time.wHour % 12) * 30.0 / 180.0 * MP2_PI)*(h/10) + w/2, -cos((Time.wHour % 12) * 30.0 / 180.0 * MP2_PI)*(h/10) + h/2); 
      DeleteObject(hPen);

      str[0] = '0' + (INT)Time.wDay/10;
      str[1] = '0' + (INT)Time.wDay%10;
      str[2] = '.';
      str[3] = '0' + (INT)Time.wMonth/10;
      str[4] = '0' + (INT)Time.wMonth%10;
      str[5] = '.';
      str[6] = '0' + (INT)Time.wYear/1000;
      str[7] = '0' + (INT)Time.wYear/100 - ((INT)Time.wYear/1000)*10;
      str[8] = '0' + (INT)Time.wYear/10 - ((INT)Time.wYear/100)*10 - ((INT)Time.wYear/100 - ((INT)Time.wYear/1000)*10);
      str[9] = '0' + (INT)Time.wYear%10;
      str[10] = ' ';
      str[11] = ' ';
      if(Time.wHour / 10 != 0){
        str[12] = '0' + (INT)Time.wHour/10;
        str[13] = '0' + (INT)Time.wHour%10;}
      else{
        str[12] = '0';
        str[13] = '0' + (INT)Time.wHour;}
      str[14] = '.';
      str[15] = '0' + (INT)Time.wMinute/10;
      str[16] = '0' + (INT)Time.wMinute%10;
      str[17] = 0;
      TextOut(hMemDC,20,h-20,str,strlen(str));
    }
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
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