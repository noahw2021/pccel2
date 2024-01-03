//
//  devices.h
//  pccel2
//
//  Created by Noah Wooten on 12/30/23.
//

#ifndef devices_h
#define devices_h

#include "../types.h"
#include "SDL.h"

#define SDL_Mutex SDL_mutex
#define SDL_Semaphore SDL_sem

typedef union _VIDEO_CTX_XARGS {
    void* Buffer;
    char* Text;
    WORD32 Color;
}VIDEO_CTX_XARGS, *PVIDEO_CTX_XARGS;

#define VCTYPE_RECT 0x00
#define VCTYPE_OUTL 0x01
#define VCTYPE_COPY 0x02
#define VCTYPE_LINE 0x03

typedef struct _VIDEO_CTX_COMMAND {
    WORD32 VcType;
    int x, y, w, h;
    VIDEO_CTX_XARGS XArgs;
}VIDEO_CTX_COMMAND, *PVIDEO_CTX_COMMAND;

typedef struct _VIDEO_CTX {
    PVIDEO_CTX_COMMAND Commands;
    WORD32 CommandCount, CommandsMax;
    
    WORD32 w, h;
    SDL_Window* Window;
    SDL_Renderer* Renderer;
    SDL_Mutex* VcMutex;
    SDL_Mutex* Signal_Shutdown;
    SDL_Mutex* Signal_VideoReady;
    SDL_Mutex* Signal_KeyModify;
}VIDEO_CTX, *PVIDEO_CTX;
extern PVIDEO_CTX VdCtx;

// Video functions
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

// Keyboard functions
void KeyboardInit(void);
void KeyboardShutdown(void);

void KeyboardSetKeyDownInterrupt(WORD64 Interrupt);
void KeyboardSetKeyUpInterrupt(WORD64 Interrupt);
WORD64 KeyboardGetKeyMapPointer(void);
void KeyboardHandleKeyUp(BYTE ScanCode);
void KeyboardHandleKeyDown(BYTE ScanCode);


// Fdisk functions
void FdiskInit(void);
void FdiskShutdown(void);

void FdiskSetActiveDrive(WORD32 DriveId);
WORD32  FdiskGetDriveCount(void);
void FdiskDriveSleep(WORD32 DriveId);
void FdiskDriveAwake(WORD32 DriveId);
WORD64 FdiskGetDriveSize(WORD32 DriveId);
BYTE FdiskIsDriveReady(WORD32 DriveId);
WORD64 FdiskDriveRead(WORD32 DriveId);
void FdiskDriveWrite(WORD32 DriveId, WORD64 Data);
WORD64 FdiskGetDriveSerial(WORD32 DriveId);
void FdiskGetDriveVendorString(WORD32 DriveId, WORD64 Pointer);
WORD64 FdiskGetDriveModel(WORD32 DriveId);
WORD64 FdiskGetDriveVendorId(WORD32 DriveId);
void FdiskFarSeek(WORD32 DriveId, WORD64 SpecPos);
void FdiskSkipFpIncrement(WORD32 DriveId);
void FdiskEnableFpIncrement(WORD32 DriveId);
void FdiskDisableFpIncrement(WORD32 DriveId);
void FdiskDriveSeek(WORD32 DriveId, WORD64 NewPos);
WORD64 FdiskDriveReadStack(WORD32 DriveId);
void FdiskDriveWriteStack(WORD32 DriveId, WORD64 Data);


#endif /* devices_h */
