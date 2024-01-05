//
//  ini_init.c
//  ;;2
//
//  Created by Noah Wooten on 1/5/24.
//

#include "ini.h"
#include <stdlib.h>
#include <string.h>

PINI_FILE IniOpenFile(const char* Name) {
    FILE* _File = fopen(Name, "r+");
    if (!_File)
        return NULL;
    
    PINI_FILE File = malloc(sizeof(INI_FILE));
    memset(File, 0, sizeof(INI_FILE));
    
    File->VFile = _File;
    
    char* LineBuffer = malloc(512);
    while (!feof(_File)) {
        fgets(LineBuffer, 512, _File);
        if (!strstr(LineBuffer, "="))
            continue;
        
        if (!File->Entries) {
            File->Entries = malloc(sizeof(INI_ENTRY));
        } else {
            File->Entries = realloc(File->Entries,
                sizeof(INI_ENTRY) * (File->EntryCount + 1));
        }
        
        PINI_ENTRY NewEntry = &File->Entries[File->EntryCount];
        File->EntryCount++;
        
        // read key name
        int i = 0, b = 0;
        while (LineBuffer[i] != '=') {
            NewEntry->Name[i] = LineBuffer[i];
            i++;
        }
        b = i;
        i = 0;
        
        // read key value
        while (LineBuffer[i] != '\0') {
            NewEntry->Value[i] = LineBuffer[b + i];
            i++;
        }
        
        NewEntry->Numerical = strtoull(NewEntry->Value,
            NULL, 10);
    }
    
    free(LineBuffer);
    
    return File;
}

PINI_FILE IniCreateFile(const char* Name) {
    FILE* _File = fopen(Name, "w+");
    if (!_File)
        return NULL;
    
    PINI_FILE File = malloc(sizeof(INI_FILE));
    memset(File, 0, sizeof(INI_FILE));
    
    File->VFile = _File;
    return File;
}
