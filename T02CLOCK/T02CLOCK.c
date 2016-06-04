/*Poroshin Mark 10-2 02.08.2015*/
/* FILE NAME: T02CLOCK.c
 * PROGRAMMER: MP2
 * PURPOSE:-
 */
#include <math.h>
#include <time.h>
#include <windows.h>
#define WND_CLASS_NAME "My window class"
#define MP2_PI 3.14159265358979323846 



/* Set/reset full screen mode function */
VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (IsFullScreen)
  {
    /* restore window size */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    /* Set full screen size to window */
    HMONITOR hmon;
    MONITORINFOEX moninfo;
    RECT rc;

    /* Store window old size */
    GetWindowRect(hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    /* Obtain monitor info */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* Set window new size */
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOPMOST,
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'FlipFullScreen' function */


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,WPARAM wParam, LPARAM lParam)
{         
  MINMAXINFO *MinMax;
  LOGFONT font;
  char str[18];
  INT i=0;
  DOUBLE x, y, speed;
  HPEN hPen;
  SYSTEMTIME Time;
  static BITMAP bm, bmPac;
  static HBITMAP hBmLogo, hBm, hBmANDPac, hBmXORPac;
  static HDC hMemDCLogo, hMemDC, hMemANDPac, hMemXORPac;
  static INT h=500, w=700, flag=1;
  HDC hDC;
  POINT pt;
  PAINTSTRUCT ps;
  switch(Msg)
  {
  case WM_GETMINMAXINFO:
    MinMax = (MINMAXINFO *)lParam;
    MinMax -> ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;

  case WM_CREATE:
    SetTimer(hWnd, 0, 10, NULL);
    hDC = GetDC(hWnd);
    hBmLogo = LoadImage(NULL, "CLOCK.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(bm), &bm);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);

    hBmANDPac = LoadImage(NULL, "ANDPAC.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmXORPac = LoadImage(NULL, "XORPAC.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmXORPac, sizeof(bmPac), &bmPac);

    hMemANDPac = CreateCompatibleDC(hDC);
    hMemXORPac = CreateCompatibleDC(hDC);
    SelectObject(hMemANDPac,hBmANDPac);
    SelectObject(hMemXORPac,hBmXORPac); 
    
    SelectObject(hMemDCLogo,hBmLogo);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_ERASEBKGND:
    return 0;

  case WM_KEYDOWN:
    if(LOWORD(wParam) == 'F')
      FlipFullScreen(hWnd);
    if(LOWORD(wParam) == VK_ESCAPE)
      DestroyWindow(hWnd);
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
      srand(30);
      for(i=0; i<10; i++)
      {
        x=rand() % (w+1);
        y=rand() % (h+1); 
        speed  =rand() % (h+1);
        StretchBlt(hMemDC, sin(clock()/100000.0*speed)*300.0+x, cos(clock()/100000.0*speed)*300.0+y, bm.bmWidth, bm.bmHeight, hMemANDPac, 0, 0,bm.bmWidth,bm.bmHeight,SRCAND);
        StretchBlt(hMemDC, sin(clock()/100000.0*speed)*300.0+x, cos(clock()/100000.0*speed)*300.0+y, bm.bmWidth, bm.bmHeight, hMemXORPac, 0, 0,bm.bmWidth,bm.bmHeight,SRCINVERT);
      }
        
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

      hPen = CreatePen(PS_SOLID, 10, RGB(100, 200, 200));
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

      
     font.lfHeight =  12 ;// Устанавливает высоту шрифта или символа
     font.lfWidth =  0;// Устанавливает среднюю ширину символов в шрифте
     font.lfEscapement = 0;// Устанавливает угол, между вектором наклона и осью X устройства
     font.lfOrientation = 0;// Устанавливает угол, между основной линией каждого символа и осью X устройства
     font.lfWeight = 100;// Устанавливает толщину шрифта в диапазоне от 0 до 1000
     font.lfItalic = 0;// Устанавливает курсивный шрифт
     font.lfUnderline = 0;// Устанавливает подчеркнутый шрифт
     font.lfStrikeOut = 0;// Устанавливает зачеркнутый шрифт
     font.lfCharSet = RUSSIAN_CHARSET;// Устанавливает набор символов
     font.lfOutPrecision = 0;// Устанавливает точность вывода
     font.lfClipPrecision = 0;// Устанавливает точность отсечения
     font.lfQuality = 0;// Устанавливает качество вывода
     font.lfPitchAndFamily = 0;// Устанавливает ширину символов и семейство шрифта
    strcpy(font.lfFaceName , "VERDANA");//  устанавливает название шрифта
 

      TextOut(hMemDC,20,h-20,str,strlen(str));
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
    DeleteDC(hMemANDPac);
    DeleteDC(hMemXORPac);
    DeleteObject(hBm);
    DeleteObject(hBmANDPac);
    DeleteObject(hBmXORPac);
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