//
//  ini_close.c
//  pccel2
//
//  Created by Noah Wooten on 1/5/24.
//

#include "ini.h"
#include <stdlib.h>

void IniCloseFile(PINI_FILE File) {
    if (!File)
        return;
    
    if (File->VFile) {
        for (int i = 0; i < File->EntryCount; i++) {
            PINI_ENTRY Entry = &File->Entries[i];
            
            fprintf(File->VFile, "%s=%s\n", Entry->Name,
                    Entry->Value);
        }
        
        fclose(File->VFile);
    }

    if (File->Entries)
        free(File->Entries);
    
    free(File);
    return;
}

