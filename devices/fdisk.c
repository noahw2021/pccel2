//
//  fdisk.c
//  pccel2
//
//  Created by Noah Wooten on 12/30/23.
//

#include "devices.h"
#include "../emu.h"
#include <stdio.h>

typedef struct _FDISK_CTX {
    FILE* PhysicalDrive;
    WORD32 DriveMaxSize;
    
    WORD32 FilePointer;
    BYTE SkipIncrement;
}FDISK_CTX, *PFDISK_CTX;
PFDISK_CTX FdCtx;

void FdiskInit(void) {
    FdCtx = malloc(sizeof(FDISK_CTX));
    memset(FdCtx, 0, sizeof(FDISK_CTX));
    
    FdCtx->PhysicalDrive = fopen(EmuSettings->DiskRaw, "rb+");
    
    if (!FdCtx->PhysicalDrive) {
        FdCtx->PhysicalDrive = fopen(EmuSettings->DiskRaw, "wb+");
        if (!FdCtx->PhysicalDrive) {
            fprintf(EmuSettings->ErrorStream,
                "Failed to open virtual disk.\n");
            FdiskShutdown();
            return;
        }
    }
    
    FdCtx->DriveMaxSize = EmuSettings->DriveMaxSpace;
    return;
}
void FdiskShutdown(void) {
    if (!FdCtx)
        return;
    
    if (FdCtx->PhysicalDrive)
        fclose(FdCtx->PhysicalDrive);
    free(FdCtx);
    
    return;
}

void FdiskSetActiveDrive(WORD32 DriveId) {
    // here for test purposes, ccel2 does not support
    // mulitple drives, for now. - nw 1/8/24
    return;
}

WORD32  FdiskGetDriveCount(void) {
    return 1;
}

void FdiskDriveSleep(WORD32 DriveId) {
    // ccel2 does not support drive sleep/wake
    // so these do nothing - nw 1/8/24
    return;
}

void FdiskDriveAwake(WORD32 DriveId) {
    // ccel2 does not support drive sleep/wake
    // so these do nothing - nw 1/8/24
    return;
}
    
WORD64 FdiskGetDriveSize(WORD32 DriveId) {
    if (!FdCtx)
        return 0;
    
    return FdCtx->DriveMaxSize;
}
    
BYTE FdiskIsDriveReady(WORD32 DriveId){
    if (!FdCtx)
        return 0;
    
    return 1;
}
    
WORD64 FdiskDriveRead(WORD32 DriveId) {
    if (!FdCtx)
        return 0;
    
    fseek(FdCtx->PhysicalDrive, FdCtx->FilePointer, SEEK_SET);
    WORD64 Buffer;
    fread(&Buffer, sizeof(WORD64), 1, FdCtx->PhysicalDrive);
    
    switch (FdCtx->SkipIncrement) {
        case 0: // regular increment
            FdCtx->FilePointer += 0x8;
            break;
        case 1: // skip once
            FdCtx->SkipIncrement = 0;
            break;
        case 2: // skip always
            break;
    }
    
    return Buffer;
}
    
void FdiskDriveWrite(WORD32 DriveId, WORD64 Data) {
    if (!FdCtx)
        return;
    
    fseek(FdCtx->PhysicalDrive, FdCtx->FilePointer, SEEK_SET);
    WORD64 Buffer = Data;
    fread(&Buffer, sizeof(WORD64), 1, FdCtx->PhysicalDrive);
    
    switch (FdCtx->SkipIncrement) {
        case 0: // regular increment
            FdCtx->FilePointer += 0x8;
            break;
        case 1: // skip once
            FdCtx->SkipIncrement = 0;
            break;
        case 2: // skip always
            break;
    }
    
    return;
}
    
WORD64 FdiskGetDriveSerial(WORD32 DriveId) {
    if (!FdCtx)
        return 0;
    
    return 0x4271;
}
    
void FdiskGetDriveVendorString(WORD32 DriveId, WORD64 Pointer) {
    if (!FdCtx)
        return;
    
    char* StrPtr = (char*)Pointer;
    strcpy(StrPtr, "CCEL2 Emulated Drive");
    
    return;
}

WORD64 FdiskGetDriveModel(WORD32 DriveId) {
    if (!FdCtx)
        return 0;
    
    return 0x20004271;
}

WORD64 FdiskGetDriveVendorId(WORD32 DriveId) {
    if (!FdCtx)
        return 0;
    
    return 0x10004271;
}

void FdiskFarSeek(WORD32 DriveId, WORD64 SpecPos) {
    if (!FdCtx)
        return;
    
    FdCtx->FilePointer = SpecPos;
    fseek(FdCtx->PhysicalDrive, SpecPos, SEEK_SET);
    return;
}

void FdiskSkipFpIncrement(WORD32 DriveId) {
    if (!FdCtx)
        return;
    
    FdCtx->SkipIncrement = 1;
    
    return;
}

void FdiskEnableFpIncrement(WORD32 DriveId) {
    if (!FdCtx)
        return;
    
    FdCtx->SkipIncrement = 1;
    
    return;
}

void FdiskDisableFpIncrement(WORD32 DriveId) {
    if (!FdCtx)
        return;
    
    FdCtx->SkipIncrement = 0;
    
    return;
}

void FdiskDriveSeek(WORD32 DriveId, WORD64 NewPos) {
    if (!FdCtx)
        return;
    
    FdCtx->FilePointer = NewPos;
    // DriveSeek does not actually seek, like FarSeek does.
    return;
}

WORD64 FdiskDriveReadStack(WORD32 DriveId) {
    // these do nothing under a language such as C - nw 1/8/24
    return FdiskDriveRead(DriveId);
}

void FdiskDriveWriteStack(WORD32 DriveId, WORD64 Data) {
    // these do nothing under a language such as C - nw 1/8/24
    FdiskDriveWrite(DriveId, Data);
    return;
}
