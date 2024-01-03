//
//  video.c
//  pccel2
//
//  Created by Noah Wooten on 12/30/23.
//

#include "devices.h"
#include "SDL.h"

#define SDL_Mutex SDL_mutex
#define SDL_Semaphore SDL_sem

#include <stdlib.h>
#include <string.h>

typedef union _VIDEO_CTX_XARGS {
    void* Buffer;
    char* Text;
    WORD32 Color;
}VIDEO_CTX_XARGS, *PVIDEO_CTX_XARGS;

#define VCTYPE_RECT 0x00
#define VCTYPE_OUTL 0x01
#define VCTYPE_COPY 0x02
#define VCTYPE_LINE 0x03

#define R(x) (Uint8)((WORD32)(x & 0xFF000000) >> 24)
#define G(x) (Uint8)((WORD32)(x & 0x00FF0000) >> 16)
#define B(x) (Uint8)((WORD32)(x & 0x0000FF00) >> 8)
#define A(x) (Uint8)((WORD32)(x & 0x000000FF) >> 0)
#define RMASK 0x000000FF
#define GMASK 0x0000FF00
#define BMASK 0x00FF0000
#define AMASK 0xFF000000

typedef struct _VIDEO_CTX_COMMAND {
    WORD32 VcType;
    int x, y, w, h;
    VIDEO_CTX_XARGS XArgs;
}VIDEO_CTX_COMMAND, *PVIDEO_CTX_COMMAND;

typedef struct _VIDEO_CTX {
    PVIDEO_CTX_COMMAND Commands;
    WORD32 CommandCount, CommandsMax;
    
    SDL_Window* Window;
    SDL_Renderer* Renderer;
    SDL_Mutex* VcMutex;
    SDL_Mutex* Signal_Shutdown;
    SDL_Mutex* Signal_VideoReady;
}VIDEO_CTX, *PVIDEO_CTX;

PVIDEO_CTX VdCtx;

void VideoInit(void) {
    VdCtx = malloc(sizeof(VIDEO_CTX));
    memset(VdCtx, 0, sizeof(VIDEO_CTX));
    
    VdCtx->Window = SDL_CreateWindow("PCCEL2/PLASM2 Layer", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    VdCtx->Renderer = SDL_CreateRenderer(VdCtx->Window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    SDL_LockMutex(VdCtx->VcMutex);
    VdCtx->CommandsMax = 4096;
    VdCtx->Commands = malloc(sizeof(VIDEO_CTX_COMMAND) *
        VdCtx->CommandsMax);
    SDL_UnlockMutex(VdCtx->VcMutex);
    
    SDL_LockMutex(VdCtx->Signal_Shutdown);
    
    SDL_Event Event;
    while (1) {
        SDL_PollEvent(&Event);
        if (Event.type == SDL_QUIT) {
            SDL_UnlockMutex(VdCtx->Signal_Shutdown);
            break;
        }
        
        SDL_LockMutex(VdCtx->Signal_VideoReady);
        for (int i = 0; i < VdCtx->CommandCount; i++) {
            PVIDEO_CTX_COMMAND ThisCmd = &VdCtx->Commands[i];
            SDL_Surface* Surface;
            
            switch (ThisCmd->VcType) {
                case VCTYPE_RECT:
                    SDL_SetRenderDrawColor(VdCtx->Renderer, R(ThisCmd->XArgs.Color),
                        G(ThisCmd->XArgs.Color), B(ThisCmd->XArgs.Color), A(ThisCmd->XArgs.Color));
                    SDL_Rect Rectangle = { ThisCmd->x, ThisCmd->y, ThisCmd->w, ThisCmd->h };
                    SDL_RenderFillRect(VdCtx->Renderer, &Rectangle);
                    break;
                    
                case VCTYPE_OUTL:
                    SDL_SetRenderDrawColor(VdCtx->Renderer, R(ThisCmd->XArgs.Color),
                        G(ThisCmd->XArgs.Color), B(ThisCmd->XArgs.Color), A(ThisCmd->XArgs.Color));
                    SDL_Rect Rectangle2 = { ThisCmd->x, ThisCmd->y, ThisCmd->w, ThisCmd->h };
                    SDL_RenderDrawRect(VdCtx->Renderer, &Rectangle2);
                    break;
                    
                case VCTYPE_COPY:
                    Surface = SDL_CreateRGBSurfaceFrom(
                        ThisCmd->XArgs.Buffer, ThisCmd->w, ThisCmd->h, 32, 32, RMASK, GMASK,
                        BMASK, AMASK);
                    SDL_Texture* TargetTexture = SDL_CreateTextureFromSurface(VdCtx->Renderer,
                        Surface);
                    SDL_FreeSurface(Surface);
                    SDL_Rect DestRect = { ThisCmd->x, ThisCmd->y, ThisCmd->w, ThisCmd->h };
                    SDL_RenderCopy(VdCtx->Renderer, TargetTexture, NULL, &DestRect);
                    SDL_DestroyTexture(TargetTexture);
                    break;
                    
                case VCTYPE_LINE:
                    SDL_SetRenderDrawColor(VdCtx->Renderer, R(ThisCmd->XArgs.Color),
                        G(ThisCmd->XArgs.Color), B(ThisCmd->XArgs.Color), A(ThisCmd->XArgs.Color));
                    SDL_RenderDrawLine(VdCtx->Renderer, ThisCmd->x,
                        ThisCmd->y, ThisCmd->w, ThisCmd->h);
                    break;
            }
        }
        SDL_UnlockMutex(VdCtx->Signal_VideoReady);
        
    }
    
    return;
}

void VideoShutdown(void) {
    return;
}

// video

WORD64 VideoGetTextBuffer(void) {
    return 0x00;
}

void VideoSetTextBuffer(WORD64 NewOffset) {
    return;
}

void VideoDrawLine(WORD16 x1, WORD16 y1, WORD16 x2, WORD16 y2, WORD32 color) {
    return;
}

void VideoDrawRect(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD32 color) {
    return;
}

void VideoDrawFill(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD32 color) {
    return;
}

void VideoCopyRect(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD64 ptr) {
    return;
}

WORD64 VideoGetWidthHeight(void) {
    return 0x00;
}

void VideoSuggestSize(WORD16 w, WORD16 h) {
    return;
}
