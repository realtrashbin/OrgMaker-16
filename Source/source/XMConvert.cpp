/* Almost all the code is copied from org2xm by Pachuco
* Download: https://github.com/pachuco/org2xm/tree/master
rest of code is all me: Bi_N.*/


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Windows.h>

#include "Setting.h"

#define read(to, bytes) fread(to, 1, bytes, f)
#define write(from, bytes) fwrite(from, 1, bytes, g)
#define VOL 51

int key, finetune;

uint8_t wKey, wInst, wVol, wFine, wPan, wPanVol, wSkip;
uint8_t* pat[256], patTable[256]; int patLen[256]; int patterns;

int instruments;    // number of used instruments
int tracks;         // number of tracks
int barLen;         // pattern length
int rows, bars;     // song length
int loop;           // does the song loop?

int compatibility;  // reset instrument at each note?
int verorg;         //version of loaded song


#pragma pack(push,1)
typedef struct OrgHeader
{
    uint8_t magic[6];
    uint16_t msPerBeat;  // 1..2000
    uint8_t measuresPerBar;
    uint8_t beatsPerMeasure;
    uint32_t loopStart;  // in beats
    uint32_t loopEnd;
} OrgHeader; //header
#pragma pack(pop)

typedef struct Note
{
    uint32_t start;
    uint8_t len;
    uint8_t key;  // 0(C-0)..96(B-7)
    uint8_t vol;  // 0..255
    uint8_t pan;  // 0..12
} *note[16];

typedef struct Note16
{
    uint32_t start;
    uint8_t len;
    uint8_t key;  // 0(C-0)..96(B-7)
    uint8_t vol;  // 0..255
    uint8_t pan;  // 0..12
} *note16[32];

///////////////////////////////////////////////////// Immediate representation
#pragma pack(push,1)
typedef struct Instrument
{
    uint16_t freqShift;
    uint8_t sample;  // melody(voice 0..7) 0-99, drums(voice 8..15) 0-11
    uint8_t noLoop;
    uint16_t notes;

    uint8_t drum;
    uint8_t instrument;
    int8_t finetune;

    int8_t lastPan;  // encoding state
    uint8_t lastVol;
    uint8_t played;
} Instrument;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct Instrument16
{
    uint16_t freqShift;
    uint8_t sample;  // melody(voice 0..7) 0-99, drums(voice 8..15) 0-11
    uint8_t noLoop;
    uint16_t notes;

    uint8_t drum;
    uint8_t instrument;
    int8_t finetune;

    int8_t lastPan;  // encoding state
    uint8_t lastVol;
    uint8_t played;
} Instrument16;
#pragma pack(pop)

struct Track
{
    float freq;  // if (freq != 0), a new note starts
    uint8_t vol;      // 1..64; if (volume == 0), the note ends
    int8_t pan;      // -127=left..127=right
} *n[16];

struct Track16
{
    float freq;  // if (freq != 0), a new note starts
    uint8_t vol;      // 1..64; if (volume == 0), the note ends
    int8_t pan;      // -127=left..127=right
} *no[32];

#pragma pack(push,1)
typedef struct XMHeader
{
    uint8_t id[17];
    uint8_t moduleName[20];
    uint8_t eof;
    uint8_t trackerName[20];
    uint16_t version;
    uint32_t headerSize;
    uint16_t songLength;  // in patterns
    uint16_t restartPosition;
    uint16_t channels;  // should be even, but we don't care ;)
    uint16_t patterns;
    uint16_t instruments;
    uint16_t flags;
    uint16_t tempo;
    uint16_t bpm;
    uint8_t patternOrder[256];
    //No clue what some of this stuff below is.
    char* modulename = "Extended Module: ";
    char* EString = "";
    char* copyright = "Org2XM by Rrrola    ";
    char thing = 0x1A;
    short thing2 = 0x104;
    short thing3 = 0x114;
} XMH;

struct XMInstrument
{
    uint32_t size;
    uint8_t instrumentName[22];
    uint8_t zero;
    uint16_t samples;
    uint32_t sampleHeaderSize;
    uint8_t misc[230];
    uint32_t sampleLength;
    uint32_t loopStart;
    uint32_t loopLength;
    uint8_t volume;
    int8_t finetune;
    uint8_t type;
    uint8_t panning;
    uint8_t relativeKey;
    uint8_t reserved;
    uint8_t sampleName[22];
    //They really should've named these...
    short thing4 = 0x107;
    char* track = "Melody-00";
    bool thing5 = false;
    bool thing6 = true;
    char thing7 = 0x28;
    char EArry[1] = {};
    short max = 256;
    bool thing8 = false;
    short max2 = 256;
    char Volume = VOL;
    bool thing9 = false;
    bool thing10 = true;
    unsigned char thing11 = 128;
    char thing12 = 48;
    bool thing13 = false;
    char* Estring2 = "";
};
#pragma pack(pop)

