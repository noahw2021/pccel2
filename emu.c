//
//  emu.c
//  pccel2
//
//  Created by Noah Wooten on 1/5/24.
//

#include "emu.h"
#include "ini/ini.h"
#include <stdlib.h>
#include <string.h>

PEMU_SETTINGS EmuSettings;

void EmuInit(FILE* Es) {
    EmuSettings = malloc(sizeof(EMU_SETTINGS));
    memset(EmuSettings, 0, sizeof(EMU_SETTINGS));
    
    EmuSettings->ErrorStream = Es;
    PINI_FILE IniFile = IniOpenFile("emu.ini");
   
    if (!IniFile) {
        IniFile = IniCreateFile("emu.ini");
        IniCreateField(IniFile, "FdiskBin", "hdd.bin");
        IniCreateField(IniFile, "FdiskSize", "16777216");
        IniCreateField(IniFile, "EmuPhysRam", "16777216");
        // 16mb
        
        EmuSettings->DriveMaxSpace = 16777216;
        EmuSettings->MemoryToAllocate = 16777216;
        strcpy(EmuSettings->DiskRaw, "hdd.bin");
    } else {
        PINI_ENTRY FdiskEntry = IniReadField(IniFile, 
            "FdiskBin");
        PINI_ENTRY FdSzeEntry = IniReadField(IniFile, 
            "FdiskSize");
        PINI_ENTRY RamSzEntry = IniReadField(IniFile, 
            "EmuPhysRam");
        
        strcpy(EmuSettings->DiskRaw, FdiskEntry->Name);
        EmuSettings->DriveMaxSpace = FdSzeEntry->Numerical;
        EmuSettings->MemoryToAllocate = RamSzEntry->Numerical;
        
        IniCloseFile(IniFile);
    }
    
    return;
}

void EmuShutdown(void) {
    if (EmuSettings)
        free(EmuSettings);
    
    return;
}
