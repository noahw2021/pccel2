//
//  posx.h
//  pccel2
//
//  Created by Noah Wooten on 1/8/24.
//

#ifndef posx_h
#define posx_h

#include "../types.h"
#include <signal.h>

typedef struct _POSX_CTX {
    // emulator information
    void* OsEntryPoint;
    
    // phsyical memory information
    void* SystemRamBase;
    WORD64 MaximumRam;
    
    // memory emulator
    void* SignalHandler;
    BYTE MemoryRegister;
    BYTE MemTestState;
    WORD64 UselessPtr;
}POSX_CTX, *PPOSX_CTX;
extern PPOSX_CTX PosxCtx;

void PosxInit(void* OsEntryPoint, void* SystemRam,
    WORD64 RamMaxSize);
void PosxShutdown(void);

WORD64 PosxMemoryTranslate(WORD64 VirtualException);
void PosxiSignalHandler(int, siginfo_t*, void*);

void PosxCallInterrupt(BYTE Interrupt);
void PosxMainFunction(void);


#endif /* posx_h */
