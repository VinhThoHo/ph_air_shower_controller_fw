/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#include "logo.h"
#include "u8g2.h"
#include "bitmaps.h"

u8g2_t u8g2;

/* logo display */
void Logo(void)
{
  u8g2_DrawBitmap(&u8g2, 0, 0, bmp_logo_ph.width/8, bmp_logo_ph.height, bmp_logo_ph.data);
  u8g2_SendBuffer(&u8g2);
}
