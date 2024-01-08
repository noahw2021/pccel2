//
//  pccel2.c
//  pccel2
//
//  Created by Noah Wooten on 1/8/24.
//

#include "pccel2_exports.h"
#include "emu.h"

void Pccel2Init(FILE* ErrorStream) {
    EmuInit(ErrorStream);
}

void Pccel2Shutdown(void) {
    EmuShutdown();
}
