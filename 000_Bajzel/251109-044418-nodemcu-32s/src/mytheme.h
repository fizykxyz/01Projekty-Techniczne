#ifndef _my_theme_h
#define _my_theme_h

/*
    Theming of color displays
    DSP_ST7735, DSP_ST7789, DSP_ILI9341, DSP_GC9106, DSP_ILI9225, DSP_ST7789_240
    ***********************************************************************
    *    !!! This file must be in the root directory of the sketch !!!    *
    ***********************************************************************
    Uncomment (remove double slash //) from desired line to apply color
*/
#define ENABLE_THEME
#ifdef  ENABLE_THEME

/*----------------------------------------------------------------------------------------------------------------*/
/*       | COLORS             |   values (0-255)  |                                                               */
/*       | color name         |    R    G    B    |                                                               */
/*----------------------------------------------------------------------------------------------------------------*/
#define COLOR_BACKGROUND           0,   0,   0      /*    */
#define COLOR_STATION_NAME       220, 220, 252      /*    */
#define COLOR_STATION_BG           0,   0,   0      /*    */
#define COLOR_STATION_FILL       255, 255, 255      /*    */
#define COLOR_SNG_TITLE_1        255, 255, 255      /*    */
#define COLOR_SNG_TITLE_2        255, 255, 255      /*    */
#define COLOR_WEATHER            255, 255, 255      /*    */
#define COLOR_VU_MAX             255,   0,   0      /*    */
#define COLOR_VU_MIN             220, 220, 252     /*    */
#define COLOR_CLOCK              255, 255, 255      /*    */
#define COLOR_CLOCK_BG             0,   0,   0      /*    */
#define COLOR_SECONDS            212, 212, 212      /*    */
#define COLOR_DAY_OF_W           212, 212, 212      /*    */
#define COLOR_DATE               255, 255, 255      /*    */
#define COLOR_BUFFER               0,   0,   0      /*    */
#define COLOR_IP                 192, 192, 192      /*    */
#define COLOR_VOLUME_VALUE       220, 220, 252     /*    */
#define COLOR_RSSI               220, 220, 252      /*    */
#define COLOR_VOLBAR_OUT         255, 255, 255      /*    */
#define COLOR_VOLBAR_IN            0,   0,   0      /*    */
#define COLOR_DIGITS             255, 255, 255      /*    */
#define COLOR_DIVIDER            186, 186, 248      /*    */
#define COLOR_BITRATE            220, 220, 252      /*    */
#define COLOR_HEAP               255, 255, 255      /*    */
#define COLOR_PL_CURRENT         255, 255, 255      /*    */
#define COLOR_PL_CURRENT_BG        0,   0,   0      /*    */
#define COLOR_PL_CURRENT_FILL    220, 220, 252      /*    */
#define COLOR_PLAYLIST_0         255, 255, 255      /*    */
#define COLOR_PLAYLIST_1         255, 255, 255      /*    */
#define COLOR_PLAYLIST_2         255, 255, 255      /*    */
#define COLOR_PLAYLIST_3         255, 255, 255      /*    */
#define COLOR_PLAYLIST_4         255, 255, 255      /*    */


#endif  /* #ifdef  ENABLE_THEME */
#endif  /* #define _my_theme_h  */
