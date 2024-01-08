//
//  platforms.h
//  pccel2
//
//  Created by Noah Wooten on 1/8/24.
//

#ifndef platforms_h
#define platforms_h

#include "types.h"

void PlatInit(void* OsEntry);
void PlatShutdown(void);

WORD64 PlatMemoryTranslate(WORD64 VirtualException);
void PosxCallInterrupt(BYTE Interrupt);
void PosxMainFunction(void);

#endif /* platforms_h */
