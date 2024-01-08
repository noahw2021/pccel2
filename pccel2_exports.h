//
//  pccel2_exports.h
//  pccel2
//
//  Created by Noah Wooten on 12/24/23.
//

#ifndef pccel2_exports_h
#define pccel2_exports_h

#include "types.h"
#include <stdio.h>

#define CCEL2_API

CCEL2_API void Pccel2Init(FILE* ErrorStream);
CCEL2_API void Pccel2Shutdown(void);

typedef struct _CPU_CTX *PCPU_CTX;
typedef struct _MMU_PAGE *PMMU_PAGE;

typedef struct _EMULATOR_CTX {
    WORD64 TimerNs;
    WORD64 LastTickNs;
    WORD64 NextTickNs;
    
    BYTE* SystemRam;
    WORD64 SystemRamSize;
    
    void* VideoData;
    PCPU_CTX Cpus;
    PMMU_PAGE Pages;
}EMULATOR_CTX, *PEMULATOR_CTX;

typedef struct _MMU_PAGE {
    WORD64 Physical;
    WORD64 Virtual;
    WORD64 Size;
    union {
        WORD64 Raw;
        struct {
            WORD64 Secure : 1;
            WORD64 Selector : 1;
            WORD64 Read : 1;
            WORD64 Active : 1;
            
            WORD64 Reserved : 60;
        };
    }Permissions;
}MMU_PAGE, *PMMU_PAGE;


typedef union _PLASM2_CTX_FLAGS {
    WORD64 FlagsR;
    struct {
        BYTE GF : 1; // Greater flag
        BYTE EF : 1; // Equal flag
        BYTE ZF : 1; // Zero flag
        BYTE HF : 1; // Halt flag
        BYTE IF : 1; // Interrupt flag
        BYTE VF : 1; // Virtual flag
        BYTE XF : 1; // Exception flag
        BYTE NF : 1; // Next Execute skip flag
        BYTE OF : 1; // Overflow flag
        BYTE SF : 1; // StackSkip flag
        BYTE CF : 1; // Call flag
        BYTE AF : 1; // Extra security flag
        BYTE TF : 1; // Interrupt table set
        BYTE MF : 1; // Memory Guard Flag
        BYTE LF : 1; // Less than flag
    };
}PLASM2_CTX_FLAGS, *PPLASM2_CTX_FLAGS;

typedef union _PLASM2_CTX_SECURITY {
    WORD64 SecurityRaw;
    struct {
        WORD64 SecurityLevel : 5;
        
        WORD64 Reserved : 59;
    };
}PLASM2_CTX_SECURITY, *PPLASM2_CTX_SECURITY;

typedef union _PLASM2_CTX_CONTROLREGISTERS {
    WORD64 ControlRegistesrRaw[8];
    
    struct {
        WORD64 PageStart; // page start
        WORD64 PageEnd; // page end
        WORD64 ReturnAddressLocation; // return address location, backup stack pointer
        WORD64 InterruptTable; // interrupt table
        WORD64 VirtualStackPointer; // virtual trailing arm
        WORD64 CSMHandler; // csm handler
        WORD64 DeviceMap; // device map pointer
        WORD64 StackPointerUpperBound; // stack pointer upper bound
        WORD64 StackPointerLowerBound; // stack lower bound
        WORD64 PageMaxLocation; // page max location
        WORD64 NextCallAddress; // next call address
        
        WORD64 Reserved[1];
    };
}PLASM2_CTX_CONTROLREGISTERS, *PPLASM2_CTX_CONTROLREGISTERS;

typedef struct _CPU_CTX {
    // Non-dividible
    union {
        WORD64 Registers64[32];
        struct {
            WORD64 GPRs[16];
            WORD64 SystemRs[16];
        };
    };
    
    struct {
        WORD64 r0, r1, r2, r3, r4, r5, r6, r7, r8,
               r9, r10, r11, r12, r13, r14, r15;
        
        WORD64 ip;
        WORD64 sp;
       
        PLASM2_CTX_FLAGS Flags;
        PLASM2_CTX_SECURITY Security;
        PLASM2_CTX_CONTROLREGISTERS ControlRegisters;
    };
}CPU_CTX, *PCPU_CTX;

