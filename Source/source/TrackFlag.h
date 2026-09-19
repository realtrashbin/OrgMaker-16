int binTrackCode(char *str);
void TitlebarRefresh(void);

void FlagAllocate(char num);
void FlagInit(void);
char FlagFinder(long x, bool clearflag);
void FlagsMoveActivate(long x);
void OrgFlagsEnable(unsigned char flag);

extern signed long* OrgFlagsX; //Contains the X-data for an OrgFlag
extern signed short** OrgFlags; //Contains all modifications an OrgFlag does
extern unsigned short** OrgFlagsUndo; //Contains info on how to undo an OrgFlag

extern char MaxFlag;
extern char OrgFlagsDlg;
extern bool ResetFlagsOnLoop;