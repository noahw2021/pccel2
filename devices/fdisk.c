//
//  fdisk.c
//  pccel2
//
//  Created by Noah Wooten on 12/30/23.
//

#include "devices.h"
#include <stdio.h>

typedef struct _FDISK_CTX {
    FILE* PhysicalDrive;
    WORD32 DriveMaxSize;
}FDISK_CTX, *PFDISK_CTX;
PFDISK_CTX FdCtx;

void FdiskInit(void) {
    return;
}
void FdiskShutdown(void) {
    return;
}

void FdiskSetActiveDrive(WORD32 DriveId) {
    return;
}

WORD32  FdiskGetDriveCount(void) {
    return 0;
}

void FdiskDriveSleep(WORD32 DriveId) {
    return;
}

void FdiskDriveAwake(WORD32 DriveId) {
    return;
}
    
WORD64 FdiskGetDriveSize(WORD32 DriveId) {
    return 0;
}
    
BYTE FdiskIsDriveReady(WORD32 DriveId){
    return 0;
}
    
WORD64 FdiskDriveRead(WORD32 DriveId) {
    return 0;
}
    
void FdiskDriveWrite(WORD32 DriveId, WORD64 Data) {
    return;
}
    
WORD64 FdiskGetDriveSerial(WORD32 DriveId) {
    return 0;
}
    
void FdiskGetDriveVendorString(WORD32 DriveId, WORD64 Pointer) {
    return;
}

WORD64 FdiskGetDriveModel(WORD32 DriveId) {
    return 0;
}

WORD64 FdiskGetDriveVendorId(WORD32 DriveId) {
    return 0;
}

void FdiskFarSeek(WORD32 DriveId, WORD64 SpecPos) {
    return;
}

void FdiskSkipFpIncrement(WORD32 DriveId) {
    return;
}

void FdiskEnableFpIncrement(WORD32 DriveId) {
    return;
}

void FdiskDisableFpIncrement(WORD32 DriveId) {
    return;
}

void FdiskDriveSeek(WORD32 DriveId, WORD64 NewPos) {
    return;
}

WORD64 FdiskDriveReadStack(WORD32 DriveId) {
    return 0;
}

void FdiskDriveWriteStack(WORD32 DriveId, WORD64 Data) {
    return;
}
