//
//  posx_memory.c
//  pccel2
//
//  Created by Noah Wooten on 1/8/24.
//

#include "posx.h"
#include "../pccel2_exports.h"
#include "../emu.h"

#include <signal.h>

WORD64 PosxMemoryTranslate(WORD64 VirtualException) {
    if (!PosxCtx)
        return 0;
    
    if (!CpuCtx->Flags.VF)
        return VirtualException;
    
    PMMU_PAGE PageTable = (PMMU_PAGE)((WORD64)EmuCtx->SystemRam
        + CpuCtx->ControlRegisters.PageStart);
    WORD64 PageCount = (CpuCtx->ControlRegisters.PageEnd -
        CpuCtx->ControlRegisters.PageStart) / sizeof(MMU_PAGE);
    
    for (WORD64 i = 0; i < PageCount; i++) {
        PMMU_PAGE ThisPage = &PageTable[i];
        
        if (InRange(VirtualException, ThisPage->Virtual, 
            ThisPage->Virtual + ThisPage->Size)
        ) {
            WORD64 Offset = VirtualException - ThisPage->Virtual;
            return (WORD64)EmuCtx->SystemRam + 
                ThisPage->Physical + Offset;
        }
    }
    
    fprintf(EmuSettings->ErrorStream,
        "Invalid physical memory call: 0x%016llX\n",
        VirtualException);
    
    return 0;
}

void PosxiSignalHandler(int signo, siginfo_t* Info, void* Context) {
    if (!PosxCtx)
        return;
    
#if defined(__arm64) && defined(__MACH__)
    
    ucontext_t* UContext = (ucontext_t*)Context;
    _STRUCT_MCONTEXT* DarwinCtx = UContext->uc_mcontext;
    
    /*
     MemTestState: controls the memory test
     0 = not set yet.
     1 = register found
     */
    
    if (PosxCtx->MemTestState == 0) {
        for (int i = 0; i < 16; i++) {
            if (DarwinCtx->__ss.__x[i] == 0x006EB380) {
                PosxCtx->MemTestState = 1;
                PosxCtx->MemoryRegister = i;
                DarwinCtx->__ss.__x[i] = (WORD64)&PosxCtx->UselessPtr;
                return;
            }
        }
    }
    
    WORD64 Virtual = DarwinCtx->__ss.__x[PosxCtx->MemoryRegister];
    WORD64 Physical = PosxMemoryTranslate(Virtual);
    DarwinCtx->__ss.__x[PosxCtx->MemoryRegister] = Physical;
    
    return;
#else
    #error "No memory controller for platform."
#endif
}
