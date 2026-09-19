
#define MAXNOTELENGTH	255//How long a note can be.
#define MAXMELODY		16//How many melody tracks there is.
#define MAXDRAM			16//How many dram tracks there is.
#define MAXTRACK		32 //32

#define MAXMELODY8		8
#define MAXDRAM8		24
#define MAXTRACK8		16

#define KEYDUMMY		255//default number for keys
#define PANDUMMY		255//default number for Panning
#define VOLDUMMY		255//default number for Volume
#define FLAGDUMMY		255//default number for Flags 

#define SETALL		0xffffffff//flags.
#define SETWAIT		0x00000001
#define SETGRID		0x00000002
#define SETALLOC	0x00000004
#define SETREPEAT	0x00000008
#define SETFREQ		0x00000010 //16
#define SETWAVE		0x00000020 //32
#define SETPIPI		0x00000040 //64


#define ALLOCNOTE	4096 //Memory Allocation for Notes. Don't change to a higher value if u want to be compatable with other orgmakers
#define MODEPARADD	0 //Adds onto an extending note?
#define MODEPARSUB	1 //Subtracts from an extending note?
//PAR is for notes I think.
#define MODEMULTIPLY 10 //Something for Rxo's features.
#define MODEDECAY   64  //Something for Rxo's features.


#define NUMDRAMITEM	43	// 43 wav files so 43 Drams

#define MAXHORZMEAS	1000 // How many beats there are(GOD DAMN WHY IS IT SET TO 999!?)

