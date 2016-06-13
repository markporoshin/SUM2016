/* FILE NAME: IMAGE.H
 * PROGRAMMER: MP2
 * DATE: 13.06.2016
 * PURPOSE: Image handle decrlaration.
 */

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"

/* Image represetation type */
typedef struct
{
  HBITMAP hImage; /* Image bitmap handle */
  INT W, H;       /* Image size */
  DWORD *Bits;    /* Image pixel color data */
} mp2IMG;

/* Load image function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       vg4IMG *Im;
 *   - image file name (*.BMP):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL MP2_ImageLoad( mp2IMG *Im, CHAR *FileName );

/* Free image function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       vg4IMG *Im;
 * RETURNS: None.
 */
VOID MP2_ImageFree( mp2IMG *Im );

/* Get image pixel color function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       vg4IMG *Im;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS:
 *   (DWORD) pixel color.
 */
DWORD MP2_ImageGetP( mp2IMG *Im, INT X, INT Y );


#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */