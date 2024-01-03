//
//  keyboard.c
//  pccel2
//
//  Created by Noah Wooten on 12/30/23.
//

#include "../emu.h"
#include "devices.h"
#include <stdlib.h>
#include <string.h>

typedef struct _KB_CTX {
    BYTE* KeyMap;
    BYTE Interrupts[2]; // 0 = up, 1 = down
}KB_CTX, *PKB_CTX;
PKB_CTX KbCtx;

void KeyboardInit(void) {
    KbCtx = malloc(sizeof(KB_CTX));
    memset(KbCtx, 0, sizeof(KB_CTX));
    
    // 0x23F0 is the VM's address, luckily we can use this
    // address thanks to the memory manager without it being
    // an access violation -nw 1/3/24
    KbCtx->KeyMap = (BYTE*)0x23F0;
    
    return;
}

void KeyboardShutdown(void) {
    if (KbCtx)
        free(KbCtx);
    
    return;
}

void KeyboardSetKeyDownInterrupt(WORD64 Interrupt) {
    KbCtx->Interrupts[1] = Interrupt;
    return;
}

void KeyboardSetKeyUpInterrupt(WORD64 Interrupt) {
    KbCtx->Interrupts[0] = Interrupt;
    return;
}

WORD64 KeyboardGetKeyMapPointer(void) {
    return (WORD64)KbCtx->KeyMap;
}

void KeyboardHandleKeyUp(BYTE ScanCode) {
    KbCtx->KeyMap[ScanCode] = 0;
    EmuInterrupt(KbCtx->Interrupts[0], ScanCode);
    return;
}

void KeyboardHandleKeyDown(BYTE ScanCode) {
    KbCtx->KeyMap[ScanCode] = 1;
    EmuInterrupt(KbCtx->Interrupts[1], ScanCode);
    return;
}
