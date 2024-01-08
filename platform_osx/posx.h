//
//  posx.h
//  pccel2
//
//  Created by Noah Wooten on 1/8/24.
//

#ifndef posx_h
#define posx_h

#include "../types.h"

void PosxInit(void* OsEntryPoint);
void PosxShutdown(void);

WORD64 PosxMemoryTranslate(WORD64 VirtualException);
void PosxCallInterrupt(BYTE Interrupt);
void PosxMainFunction(void);


#endif /* posx_h */
