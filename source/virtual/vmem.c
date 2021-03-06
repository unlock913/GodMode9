#include "vmem.h"
#include "unittype.h"

#define VFLAG_N3DS_ONLY     (1UL<<31)

// see: http://3dbrew.org/wiki/Memory_layout#ARM9
static const VirtualFile vMemFileTemplates[] = {
    { "itcm.mem"         , 0x01FF8000, 0x00008000, 0xFF, 0 },
    { "arm9.mem"         , 0x08000000, 0x00100000, 0xFF, 0 },
    { "arm9ext.mem"      , 0x08010000, 0x00080000, 0xFF, VFLAG_N3DS_ONLY },
    { "vram.mem"         , 0x18000000, 0x00600000, 0xFF, 0 },
    { "dsp.mem"          , 0x1FF00000, 0x00080000, 0xFF, 0 },
    { "axiwram.mem"      , 0x1FF80000, 0x00080000, 0xFF, 0 },
    { "fcram.mem"        , 0x20000000, 0x08000000, 0xFF, 0 },
    { "fcramext.mem"     , 0x28000000, 0x08000000, 0xFF, VFLAG_N3DS_ONLY },
    { "dtcm.mem"         , 0x30008000, 0x00004000, 0xFF, 0 },
    // { "otp.mem"          , 0x10012000, 0x00000108, 0xFF, 0 },
    { "bootrom.mem"      , 0xFFFF0000, 0x00010000, 0xFF, 0 },
    { "bootrom_unp.mem"  , 0xFFFF0000, 0x00008000, 0xFF, 0 },
    { "godmode9.bin"     , 0x23F00000, SELF_MAX_SIZE, 0xFF, 0 }
};

bool ReadVMemDir(VirtualFile* vfile, VirtualDir* vdir) { // uses a generic vdir object generated in virtual.c
    int n_templates = sizeof(vMemFileTemplates) / sizeof(VirtualFile);
    const VirtualFile* templates = vMemFileTemplates;
    
    while (++vdir->index < n_templates) {
        // copy current template to vfile
        memcpy(vfile, templates + vdir->index, sizeof(VirtualFile));
        
        // process special flag
        if ((vfile->flags & VFLAG_N3DS_ONLY) && (IS_O3DS))
            continue; // this is not on O3DS consoles
        
        // found if arriving here
        return true;
    }
    
    return false;
}

int ReadVMemFile(const VirtualFile* vfile, u8* buffer, u64 offset, u64 count) {
    u32 foffset = vfile->offset + offset;
    memcpy(buffer, (u8*) foffset, count);
    return 0;
}

int WriteVMemFile(const VirtualFile* vfile, const u8* buffer, u64 offset, u64 count) {
    u32 foffset = vfile->offset + offset;
    memcpy((u8*) foffset, buffer, count);
    return 0;
}
