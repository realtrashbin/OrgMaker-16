#include "TrackFlag.h"

#include "Setting.h"
#include "OrgData.h"
#include "DefOrg.h"
#include "Sound.h"

signed long* OrgFlagsX; //Contains the X-data for an OrgFlag
signed short** OrgFlags; //Contains all modifications an OrgFlag does
/*
* 0. Mode
* 1. Track
* 2. Wave No.
* 3. Pipi
* 4. Freq.
*/
unsigned short** OrgFlagsUndo; //Contains info on how to undo an OrgFlag
/*
* 0. Wave No.
* 1. Pipi
* 2. Freq.
*/

char MaxFlag = 32;
bool ResetFlagsOnLoop = false;

void FlagAllocate(char num) {
	char i;
	MaxFlag = num;
	if (OrgFlags != NULL) {
		free(OrgFlagsX);
		free(*OrgFlags);
		free(OrgFlags);
		free(*OrgFlagsUndo);
		free(OrgFlagsUndo);
		OrgFlagsX = NULL;
		*OrgFlags = NULL;
		OrgFlags = NULL;
		*OrgFlagsUndo = NULL;
		OrgFlagsUndo = NULL;
		if (!MaxFlag) { MessageBox(NULL, "TEST", "TEST", MB_OK); return; }
	}

	OrgFlagsX = (signed long*)calloc(MaxFlag*sizeof(long), sizeof(long));
	OrgFlags = (signed short**)calloc(MaxFlag*sizeof(short), sizeof(short));
	if (OrgFlags) for (i = 0; i <= MaxFlag - 1; i++) OrgFlags[i] = new signed short[5];
	OrgFlagsUndo = (unsigned short**)calloc(MAXTRACK*sizeof(short), sizeof(short)); //This just gets the original state of every track
	if (OrgFlagsUndo) for (i = 0; i <= MAXTRACK - 1; i++) OrgFlagsUndo[i] = new unsigned short[3];
}

void FlagInit(void) {
	char i;
	MUSICINFO info;
	if (OrgFlagsX != NULL) {
		for (i = 0; i < MaxFlag; i++)
		{
			if (OrgFlagsX[i] != -1) org_data.CutFlag(OrgFlagsX[i]);
		}
	}

	for (i = 0; i < MaxFlag; i++)
	{
			OrgFlagsX[i] = -1;
			for (char j = 0; j < 5; j++) OrgFlags[i][j] = 0;
	}

	org_data.GetMusicInfo(&info);
	for (char i = 0; i < MAXTRACK; i++)
	{
		OrgFlagsUndo[i][0] = info.tdata[i].wave_no;
		OrgFlagsUndo[i][1] = info.tdata[i].pipi;
		OrgFlagsUndo[i][2] = info.tdata[i].freq;
	}
}

char FlagFinder(long x,bool clearflag)
{
	//Used for anything relating to clicking and flags
	//Ex. Deleting a flag, it has to get the flag ID to clear it obviously.
	
	char i;

	for (i = 0; i < MaxFlag; i++)
	{
		if (OrgFlagsX[i] == x) break;
	}

	if (MaxFlag == i) return -1;

	if (clearflag == true)
	{
		OrgFlagsX[i] = -1;
		for (char j = 0; j < 4; j++) OrgFlags[i][j] = 0;
	}
	return i; //returns flag
}

void FlagsMoveActivate(long x)
{
	char i;
	MUSICINFO mi;

	Rxo_StopAllSoundNow();
	for (i = 0; i < MAXTRACK; i++)
	{
		org_data.GetMusicInfo(&mi);
		mi.tdata[i].wave_no = OrgFlagsUndo[i][0];
		if (i < MAXMELODY) {
			mi.tdata[i].pipi = OrgFlagsUndo[i][1];
			mi.tdata[i].freq = OrgFlagsUndo[i][2];
			MakeOrganyaWave(i, OrgFlagsUndo[i][0], OrgFlagsUndo[i][1]); //Melody
			org_data.SetMusicInfo(&mi, SETWAVE | SETPIPI | SETFREQ);
		}
		else InitDramObject(OrgFlagsUndo[i][0],i-MAXMELODY); org_data.SetMusicInfo(&mi, SETWAVE);
	}
	
	for (i = 0; i < MaxFlag; i++)
	{
		if (OrgFlagsX[i] <= x && OrgFlagsX[i] != -1) OrgFlagsEnable(i);
	}
}

void OrgFlagsEnable(unsigned char flag)
{
	MUSICINFO mi;
	if (flag == FLAGDUMMY) return;
	switch (OrgFlags[flag][0])
	{
		Rxo_StopTrackSound((char)OrgFlags[flag][1]);
		case 1:
		{
			org_data.GetMusicInfo(&mi);
			if (OrgFlags[flag][1] < MAXMELODY) {
				mi.tdata[OrgFlags[flag][1]].wave_no = (unsigned char)OrgFlags[flag][2];
				mi.tdata[OrgFlags[flag][1]].pipi = (char)OrgFlags[flag][3];
				MakeOrganyaWave((char)OrgFlags[flag][1], (char)OrgFlags[flag][2], (char)OrgFlags[flag][3]); //Melody
				org_data.SetMusicInfo(&mi, SETWAVE | SETPIPI);
			}
			else
			{
				org_data.GetMusicInfo(&mi);
				mi.tdata[OrgFlags[flag][1]].wave_no = (unsigned char)OrgFlags[flag][2];
				InitDramObject((char)OrgFlags[flag][2], (int)OrgFlags[flag][1] - MAXMELODY);
				org_data.SetMusicInfo(&mi, SETWAVE);
			}
			return;
		}
		case 2:
		{
			org_data.GetMusicInfo(&mi);
			mi.tdata[OrgFlags[flag][1]].freq = (unsigned char)OrgFlags[flag][4];
			org_data.SetMusicInfo(&mi, SETFREQ);
			return;
		}
		default:
		{
			return;
		}
	}
}