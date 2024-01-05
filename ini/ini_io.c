//
//  ini_io.c
//  pccel2
//
//  Created by Noah Wooten on 1/5/24.
//

#include "ini.h"

BYTE IniDoesFieldExist(PINI_FILE File, const char* FieldName) {
    return 0;
}

void IniCreateField(
    PINI_FILE File, const char* FieldName,
    const char* Default
) {
    return;
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
