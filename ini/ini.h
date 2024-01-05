//
//  ini.h
//  pccel2
//
//  Created by Noah Wooten on 1/5/24.
//

#ifndef ini_h
#define ini_h

#include "../types.h"
#include <stdio.h>

typedef struct _INI_ENTRY {
    char Name[64];
    
    char Value[256];
    WORD64 Numerical;
}INI_ENTRY, *PINI_ENTRY;

typedef struct _INI_FILE {
    FILE* VFile;
    
    PINI_ENTRY Entries;
    WORD32 EntryCount;
}INI_FILE, *PINI_FILE;

PINI_FILE IniOpenFile(const char* Name);
PINI_FILE IniCreateFile(const char* Name);
void IniCloseFile(PINI_FILE File);

BYTE IniDoesFieldExist(PINI_FILE File, const char* FieldName);
void IniCreateField(PINI_FILE File, const char* FieldName, 
    const char* Default);
PINI_ENTRY IniReadField(PINI_FILE File, const char* FieldName);
void IniWriteField(PINI_FILE File, PINI_ENTRY Entry);
void IniDeleteField(PINI_FILE File, const char* FieldName);

#endif /* ini_h */
