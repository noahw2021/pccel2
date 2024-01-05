//
//  ini_io.c
//  pccel2
//
//  Created by Noah Wooten on 1/5/24.
//

#include "ini.h"
#include <string.h>
#include <stdlib.h>

BYTE IniDoesFieldExist(PINI_FILE File, const char* FieldName) {
    if (!File)
        return 0;
    
    for (int i = 0; i < File->EntryCount; i++) {
        PINI_ENTRY ThisEntry = &File->Entries[i];
        
        if (!strcmp(ThisEntry->Name, FieldName))
            return 1;
    }
    
    return 0;
}

void IniCreateField(
    PINI_FILE File, const char* FieldName,
    const char* Default
) {
    if (!File)
        return;
    
    if (!File->Entries) {
        File->Entries = malloc(sizeof(INI_ENTRY));
        File->EntryCount++;
    } else {
        File->Entries = realloc(File->Entries,
            sizeof(INI_ENTRY) * (File->EntryCount + 1));
    }
}

PINI_ENTRY IniReadField(PINI_FILE File, const char* FieldName) {
    return NULL;
}

void IniWriteField(PINI_FILE File, PINI_ENTRY Entry) {
    return;
}

void IniDeleteField(PINI_FILE File, const char* FieldName) {
    return;
}
