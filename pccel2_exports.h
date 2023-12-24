//
//  pccel2_exports.h
//  pccel2
//
//  Created by Noah Wooten on 12/24/23.
//

#ifndef pccel2_exports_h
#define pccel2_exports_h

void Pccel2Init(void);
void Pccel2Shutdown(void);

typedef struct _EMULATOR_CTX {
    WORD64 TimerNs;
    WORD64 LastTickNs;
    WORD64 NextTickNs;
    
    BYTE* SystemRam;
    WORD64 SystemRamSize;
    
    void* VideoData;
}EMULATOR_CTX, *PEMULATOR_CTX;

typedef struct _CPU_CTX {
    union {
        WORD64 Registers[32];
        
        struct {
            WORD64 GPRs[16];
            WORD64 System[16];
        };
        
        struct {
            WORD64 r0, r1, r2, r3, r4, r5, r6, r7;
            WORD64 r8, r9, r10, r11, r12, r13, r14, r15;
            WORD64 ip, sp;
            
            union {
                WORD64 FlagsRaw;
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
                }Flags;
            };
            
            union {
                WORD64 SecurityRaw;
                struct {
                    BYTE SecurityLevel : 5;
                    WORD64 Reserved : 59;
                }Security;
            };
            
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
            }ControlRegisters;
        };
    };
}CPU_CTX, *PCPU_CTX;

extern PEMULATOR_CTX EmuCtx;
extern PCPU_CTX CpuCtx;

#endif /* pccel2_exports_h */
