#ifndef _VBE_H_
#define _VBE_H_

typedef struct _VgaInfo {
    char VESASignature[4];
    unsigned short VESAVersion;
    char *OEMString;
    unsigned int Capabilities;
    unsigned short *VideoModes;
    unsigned short TotalMemory;
    char Reserved[236];
} VgaInfo;

typedef struct _ModeInfo {
    unsigned short ModeAttributes;
    unsigned char WinAAttributes;
    unsigned char WinBAttributes;
    unsigned short WinGranularity;
    unsigned short WinSize;
    unsigned short WinASegment;
    unsigned short WinBSegment;
    unsigned char *WinFuncPtr;
    unsigned short BytesPerScanLine;
    unsigned short XResolution;
    unsigned short YResolution;
    unsigned char XCharSize;
    unsigned char YCharSize;
    unsigned char NumberOfPlanes;
    unsigned char BitsPerPixel;
    unsigned char NumberOfBanks;
    unsigned char MemoryModel;
    unsigned char BankSize;
    unsigned char NumberOfImagePages;
    unsigned char ReservedPageFn;
    unsigned char RedMaskSize;
    unsigned char RedFieldPosition;
    unsigned char GreenMaskSize;
    unsigned char GreenFieldPosition;
    unsigned char BlueMaskSize;
    unsigned char BlueFieldPosition;
    unsigned char RsvdMaskSize;
    unsigned char RsvdFieldPosition;
    unsigned char DirectColorModeInfo;
    unsigned char Reserved[216];
} ModeInfo;

typedef struct _PMInfo {
    unsigned short SetBankLen;
    unsigned char *SetBank;
    unsigned short GetBankLen;
    unsigned char *GetBank;
    unsigned short SetDisplayStartLen;
    unsigned char *SetDisplayStart;
} PMInfo;

unsigned int cdecl GenWinFuncPtr(void);

int vbe_InitBuffer(void);
int vbe_GetVgaInfo(VgaInfo *a1);
int vbe_GetModeInfo(unsigned int a1, ModeInfo *a2, unsigned int *a3, unsigned int *a4);
int vbe_GetPMInfo(PMInfo *a1);
int vbe_SetSvgaMode(unsigned int a1);
int vbe_GetVideoMode(unsigned int *a1);
int vbe_GetVideoStateSize(unsigned int a1, unsigned int *a2);
int vbe_SaveVideoState(unsigned int a1, void *a2, unsigned int a3);
int vbe_RestoreVideoState(unsigned int a1, void *a2, unsigned int a3);
int cdecl vbe_SetWindowPosition(unsigned int a1, unsigned int a2);
int cdecl vbe_GetWindowPosition(unsigned int a1, unsigned int *a2);
int vbe_SetScanLineLength(unsigned int *a1, unsigned int *a2, unsigned int *a3);
int vbe_GetScanLineLength(unsigned int *a1, unsigned int *a2, unsigned int *a3);
int vbe_SetDisplayStart(unsigned int a1, unsigned int a2);
int vbe_GetDisplayStart(unsigned int *a1, unsigned int *a2);
int vbe_SetDacWidth(unsigned int *a1);
int vbe_GetDacWidth(unsigned int *a1);

#endif // !_VBE_H_
