//
//  video.c
//  pccel2
//
//  Created by Noah Wooten on 12/30/23.
//

#include "devices.h"
#include "SDL.h"

void VideoInit(void);
void VideoShutdown(void);

WORD64 VideoGetTextBuffer(void);
void VideoSetTextBuffer(WORD64 NewOffset);
void VideoDrawLine(WORD16 x1, WORD16 y1, WORD16 x2, WORD16 y2, WORD32 color);
void VideoDrawRect(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD32 color);
void VideoDrawFill(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD32 color);
void VideoCopyRect(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD64 ptr);
WORD64 VideoGetWidthHeight(void);
void VideoSuggestSize(WORD16 w, WORD16 h);
