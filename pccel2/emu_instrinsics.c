//
//  emu_instrinsics.c
//  pccel2
//
//  Created by Noah Wooten on 12/26/23.
//

#include "../pccel2_exports.h"

void __shf(void) {
    EmuCtx->Cpus->Flags.HF = 1;
    return;
}

// Virtual Memory
void __vme(void) {
    EmuCtx->Cpus->Flags.VF = 1;
}

void __vmd(void){
    EmuCtx->Cpus->Flags.VF = 0;
}

WORD64 __vpc(WORD64 Phys, WORD64 Size, WORD64 Permissions) {
    EmuCtx->Pages = (PMMU_PAGE)EmuCtx->SystemRam + CpuCtx->ControlRegisters.PageStart;
    int PageCount = (int)(CpuCtx->ControlRegisters.PageEnd - CpuCtx->ControlRegisters.PageStart);
    PageCount /= sizeof(MMU_PAGE);
    
    for (int i = 0; i < PageCount; i++) {
        PMMU_PAGE ThisPage = &EmuCtx->Pages[i];
        
        if (ThisPage->Permissions.Active)
            continue;
        
        ThisPage->Size = Size;
        ThisPage->Physical = Phys;
        ThisPage->Permissions.Raw = Permissions;
        ThisPage->Virtual = CpuCtx->ControlRegisters.VirtualStackPointer;
        
        CpuCtx->ControlRegisters.VirtualStackPointer += Size + 0x4200;
        
        return ThisPage->Virtual;
    }
    
    return 0;
}

void __vpd(WORD64 Virt) {
    EmuCtx->Pages = (PMMU_PAGE)EmuCtx->SystemRam + CpuCtx->ControlRegisters.PageStart;
    int PageCount = (int)(CpuCtx->ControlRegisters.PageEnd - CpuCtx->ControlRegisters.PageStart);
    PageCount /= sizeof(MMU_PAGE);
    
    for (int i = 0; i < PageCount; i++) {
        PMMU_PAGE ThisPage = &EmuCtx->Pages[i];
        
        if (ThisPage->Permissions.Active)
            continue;
        
        if (InRange(Virt, ThisPage->Virtual, ThisPage->Virtual + ThisPage->Size)) {
            ThisPage->Physical = 0x00;
            ThisPage->Permissions.Active = 0;
        }
        
        return;
    }
    
    return;
}

void __vsi(void) {
    CpuCtx->Security.SecurityLevel++;
    return;
}

void __vsd(void) {
    if (!CpuCtx->Flags.AF)
        CpuCtx->Security.SecurityLevel--;
    return;
}

void __vss(WORD64 New) {
    CpuCtx->ControlRegisters.PageStart = New;
    return;
}

void __ves(WORD64 End) {
    CpuCtx->ControlRegisters.PageEnd = End;
}

// Memory
void __psr(CPU_CTX Ctx) {
    for (int i = 0; i < 16; i++)
        CpuCtx->GPRs[i] = Ctx.GPRs[i];
    
    if (CpuCtx->Security.SecurityLevel <= 1) {
        for (int i = 0;i < 16; i++)
            CpuCtx->SystemRs[i] = Ctx.SystemRs[i];
    }
    return;
}

CPU_CTX __por(void) {
    CPU_CTX Return = {0};
    for (int i = 0; i < 16; i++) {
        Return.GPRs[i] = CpuCtx->GPRs[i];
        Return.SystemRs[i] = CpuCtx->SystemRs[i];
    }
    
    return Return;
}

// Devices
WORD64 __dsq(WORD64 Device) {
    PCPU_DEVICE Devices = (PCPU_DEVICE)EmuCtx->SystemRam +
        CpuCtx->ControlRegisters.DeviceMap;
    PCPU_DEVICE ThisDevice = &Devices[Device];
    
    return 
}

void __dsc(WORD64 Device, WORD64 Command) {
     
}

void __dsd(WORD64 Device, WORD64 Data) {
    
}

WORD64 __dgd(WORD64 Device) {
    return 0x00;
}

void __dpe(WORD64 Device) {
    
}

void __dpd(WORD64 Device) {
    
}

WORD64 __dgc(void) {
    return 0x00;
}

// Interrupts
void __int(WORD64 Interrupt) {
    
}
void __hnd(WORD64 Interrupt, WORD64 VAddr){
    
}

void __irt(void){
    
}

void __eni(void){
    
}

void __dsi(void) {
    
}

void __smh(WORD64 Handler) {
    
}

void __sit(WORD64 Handler) {
    
}
