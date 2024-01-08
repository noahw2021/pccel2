//
//  emu.h
//  pccel2
//
//  Created by Noah Wooten on 1/3/24.
//

#ifndef emu_h
#define emu_h

#include "types.h"

typedef struct _EMU_SETTINGS {
    char DiskRaw[64];
    WORD32 DriveMaxSpace;
    WORD32 MemoryToAllocate;
    
    FILE* ErrorStream;
}EMU_SETTINGS, *PEMU_SETTINGS;
extern PEMU_SETTINGS EmuSettings;

void EmuInit(FILE* Es);
void EmuShutdown(void);

void EmuInterrupt(BYTE Interrupt, WORD64 Argument);

#endif /* emu_h */
