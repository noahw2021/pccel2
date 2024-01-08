//
//  posx_init.c
//  pccel2
//
//  Created by Noah Wooten on 1/8/24.
//

#include "posx.h"
#include "../emu.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

PPOSX_CTX PosxCtx;

void PosxInit(void* OsEntryPoint, void* SystemRam,
    WORD64 RamMaxSize
) {
    PosxCtx = malloc(sizeof(POSX_CTX));
    memset(PosxCtx, 0, sizeof(POSX_CTX));
    
    PosxCtx->OsEntryPoint = OsEntryPoint;
    PosxCtx->SystemRamBase = SystemRam;
    PosxCtx->MaximumRam = RamMaxSize;
    
    struct sigaction Sa;
    Sa.sa_sigaction = PosxiSignalHandler;
    Sa.sa_flags = SA_SIGINFO;
    if (sigaction(SIGSEGV, &Sa, NULL) == -1) {
        fprintf(EmuSettings->ErrorStream,
                "Failed to register debug helper.\n");

        PosxShutdown();
        return;
    }
    
    // find the correct register
    // 0x006EB380 is a random address, that is
    // unlikely (impossible) to be a valid address in the host
    // machine. - nw 1/8/24
    WORD64* Pointer = (WORD64*)0x006EB380;
    fprintf(EmuSettings->ErrorStream,
        "Memory Test Value: %llu\n", *Pointer);
    
    
    return;
}

void PosxShutdown(void) {
    if (!PosxCtx)
        return;
    
    free(PosxCtx);
    
    return;
}