typedef struct SoundBank
{
    uint8_t magic[6];
    uint8_t verbank;
    uint8_t verorg;
    uint8_t snumMelo;
    uint8_t snumDrum;
    uint16_t lenMelo;
    uint32_t* tblLenDrum;
    char(*tblNameDrum)[22];
    int8_t* melody;
    int8_t* drums;

    uint32_t* tblOffDrum;
    uint32_t lenAllDrm;
} sndbank;

OrgHeader header;
Instrument t[16];
Instrument16 tr[32];



int SBKload(char* path)
{
    FILE* f;
    uint32_t i, j;
    uint32_t fSize;
    sndbank sbank;
    if (!(f = fopen(path, "rb"))) goto Err10;
    //file size, just in case
    fseek(f, 0, SEEK_END);
    fSize = ftell(f);
    rewind(f);

    //read sample bank
    //header
    read(&sbank.magic, 6);
    if (memcmp(sbank.magic, "ORGBNK", 6)) goto Err11;

    //bank version
    read(&sbank.verbank, 1);
    //Organya song version this bank is intended for
    read(&sbank.verorg, 1);

    //number of melody samples
    read(&sbank.snumMelo, 1);
    //number of drums
    read(&sbank.snumDrum, 1);

    //length of each melody sample
    uint16_t tmp = 0;
    uint16_t x = 0;
    read(&x, 1); tmp = (tmp << 8) + x;
    read(&x, 1); tmp = (tmp << 8) + x;
    sbank.lenMelo = tmp;

    //drum sample length and offset tables
    sbank.tblLenDrum = malloc(sbank.snumDrum * sizeof(uint32_t));
    sbank.tblOffDrum = malloc(sbank.snumDrum * sizeof(uint32_t));
    sbank.lenAllDrm = 0;
    uint32_t off = 0;
    for (i = 0; i < sbank.snumDrum; i++)
    {
        uint32_t tmp = 0;
        uint32_t x = 0;
        read(&x, 1); tmp = (tmp << 8) + x;
        read(&x, 1); tmp = (tmp << 8) + x;
        read(&x, 1); tmp = (tmp << 8) + x;
        read(&x, 1); tmp = (tmp << 8) + x;
        sbank.tblLenDrum[i] = tmp;
        sbank.lenAllDrm += tmp;
        //offsets
        sbank.tblOffDrum[i] = off;
        off += tmp;
    }

    //drum sample names
#define MAXSTR 23
    sbank.tblNameDrum = malloc(sbank.snumDrum * sizeof(char[MAXSTR]));
    for (i = 0; i < sbank.snumDrum; i++)
        for (j = 0; j < MAXSTR; j++)
        {
            char c = (char)fgetc(f);
            sbank.tblNameDrum[i][j] = j == MAXSTR - 1 ? '\0' : c;
            if (c == '\0') break;
        }

    //melody waves
    sbank.melody = malloc(sbank.snumMelo * sbank.lenMelo * 1);
    read(sbank.melody, sbank.snumMelo * sbank.lenMelo * 1);

    //drum waves
    sbank.drums = malloc(sbank.lenAllDrm * 1);
    read(sbank.drums, sbank.lenAllDrm * 1);

    //delta encode samples
    int8_t* buf;
    //melody
    for (i = 0; i < sbank.snumMelo; i++)
    {
        buf = &sbank.melody[i * sbank.lenMelo];
        for (j = sbank.lenMelo - 1; j > 0; --j)
            buf[j] -= buf[j - 1];
    }
    //drums
    for (i = 0; i < sbank.snumDrum; i++)
    {
        buf = &sbank.drums[sbank.tblOffDrum[i]];
        for (j = sbank.tblLenDrum[i] - 1; j > 0; --j)
            buf[j] -= buf[j - 1];
        buf[0] ^= 0x80;
    }

    fclose(f);

    return 0;
    //errors
Err11: printf("Invalid samplebank header!\n");  return 11;
Err10: printf("Couldn't open samplebank!\n");   return 10;
}

////////////////////////////////////////////////////////// XM pattern encoding
// returns whatever is supposed to be encoded for (track i, row j)
// assumes that (i, j-1) has already been processed