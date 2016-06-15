/* FILE NAME: IMAGE.C
 * PROGRAMMER: MP2
 * DATE: 13.06.2016
 * PURPOSE: Image handle functions.
 */

#include "image.h"

/* Load image function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       vg4IMG *Im;
 *   - image file name (*.BMP):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL MP2_ImageLoad( mp2IMG *Im, CHAR *FileName )
{
  HDC hDC, hMemDC, hMemDC1;
  HBITMAP hBm;
  BITMAP bm;
  BITMAPINFOHEADER bmih;

  memset(Im, 0, sizeof(mp2IMG));

  /* Load image from file */
  hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBm == NULL)
    return FALSE;

  /* Create contextes */
  hDC = GetDC(NULL);
  hMemDC = CreateCompatibleDC(hDC);
  hMemDC1 = CreateCompatibleDC(hDC);
  GetObject(hBm, sizeof(bm), &bm);
  Im->W = bm.bmWidth;
  Im->H = bm.bmHeight;
  SelectObject(hMemDC, hBm);
  ReleaseDC(NULL, hDC);

  /* Build DIB section */
  memset(&bmih, 0, sizeof(bmih));
  bmih.biSize = sizeof(BITMAPINFOHEADER);
  bmih.biBitCount = 32;                   /* BGRx - DWORD */
  bmih.biPlanes = 1;                      /* always */
  bmih.biCompression = BI_RGB;            /* 0 - no compression */
  bmih.biWidth = bm.bmWidth;              /* width */
  bmih.biHeight = -bm.bmHeight;           /* height + first coded row is 0 */
  bmih.biSizeImage = bm.bmWidth * bm.bmHeight * 4;  /* image size in bytes */

  Im->hImage = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, DIB_RGB_COLORS,
                 (VOID **)&Im->Bits, NULL, 0);
  SelectObject(hMemDC1, Im->hImage);

  /* Draw load image to DIB */
  BitBlt(hMemDC1, 0, 0, Im->W, Im->H, hMemDC, 0, 0, SRCCOPY);

  DeleteDC(hMemDC);
  DeleteDC(hMemDC1);
  return TRUE;
} /* End of 'VG4_ImageLoad' function */

/* Free image function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       vg4IMG *Im;
 * RETURNS: None.
 */
VOID mp2_ImageFree( mp2IMG *Im )
{
  if (Im->hImage != NULL)
    DeleteObject(Im->hImage);
  memset(Im, 0, sizeof(mp2IMG));
} /* End of 'VG4_ImageFree' function */

/* Get image pixel color function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       vg4IMG *Im;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS:
 *   (DWORD) pixel color.
 */
DWORD MP2_ImageGetP( mp2IMG *Im, INT X, INT Y )
{
  if (Im->hImage != NULL && X >= 0 && X < Im->W && Y >= 0 && Y < Im->H)
    return Im->Bits[Y * Im->W + X];
  return 0;
} /* End of 'VG4_ImageGetP' function */

/* END OF 'IMAGE.C' FILE */