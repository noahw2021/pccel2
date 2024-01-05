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
    
    PINI_ENTRY NewEntry = &File->Entries[File->EntryCount];
    File->EntryCount++;
    
    strcpy(NewEntry->Name, FieldName);
    strcpy(NewEntry->Value, Default);
    NewEntry->Numerical = strtoull(NewEntry->Value, NULL, 10);
    
    return;
}

PINI_ENTRY IniReadField(PINI_FILE File, const char* FieldName) {
    if (!File)
        return NULL;
    
    for (int i = 0; i < File->EntryCount; i++) {
        PINI_ENTRY ThisEntry = &File->Entries[i];
        if (!strcmp(ThisEntry->Name, FieldName))
            return ThisEntry;
    }
    
    return NULL;
}

void IniWriteField(PINI_FILE File, PINI_ENTRY Entry) {
    for (int i = 0; i < File->EntryCount; i++) {
        PINI_ENTRY ThisEntry = &File->Entries[i];
        if (!strcmp(ThisEntry->Name, Entry->Name)) {
            memcpy(ThisEntry, Entry, sizeof(INI_ENTRY));
            break;
        }
    }
    
    return;
}

void IniDeleteField(PINI_FILE File, const char* FieldName) {
    if (!File)
        return;
    
    BYTE DidDeleteOccur = 0;
    PINI_ENTRY NewEntryList = malloc(sizeof(INI_ENTRY) *
        File->EntryCount - 1);
    
    int c = 0;
    for (int i = 0; i < File->EntryCount; i++) {
        PINI_ENTRY ThisEntry = &File->Entries[i];
        
        if (!strcmp(ThisEntry->Name, FieldName)) {
            DidDeleteOccur = 1;
            continue;
        } else {
            memcpy(&NewEntryList[c], ThisEntry, 
                sizeof(INI_ENTRY));
            c++;
        }
    }
    
    if (DidDeleteOccur) {
        free(File->Entries);
        File->Entries = NewEntryList;
    } else {
        free(NewEntryList);
    }
    
    return;
}
