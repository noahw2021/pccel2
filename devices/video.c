//
//  video.c
//  pccel2
//
//  Created by Noah Wooten on 12/30/23.
//

#include "devices.h"
#include "SDL.h"

#include <stdlib.h>
#include <string.h>

#define R(x) (Uint8)((WORD32)(x & 0xFF000000) >> 24)
#define G(x) (Uint8)((WORD32)(x & 0x00FF0000) >> 16)
#define B(x) (Uint8)((WORD32)(x & 0x0000FF00) >> 8)
#define A(x) (Uint8)((WORD32)(x & 0x000000FF) >> 0)
#define RMASK 0x000000FF
#define GMASK 0x0000FF00
#define BMASK 0x00FF0000
#define AMASK 0xFF000000

#define HILOW64(h, l) ((h << 32) | l)

PVIDEO_CTX VdCtx;

void VideoInit(void) {
    VdCtx = malloc(sizeof(VIDEO_CTX));
    memset(VdCtx, 0, sizeof(VIDEO_CTX));
    
    VdCtx->Signal_Shutdown = SDL_CreateMutex();
    VdCtx->Signal_KeyModify = SDL_CreateMutex();
    VdCtx->Signal_VideoReady = SDL_CreateMutex();
    VdCtx->VcMutex = SDL_CreateMutex();
    VdCtx->w = 640;
    VdCtx->h = 480;
    
    VdCtx->Window = SDL_CreateWindow("PCCEL2/PLASM2 Layer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)VdCtx->w,
        (int)VdCtx->h, 0);
    
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
        while (SDL_PollEvent(&Event)) {
            if (Event.type == SDL_QUIT) {
                SDL_UnlockMutex(VdCtx->Signal_Shutdown);
                break;
            }
            
            if (Event.type == SDL_KEYUP) {
                SDL_LockMutex(VdCtx->Signal_KeyModify);
                
                KeyboardHandleKeyUp(Event.key.keysym.scancode);
                
                SDL_UnlockMutex(VdCtx->Signal_KeyModify);
            }
            
            if (Event.type == SDL_KEYDOWN) {
                SDL_LockMutex(VdCtx->Signal_KeyModify);
                
                KeyboardHandleKeyDown(Event.key.keysym.scancode);
                
                SDL_UnlockMutex(VdCtx->Signal_KeyModify);
            }
        }
        
        if (!SDL_TryLockMutex(VdCtx->Signal_Shutdown))
            break;
        
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
        VdCtx->CommandCount = 0;
        SDL_UnlockMutex(VdCtx->Signal_VideoReady);
        
    }
    
    return;
}

void VideoShutdown(void) {
    if (!VdCtx)
        return;
    
    SDL_DestroyRenderer(VdCtx->Renderer);
    SDL_DestroyWindow(VdCtx->Window);
    
    if (VdCtx->Commands)
        free(VdCtx->Commands);
    
    SDL_DestroyMutex(VdCtx->Signal_Shutdown);
    SDL_DestroyMutex(VdCtx->Signal_KeyModify);
    SDL_DestroyMutex(VdCtx->Signal_VideoReady);
    SDL_DestroyMutex(VdCtx->VcMutex);
    
    free(VdCtx);
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
    SDL_LockMutex(VdCtx->Signal_VideoReady);
    
    if (VdCtx->CommandCount >= VdCtx->CommandsMax) {
        VdCtx->CommandsMax += 100;
        VdCtx->Commands = realloc(VdCtx->Commands,
            sizeof(VIDEO_CTX_COMMAND) * VdCtx->CommandsMax);
    }
    
    PVIDEO_CTX_COMMAND ThisCmd = &VdCtx->Commands[VdCtx->CommandCount++];
    ThisCmd->x = x1;
    ThisCmd->y = y1;
    ThisCmd->w = x2;
    ThisCmd->h = y2;
    ThisCmd->XArgs.Color = color;
    ThisCmd->VcType = VCTYPE_LINE;
    
    SDL_UnlockMutex(VdCtx->Signal_VideoReady);
    return;
}

void VideoDrawRect(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD32 color) {
    SDL_LockMutex(VdCtx->Signal_VideoReady);
    
    if (VdCtx->CommandCount >= VdCtx->CommandsMax) {
        VdCtx->CommandsMax += 100;
        VdCtx->Commands = realloc(VdCtx->Commands,
            sizeof(VIDEO_CTX_COMMAND) * VdCtx->CommandsMax);
    }
    
    PVIDEO_CTX_COMMAND ThisCmd = &VdCtx->Commands[VdCtx->CommandCount++];
    ThisCmd->x = x;
    ThisCmd->y = y;
    ThisCmd->w = w;
    ThisCmd->h = h;
    ThisCmd->XArgs.Color = color;
    ThisCmd->VcType = VCTYPE_OUTL;
    
    SDL_UnlockMutex(VdCtx->Signal_VideoReady);
    return;
}

void VideoDrawFill(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD32 color) {
    SDL_LockMutex(VdCtx->Signal_VideoReady);
    
    if (VdCtx->CommandCount >= VdCtx->CommandsMax) {
        VdCtx->CommandsMax += 100;
        VdCtx->Commands = realloc(VdCtx->Commands,
            sizeof(VIDEO_CTX_COMMAND) * VdCtx->CommandsMax);
    }
    
    PVIDEO_CTX_COMMAND ThisCmd = &VdCtx->Commands[VdCtx->CommandCount++];
    ThisCmd->x = x;
    ThisCmd->y = y;
    ThisCmd->w = w;
    ThisCmd->h = h;
    ThisCmd->XArgs.Color = color;
    ThisCmd->VcType = VCTYPE_RECT;
    
    SDL_UnlockMutex(VdCtx->Signal_VideoReady);
    return;
}

void VideoCopyRect(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD64 ptr) {
    SDL_LockMutex(VdCtx->Signal_VideoReady);
    
    if (VdCtx->CommandCount >= VdCtx->CommandsMax) {
        VdCtx->CommandsMax += 100;
        VdCtx->Commands = realloc(VdCtx->Commands,
            sizeof(VIDEO_CTX_COMMAND) * VdCtx->CommandsMax);
    }
    
    PVIDEO_CTX_COMMAND ThisCmd = &VdCtx->Commands[VdCtx->CommandCount++];
    ThisCmd->x = x;
    ThisCmd->y = y;
    ThisCmd->w = w;
    ThisCmd->h = h;
    ThisCmd->XArgs.Buffer = (void*)ptr;
    ThisCmd->VcType = VCTYPE_COPY;
    
    SDL_UnlockMutex(VdCtx->Signal_VideoReady);
    return;
}

WORD64 VideoGetWidthHeight(void) {
    return HILOW64(VdCtx->w, VdCtx->h);
}

void VideoSuggestSize(WORD16 w, WORD16 h) {
    VdCtx->w = w;
    VdCtx->h = h;
    SDL_SetWindowSize(VdCtx->Window, w, h);
    return;
}
