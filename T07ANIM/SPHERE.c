#include "ANIM.H"
#include <time.h>
#define  M 30
#define  N 30

typedef struct
{
  mp2UNIT;
  VEC Pos;
}mp2SPHERE;

typedef struct
{
  HBITMAP hBmGlobe;
  DWORD *Bits;
  INT W, H;
} IMG;

IMG Globe;

VOID sphere( HDC hDC, INT xc, INT yc );

static VOID MP2_UnitInit( mp2SPHERE *Uni, mp2ANIM *Ani )
{
  
} /* End of 'MP2_UnitInit' function */

static VOID MP2_UnitRender( mp2SPHERE *Uni, mp2ANIM *Ani )
{
  //Ellipse(Ani->hDC, Uni->Pos.X, Uni->Pos.X, Uni->Pos.X + 100, Uni->Pos.X + 100);
  sphere(Ani->hDC, Uni->Pos.X, Uni->Pos.Y); 
}

mp2UNIT * MP2_UnitCreateBall( DBL x, DBL y, DBL z )
{
  mp2SPHERE *Uni;

  if ((Uni = (mp2SPHERE *)MP2_AnimUnitCreate( sizeof(mp2SPHERE) )) == NULL)
    return NULL;
  Uni->Pos = VecSet(x, y, z); 
  /* Setup unit methods */
  Uni->Init = (VOID *)MP2_UnitInit;
  Uni->Render = (VOID *)MP2_UnitRender;
  
  return (mp2UNIT *)Uni;
} /* End of 'SA2_UnitCreateBall' function */



VOID LoadSphere( VOID )
{
  HDC hDC = GetDC(NULL), hMemDC, hMemDC1;
  HBITMAP hBm;
  BITMAP bm;
  BITMAPINFOHEADER bmih;

  /* Load image from file */
  hMemDC = CreateCompatibleDC(hDC);
  hBm = LoadImage(NULL, "GLOBE.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  GetObject(hBm, sizeof(bm), &bm);
  Globe.W = bm.bmWidth;
  Globe.H = bm.bmHeight;
  SelectObject(hMemDC, hBm);

  /* Fill DIB info */
  memset(&bmih, 0, sizeof(bmih));
  bmih.biSize = sizeof(BITMAPINFOHEADER);
  bmih.biBitCount = 32;                   /* BGRx - DWORD */
  bmih.biPlanes = 1;                      /* always */
  bmih.biCompression = BI_RGB;            /* 0 - no compression */
  bmih.biWidth = bm.bmWidth;              /* width */
  bmih.biHeight = -bm.bmHeight;           /* height + first coded row is 0 */
  bmih.biSizeImage = bm.bmWidth * bm.bmHeight * 4;  /* image size in bytes */

  Globe.hBmGlobe = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, DIB_RGB_COLORS,
                 (VOID **)&Globe.Bits, NULL, 0);
  hMemDC1 = CreateCompatibleDC(hDC);
  SelectObject(hMemDC1, Globe.hBmGlobe);

  BitBlt(hMemDC1, 0, 0, Globe.W, Globe.H, hMemDC, 0, 0, SRCCOPY);

  DeleteDC(hMemDC);
  DeleteDC(hMemDC1);
  DeleteObject(hBm);

  ReleaseDC(NULL, hDC);
} /* End of 'LoadSphere' function */



VOID DrawQuad( HDC hDC, POINT P0, POINT P1, POINT P2, POINT P3 )
{
  INT s =
    (P0.x - P1.x) * (P0.y + P1.y) +
    (P1.x - P2.x) * (P1.y + P2.y) +
    (P2.x - P3.x) * (P2.y + P3.y) +
    (P3.x - P0.x) * (P3.y + P0.y);

  if (s > 0)
  {
    POINT pts[4];
    
    pts[0] = P0;
    pts[1] = P1;
    pts[2] = P2;
    pts[3] = P3;

    Polygon(hDC, pts, 4);
  }
} /* End of 'DrawQuad' function */
VOID sphere( HDC hDC, INT xc, INT yc )
{
  DOUBLE phi, theta, x, y, z, phaze = clock() / 1000.0;
  INT i, j, k, r1 = 100;
  SYSTEMTIME Time;
  static VEC V[N][M];
  static POINT Ps[N][M], p0, p1, p2, p3;
  INT img_x, img_y;
  COLORREF c;
  BYTE r,g,b;
  
  LoadSphere();

  GetLocalTime(&Time);
  for (i = 0; i < N; i++)
  {
    theta = i * PI / (N - 1);
    for (j = 0; j < M; j++)
    { 

      phi = j * 2.0 * PI / (M - 1) + phaze;
      V[i][j].X = r1 * sin(theta) * cos(phi);
      V[i][j].Y = r1 * sin(theta) * sin(phi);
      V[i][j].Z = r1 * cos(theta);
      V[i][j] = PointTransform4(V[i][j] , MatrRotate(30, VecSet(1, 1, 1) ) );
      Ps[i][j].x = xc + V[i][j].X;
      Ps[i][j].y = yc + V[i][j].Z;
      //SetPixelV(hDC, V[i][j].X + xc, V[i][j].Z + yc, RGB(b, g, r));
    }
  }

  for (j = 0; j < M - 1; j++)
  { 
    for (i = 1; i < N - 1; i++)
    { 
      img_x = j * (Globe.W - 1) / (M - 1);
      img_y = i * (Globe.H - 1) / (N - 1);
      c = Globe.Bits[img_x + img_y * Globe.W]; /* GetPixel(Globe.hDC, img_x, img_y); */
      r = GetRValue(c);
      g = GetGValue(c);
      b = GetBValue(c);
      c = RGB(b, g, r);
      p0 = Ps[i][j];
      p1 = Ps[i][j + 1];
      p2 = Ps[i + 1][j + 1];
      p3 = Ps[i + 1][j];
      SelectObject(hDC, GetStockObject(DC_PEN));
      SelectObject(hDC, GetStockObject(DC_BRUSH));
      SetDCPenColor(hDC, c);
      SetDCBrushColor(hDC, c);
      DrawQuad(hDC, p0, p1, p2, p3);
    }
  }
}