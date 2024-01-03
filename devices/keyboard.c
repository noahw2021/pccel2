//
//  keyboard.c
//  pccel2
//
//  Created by Noah Wooten on 12/30/23.
//

#include "devices.h"
#include <stdlib.h>
#include <string.h>

typedef struct _KB_CTX {
    BYTE KeyMap[256];
    BYTE Interrupts[2]; // 0 = up, 1 = down
}KB_CTX, *PKB_CTX;
PKB_CTX KbCtx;

void KeyboardInit(void) {
    return;
}

void KeyboardShutdown(void) {
    return;
}

void KeyboardSetKeyDownInterrupt(WORD64 Interrupt) {
    return;
}

void KeyboardSetKeyUpInterrupt(WORD64 Interrupt) {
    return;
}

WORD64 KeyboardGetKeyMapPointer(void) {
    return 0x00;
}

void KeyboardHandleKeyUp(BYTE ScanCode) {
    return;
}

void KeyboardHandleKeyDown(BYTE ScanCode) {
    return;
}
