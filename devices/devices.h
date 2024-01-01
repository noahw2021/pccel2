//
//  devices.h
//  pccel2
//
//  Created by Noah Wooten on 12/30/23.
//

#ifndef devices_h
#define devices_h

#include "../types.h"

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