typedef union _CPU_DEVICE_FLAGS {
    WORD32 FlagsRaw;
    struct {
        WORD32 Active : 1;
        WORD32 On : 1;
        WORD32 Sleep : 1;
        WORD32 Hotswappable : 1;
    };
}CPU_DEVICE_FLAGS, *PCPU_DEVICE_FLAGS;

typedef struct _CPU_DEVICE {
    int DeviceType;
    char DeviceName[32];
    char DeviceVendor[32];
    WORD64 VendorId;
    WORD64 DeviceModel;
    WORD64 DeviceSerial;
    WORD64(*Callbacks[7])(WORD32 DeviceId, WORD64 Argument);
    CPU_DEVICE_FLAGS Flags;
}CPU_DEVICE, *PCPU_DEVICE;

#define DEVTYPE_KB          0x10000001
#define DEVTYPE_MOUSE       0x10000002
#define DEVTYPE_CPU         0x10000003
#define DEVTYPE_TERMINAL    0x10000004
#define DEVTYPE_VIDEO       0x10000005
#define DEVTYPE_FDISKC      0x10000006
#define DEVTYPE_RDISKC      0x10000007
#define DEVTYPE_NETWORK     0x10000008

#define DEVSTATUS_GOOD       0x1100100A
#define DEVSTATUS_FAIL       0x1100100B
#define DEVSTATUS_WARN       0x1100100C
#define DEVSTAUTS_INVL       0x11002000

#define DEVSTATUS_RSN_FIRST  0x00000000
#define DEVSTATUS_RSN_INVCMD 0x00010000
#define DEVSTATUS_RSN_CLEAR  0x00FF0000

#define PPCEL2_DEVICE_VIDEO 0x00
#define PPCEL2_DEVICE_KEYBD 0x01
#define PPCEL2_DEVICE_FDISK 0x02

extern PEMULATOR_CTX EmuCtx;
extern PCPU_CTX CpuCtx;

// General
CCEL2_API void __shf(void);

// Virtual Memory
CCEL2_API void __vme(void);
CCEL2_API void __vmd(void);
CCEL2_API WORD64 __vpc(WORD64 Phys, WORD64 Size, WORD64 Permissions);
CCEL2_API void __vpd(WORD64 Virt);
CCEL2_API void __vsi(void);
CCEL2_API void __vsd(void);
CCEL2_API void __vss(WORD64 New);
CCEL2_API void __ves(WORD64 End);

// Memory
CCEL2_API void __psr(CPU_CTX Ctx);
CCEL2_API CPU_CTX __por(void);

// Devices
CCEL2_API WORD64 __dsq(WORD64 Device);
CCEL2_API void __dsc(WORD64 Device, WORD64 Command);
CCEL2_API void __dsd(WORD64 Device, WORD64 Data);
CCEL2_API WORD64 __dgd(WORD64 Device);
CCEL2_API void __dpe(WORD64 Device);
CCEL2_API void __dpd(WORD64 Device);
CCEL2_API WORD64 __dgc(void);

// Interrupts
CCEL2_API void __int(WORD64 Interrupt);
CCEL2_API void __hnd(WORD64 Interrupt, WORD64 VAddr);
CCEL2_API void __irt(void);
CCEL2_API void __eni(void);
CCEL2_API void __dsi(void);
CCEL2_API void __smh(WORD64 Handler);
CCEL2_API void __sit(WORD64 Handler);

CCEL2_API void MemoryRead(WORD64 Address, WORD64 Value);
CCEL2_API void MemoryWrite(WORD64 Address, WORD64 Value);

#endif /* pccel2_exports_h */
