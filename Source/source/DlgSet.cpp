#include "Setting.h"
#include "DefOrg.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>

#include "Sound.h"
#include "resource.h"
#include "OrgData.h"
#include "Gdi.h"
#include "Filer.h"
#include <commctrl.h>
#include "Mouse.h"
#include "rxoFunction.h"
#include "TrackFlag.h"

#define PI 3.14159265358979323846

#define NUMGRID		16
#define NUMGRIDA		11
#define MAXWAVE		100
#define maxx(a, b) ((a) > (b) ? (a) : (b))

extern HWND hDlgTrack;
extern HWND hDlgPlayer;
extern HWND hDlgEZCopy;
extern char* gSelectedTheme;
extern char* gSelectedWave;

extern char TrackN[];

extern HBITMAP waveBmp; // azy


int volChangeLength = 10;
bool volChangeUseNoteLength = true;
bool volChangeSetNoteLength = false;
bool FlagR;

typedef struct{
	char name[20];
	char line;
	char dot;
}GRID;
GRID grid[NUMGRIDA] = {
	{"☆自由設定",0,0},
	{"４拍：４分割",4,4},
	{"４拍：３分割",4,3},
	{"３拍：４分割",3,4},
	{"３拍：３分割",3,3},
	{"４拍：６分割",4,6},
	{"３拍：６分割",3,6},
	{"４拍：２分割",4,2},
	{"４拍：８分割",4,8},
	{"４拍：12分割",4,12},
	{"５拍：４分割",5,4},
};
//IDS_GRID_STRINGに!区切りで追加すること。ここの文字列はﾀﾞﾐｰです。

int check_pipi[NUMGRID] ={
	IDC_CHECK_PIPI0,
	IDC_CHECK_PIPI1,
	IDC_CHECK_PIPI2,
	IDC_CHECK_PIPI3,
	IDC_CHECK_PIPI4,
	IDC_CHECK_PIPI5,
	IDC_CHECK_PIPI6,
	IDC_CHECK_PIPI7,
	IDC_CHECK_PIPI8,
	IDC_CHECK_PIPI9,
	IDC_CHECK_PIPI10,
	IDC_CHECK_PIPI11,
	IDC_CHECK_PIPI12,
	IDC_CHECK_PIPI13,
	IDC_CHECK_PIPI14,
	IDC_CHECK_PIPI15
};

int txt_Pan[MAXTRACK]={
	IDC_PANPOT1, IDC_PANPOT2 , IDC_PANPOT3, IDC_PANPOT4, IDC_PANPOT5, IDC_PANPOT6, IDC_PANPOT7, IDC_PANPOT8, 
	IDC_PANPOT9, IDC_PANPOT10, IDC_PANPOT11, IDC_PANPOT12, IDC_PANPOT13, IDC_PANPOT14, IDC_PANPOT15, IDC_PANPOT16,
	IDC_PANPOTDxQ,IDC_PANPOTDxW,IDC_PANPOTDxE,IDC_PANPOTDxR,IDC_PANPOTDxT,IDC_PANPOTDxY,IDC_PANPOTDxU,IDC_PANPOTDxI,
	IDC_PANPOTDxA,IDC_PANPOTDxS,IDC_PANPOTDxD,IDC_PANPOTDxF,IDC_PANPOTDxG,IDC_PANPOTDxH,IDC_PANPOTDxJ,IDC_PANPOTDxK
};

int txt_Vol[MAXTRACK]={
	IDC_VOLUME1, IDC_VOLUME2, IDC_VOLUME3, IDC_VOLUME4, IDC_VOLUME5, IDC_VOLUME6, IDC_VOLUME7, IDC_VOLUME8,
	IDC_VOLUME9, IDC_VOLUME10, IDC_VOLUME11, IDC_VOLUME12, IDC_VOLUME13, IDC_VOLUME14, IDC_VOLUME15, IDC_VOLUME16,
	IDC_VOLUMEDxQ, IDC_VOLUMEDxW, IDC_VOLUMEDxE,IDC_VOLUMEDxR,IDC_VOLUMEDxT,IDC_VOLUMEDxY,IDC_VOLUMEDxU,IDC_VOLUMEDxI,
	IDC_VOLUMEDxA,IDC_VOLUMEDxS,IDC_VOLUMEDxD,IDC_VOLUMEDxF,IDC_VOLUMEDxG,IDC_VOLUMEDxH,IDC_VOLUMEDxJ,IDC_VOLUMEDxK,
};

int Preset1_Pan[]={
	6, 6, 5, 4, 8, 9, 3, 7,
	6, 7, 5, 5, 6, 6, 8, 4,
	4, 7, 5, 2, 4, 6, 3, 5,
	5, 8, 3, 6, 2, 1, 9, 3
};
int Preset1_Vol[]={
	180, 210, 170, 150, 150, 190, 120, 140,
	180, 190, 225, 230, 195, 200, 180, 190,
	180, 150, 200, 237, 210, 180, 155, 165,
	180, 164, 225, 160, 155, 122, 111, 100
};


char* strBracketTrack[MAXTRACK] = { "[1]","[2]","[3]", "[4]","[5]","[6]","[7]","[8]","[9]","[10]","[11]","[12]","[13]","[14]","[15]","[16]",
"[Q]","[W]","[E]","[R]","[T]","[Y]","[U]","[I]","[A]","[S]","[D]","[F]","[G]","[H]","[J]","[K]" };

char* strTrack[MAXTRACK] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16",
"Q","W","E","R","T","Y","U","I","A","S","D","F","G","H","J","K"};

int SamplePlayHeight = 36; //サンプル音を鳴らす高さ

int SaveWithInitVolFile;	//曲データと…セーブするか。

extern HINSTANCE hInst;//インスタンスハンドル
extern void ClearEZC_Message(); //EZメッセージと範囲を消す
extern void ReloadBitmaps();
extern void GetApplicationPath(char* path);

double GetNormDistRand(){
	double x, y;
	double s, t;
	double r1, r2;

	// 一様分布に従う擬似乱数 x, y を作る
	do { x = (double)rand()/(double)RAND_MAX; } while (x == 0.0); // x が 0 になるのを避ける
	y = (double)rand()/(double)RAND_MAX;

	// s, t の計算
	s = sqrt(-2.0 * log(x)); // C, Java では log が自然対数
	t = 2.0 * PI * y;

	// 標準正規分布に従う擬似乱数 r1, r2
	r1 = s * cos(t);  r2 = s * sin(t);
	return r2;
}

void SetText(HWND hdwnd, int ID_TEXTBOX, char *str)
{
	SetDlgItemText(hdwnd, ID_TEXTBOX , str);
}

void SetText(HWND hdwnd, int ID_TEXTBOX, int iValue)
{
	char str[128] = {NULL};
	itoa(iValue,str,10);
	SetDlgItemText(hdwnd, ID_TEXTBOX , str);
}

int GetText(HWND hdwnd, int ID_TEXTBOX)
{
	char str[32] = {NULL};
	GetDlgItemText(hdwnd,ID_TEXTBOX,str,31);
	int r;
	r = atoi(str);
	return r;
}

void EnableDialogWindow(int iValue = TRUE)
{
	EnableWindow(hDlgPlayer,iValue); //The thing that plays the orgs
	EnableWindow(hDlgTrack,iValue); //holds the tracks
	EnableWindow(hDlgEZCopy,iValue); //Weird white bar thingy Rxo added

}


//ダイアログ内容の初期化
void InitSettingDialog(HWND hdwnd)
{
	char str[128] = {NULL};
	long i,a;
	MUSICINFO mi;
	org_data.GetMusicInfo(&mi);
//	//編集トラック情報の初期化//////////////////
//	itoa(org_data.track,str,10);
//	SetDlgItemText(hdwnd,IDD_SETTRACK,str);
	//再生ウエイトの初期化//////////////////
	itoa(mi.wait,str,10);
	SetDlgItemText(hdwnd,IDD_SETWAIT,str);
	snprintf(str, 128, "%.2f", (60000.0 / (double)(mi.wait * mi.dot)));
	SetDlgItemText(hdwnd, IDC_BPM, str);
	//ｸﾞﾘｯﾄﾞの初期化
	TCHAR *q, *p;
	p = MessageString[IDS_GRID_STRING];
	for(i = 0; i < NUMGRIDA; i++){
		for(q = grid[i].name; *p != 0; p++, q++)*q = *p; //実質strcpy
		*q = 0; p++; //ポインタを'\0'の次に
	}

	//表示グリッドの初期化//////////////////
	for(i = 0; i < NUMGRIDA; i++)//リストボックスの初期化
		SendDlgItemMessage(hdwnd,IDD_LB1,LB_ADDSTRING,0,(LPARAM)grid[i].name);
	for(i = 0; i < NUMGRIDA; i++)//現在使用中のグリッドを検索
		if(mi.line == grid[i].line && mi.dot == grid[i].dot)break;
	if(i == NUMGRIDA)i = 0;//無ければ０を設定
	
	if(i > 0){
		a = grid[i].line;
		itoa(a,str,10);
		SetDlgItemText(hdwnd,IDD_GRIDEDIT1,str);
		a = grid[i].dot;
		itoa(a,str,10);
		SetDlgItemText(hdwnd,IDD_GRIDEDIT2,str);
		EnableWindow(GetDlgItem(hdwnd, IDD_GRIDEDIT1), FALSE); //自由設定テキスト無効化
		EnableWindow(GetDlgItem(hdwnd, IDD_GRIDEDIT2), FALSE);
	}else{
		EnableWindow(GetDlgItem(hdwnd, IDD_GRIDEDIT1), TRUE); //自由設定テキスト有効化
		EnableWindow(GetDlgItem(hdwnd, IDD_GRIDEDIT2), TRUE);
		a = mi.line;
		itoa(a,str,10);
		SetDlgItemText(hdwnd,IDD_GRIDEDIT1,str);
		a = mi.dot;
		itoa(a,str,10);
		SetDlgItemText(hdwnd,IDD_GRIDEDIT2,str);
	}

	SendDlgItemMessage(hdwnd,IDD_LB1,LB_SETCURSEL,i,0);
	//リピート範囲の初期化//////////////////
	//CheckDlgButton(hdwnd, IDC_CHECK_PRECISELR, preciselr ? 1 : 0);
	a = mi.repeat_x / (mi.dot * mi.line);
	itoa(a,str,10);
	SetDlgItemText(hdwnd,IDD_REP_MEAS,str);
	a = mi.end_x / (mi.dot * mi.line);
	itoa(a,str,10);
	SetDlgItemText(hdwnd,IDD_END_MEAS,str);
	// this one is better
	a = mi.repeat_x % (mi.dot * mi.line);
	itoa(a, str, 10);
	SetDlgItemText(hdwnd, IDD_REP_BEAT, str);
	a = mi.end_x % (mi.dot * mi.line);
	itoa(a, str, 10);
	SetDlgItemText(hdwnd, IDD_END_BEAT, str);
	//の初期化//////////////////

	//MessageBox(NULL, "メッセージループを抜けました", "OK", MB_OK);
	itoa(MaxFlag, str, 10);
	SetDlgItemText(hdwnd, IDD_MAXFLAGS, str);
	CheckDlgButton(hdwnd, IDD_RESETFLAGS, ResetFlagsOnLoop);
}

//ウエイトの設定
BOOL SetWait(HWND hdwnd, MUSICINFO *mi)
{
	char str[128] = {NULL};
	long a;
	GetDlgItemText(hdwnd,IDD_SETWAIT,str,7);
	a = atol(str);
	if(a > 2000 || a < 1){ 
		MessageBox(hdwnd,"Wait cannot be below 1, nor exceed 2000.","Could not set Wait",MB_OK | MB_ICONERROR);	// 2014.10.19 D
		return FALSE;
	}
	mi->wait = (unsigned short)a;
	return TRUE;
}

//表示グリッドの設定
BOOL SetGrid(HWND hdwnd,MUSICINFO *mi)
{
	int i;
	//選択グリッドの設定///////////////////////
	i = SendDlgItemMessage(hdwnd, IDD_LB1,LB_GETCURSEL,0,0);//インデックスを得る
//	EndDialog(hdwnd,0);
	if(i>0){
		mi->dot = grid[i].dot;
		mi->line = grid[i].line;
	}else{
		char str[128] = {NULL};
		long a,b;
		GetDlgItemText(hdwnd,IDD_GRIDEDIT1,str,3);
		a = atol(str);
		if(a<=0 || a>=128){
			MessageBox(hdwnd,"Time Signature cannot be below 0 or above 128.","Could not set custom Beat",MB_OK | MB_ICONERROR);	// 2014.10.19 D
			return FALSE;
		}
		GetDlgItemText(hdwnd,IDD_GRIDEDIT2,str,3);
		b = atol(str);
		if(b<=0 || b>=128){
			MessageBox(hdwnd,"Time Signature cannot be below 0 or above 128.","Could not set custom Step",MB_OK | MB_ICONERROR);	// 2014.10.19 D
			return FALSE;
		}

		mi->line = (unsigned char)a;
		mi->dot = (unsigned char)b;

	}
	return TRUE;
}
//リピート範囲の設定
BOOL SetRepeat(HWND hdwnd, MUSICINFO *mi)
{
	char str[128] = {NULL};
	long a,b;
	GetDlgItemText(hdwnd,IDD_REP_MEAS,str,7);
	a = atol(str);
	GetDlgItemText(hdwnd, IDD_REP_BEAT, str, 7);
	b = atol(str);
	mi->repeat_x = (unsigned short)a * (mi->dot * mi->line) + b;
	GetDlgItemText(hdwnd,IDD_END_MEAS,str,7);
	a = atol(str);
	GetDlgItemText(hdwnd, IDD_END_BEAT, str, 7);
	b = atol(str);
	mi->end_x = (unsigned short)a * (mi->dot * mi->line) + b;
	if(mi->end_x <= mi->repeat_x){
		MessageBox(hdwnd,"Range is invalid.","Could not set Repeat Range",MB_OK | MB_ICONERROR);	// 2014.10.19 D
		return FALSE;
	}
	return TRUE;
}


int freqbox[MAXTRACK] = {
	IDD_SETFREQ0,
	IDD_SETFREQ1,
	IDD_SETFREQ2,
	IDD_SETFREQ3,

	IDD_SETFREQ4,
	IDD_SETFREQ5,
	IDD_SETFREQ6,
	IDD_SETFREQ7,

	IDD_SETFREQ8,
	IDD_SETFREQ9,
	IDD_SETFREQ10,
	IDD_SETFREQ11,

	IDD_SETFREQ12,
	IDD_SETFREQ13,
	IDD_SETFREQ14,
	IDD_SETFREQ15,

	IDD_SETFREQDxQ,
	IDD_SETFREQDxW,
	IDD_SETFREQDxE,
	IDD_SETFREQDxR,

	IDD_SETFREQDxT,
	IDD_SETFREQDxY,
	IDD_SETFREQDxU,
	IDD_SETFREQDxI,

	IDD_SETFREQDxA,
	IDD_SETFREQDxS,
	IDD_SETFREQDxD,
	IDD_SETFREQDxF,

	IDD_SETFREQDxG,
	IDD_SETFREQDxH,
	IDD_SETFREQDxJ,
	IDD_SETFREQDxK,
};

//曲の設定
BOOL CALLBACK DialogSetting(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	double iBPM;
	int iWAIT;
	char str[128] = {NULL};
	RECT rect = {64,0,WWidth,WHeight};//更新する領域
	MUSICINFO mi,mg;
	switch(message){
	case WM_INITDIALOG://ダイアログが呼ばれた
		InitSettingDialog(hdwnd);
		EnableDialogWindow(FALSE);
		return 1;
	case WM_CTLCOLOREDIT: //テキストが編集された。
		
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDC_BTN1:
			org_data.GetMusicInfo(&mi);
			GetDlgItemText(hdwnd, IDC_BPM, str, 128);
			iBPM = std::stod(str);
			if(iBPM > 0){
				SetGrid(hdwnd, &mg);
				iWAIT = (int)round(60000.0 / iBPM / (double)mi.dot);
				snprintf(str, 128, "%d", iWAIT);
				SetDlgItemText(hdwnd, IDD_SETWAIT, str);
			}
			return 1;
		case IDC_BTN2:
			org_data.GetMusicInfo(&mi);
			GetDlgItemText(hdwnd, IDD_SETWAIT, str, 128);
			iWAIT = atoi(str);
			if(iWAIT > 0){
				SetGrid(hdwnd, &mg);
				iBPM = 60000.0 / (double)(iWAIT * mi.dot);
				snprintf(str, 128, "%.2f", iBPM);
				SetDlgItemText(hdwnd, IDC_BPM, str);
			}
			return 1;
		case IDD_LB1:
			if(HIWORD(wParam) == LBN_SELCHANGE){ //ﾘｽﾄﾎﾞｯｸｽでの選択変更
				i = SendDlgItemMessage(hdwnd, IDD_LB1,LB_GETCURSEL,0,0);//インデックスを得る
				if(i == 0){
					EnableWindow(GetDlgItem(hdwnd, IDD_GRIDEDIT1), TRUE);
					EnableWindow(GetDlgItem(hdwnd, IDD_GRIDEDIT2), TRUE);
				}else{
					EnableWindow(GetDlgItem(hdwnd, IDD_GRIDEDIT1), FALSE);
					EnableWindow(GetDlgItem(hdwnd, IDD_GRIDEDIT2), FALSE);
				}
			}
			break;
		case IDD_REP_MEAS: case IDD_END_MEAS: case IDD_REP_BEAT: case IDD_END_BEAT: case IDD_SETWAIT: case IDC_BPM:
		case IDD_GRIDEDIT1: case IDD_GRIDEDIT2: case IDD_MAXFLAGS: case IDD_RESETFLAGS:
			if(HIWORD(wParam) == EN_SETFOCUS)PostMessage(GetDlgItem(hdwnd, LOWORD(wParam)), EM_SETSEL, 0, -1); //フォーカス時にテキストを全選択する
			return -1;
		case IDCANCEL:
			EndDialog(hdwnd,0);
			EnableDialogWindow(TRUE);
			return 1;
		case IDOK:
			org_data.GetMusicInfo( &mi );

			SetUndo();

			if(!SetWait(hdwnd, &mi))return 1;
			if(!SetGrid(hdwnd,&mi))return 1;
			if(!SetRepeat(hdwnd, &mi))return 1;

			GetDlgItemText(hdwnd, IDD_MAXFLAGS, str, 4);
			if (atoi(str) == NULL) { MessageBox(hdwnd, "Max Number of Flags cannot be zero.", "Could not set Max Number of Flags", MB_OK | MB_ICONERROR); return TRUE; }
			if (atoi(str) != MaxFlag) if (MessageBox(hdwnd, "Changing the number of Max Flags will erase all previous flag data. Are you sure you want to continue?", "Notice", MB_YESNO | MB_ICONQUESTION)
				== IDYES) { FlagAllocate(atoi(str)); FlagInit();}
			ResetFlagsOnLoop = IsDlgButtonChecked(hdwnd, IDD_RESETFLAGS) ? true : false;

			//プレイヤーに表示
			itoa(mi.wait,str,10);
			SetDlgItemText(hDlgTrack,IDE_VIEWWAIT,str);
			//パラメータを設定
			org_data.SetMusicInfo(&mi,SETGRID|SETWAIT|SETREPEAT);
			//波形の作りなおし
			//再描画
			//org_data.PutMusic();
			//RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
			EndDialog(hdwnd,0);
			EnableDialogWindow(TRUE);
			ClearEZC_Message(); //EZメッセージと範囲を消す
			return 1;
		}
	}
	return 0;
}
char *dram_name[] = { //Names for all the drams.
	"Bass01",//0
	"Bass02",//1
	"Snare01",//2

	"Snare02",//3
	"Tom01",//4
	"HiClose",//5
	
	"HiOpen",//6
	"Crash",//7
	"Per01",//8
	
	"Per02",//9
	"Bass03",//10
	"Tom02",//11
	
	"Bass04", //12
	"Bass05",//13
	"Snare03",//14

	"Snare04",//15
	"HiClose02",//16
	"HiOpen02",//17
	
	"HiClose03",//18
	"HiOpen03",//19
	"Crash02",//20
	
	"RevSym01",//21
	"Ride01",//22
	"Tom03",//23
	
	"Tom04",//24
	"OrcDrm01",	//25
	"Bell",//26
	
	"Cat" ,//27
	"Bass06",//28
	"Bass07",//29

	"Snare05",//30
	"Snare06",//31
	"Snare07",//32
	
	"Tom05",//33
	"HiOpen04",//34
	"HiClose04",//35
	
	"Clap01",//36
	"Pesi01",//37
	"Quick01",//38
	
	"Bass08" ,//39

	"Snare08",//40
	"HiClose05",//41
    "Per03",//42
	//Bi_N down.
	"Clap02", //43 -
	"Bass09",//44 -
	"Bass10",//45 -

	"HiClose06",//46 -
	"HiClose07",//47 -
	"Snare09",//48 -

	"Crash03",//49
	"Bass11",//50 -
	"Snare10",//51 -

	"HiClose08",//52 -
	"HiOpen05",//53 -
	"Tom06"//54 -
};

//アルファベット順変換定義
unsigned char Wave_no_to_List_no[] = { //Format as to how drums are shown in DLG_TRACK
	0,1,10,12,13,28,29,39,26,27,36,7,20,5,16,18,35,41,6,17,19,34,25,8,9,42,37,38,21,22,2,3,14,15,30,31,32,40,4,11,23,24,33
}; //0 = Bass, 2 = Snare, 5 = Hiclose, 6 = HiOpen, 4 = Tom

unsigned char List_no_to_Wave_no[] = { //Format that the program reads off of to play drams?
	0,1,30,31,38,13,18,11,23,24,2,39,3,4,32,33,14,19,15,20,12,28,29,40,41,22,8,9,5,6,34,35,36,42,21,16,10,26,27,7,37,17,25
}; 
/*unsigned char Wave_no_to_List_no[]={
	0,1,10,12,13,28,29,39,45,46,51,26,27,36,44,7,20,50,5,16,18,35,41,47,48,53,6,17,19,34,25,8,9,42,43,37,38,21,22,2,3,14,15,30,31,32,40,49,52,4,11,23,24,33,55
};
unsigned char List_no_to_Wave_no[]={
	0,1,30,31,38,13,18,11,23,24,2,39,3,4,32,33,14,19,15,20,12,28,29,40,41,22,8,9,5,6,34,35,36,42,21,16,10,26,27,7,37,17,25
};*/


void Sl_Reset(HWND hdwnd)
{
	int i;
	SendDlgItemMessage(hdwnd , IDC_SLIDER1 , TBM_SETRANGEMIN , FALSE, 0);
	SendDlgItemMessage(hdwnd , IDC_SLIDER1 , TBM_SETRANGEMAX , FALSE, 95);
	SendDlgItemMessage(hdwnd , IDC_SLIDER1 , TBM_CLEARTICS , FALSE, 0);
	for(i=1; i<MAXMELODY; i++){
		SendDlgItemMessage(hdwnd , IDC_SLIDER1 , TBM_SETTIC , 0, i*12);

	}
	SendDlgItemMessage(hdwnd , IDC_SLIDER1 , TBM_SETTICFREQ  , TRUE, 12);
	SendDlgItemMessage(hdwnd , IDC_SLIDER1 , TBM_SETPOS , TRUE, SamplePlayHeight);

}

void ChangePiPiActive(HWND hdwnd, char mode)
{
	char j;
	switch (mode)
	{
	case 0:
	{
		for (j = 0; j < 8; j++)
		{
			CheckDlgButton(hdwnd, IDC_CHECK_PIPIx0 + j, BST_UNCHECKED);
			CheckDlgButton(hdwnd, IDC_CHECK_PIPIx0 + j + 541, BST_UNCHECKED);
		}
		break;
	}
	case 1:
	{
		for (j = 0; j < 8; j++)
		{
			CheckDlgButton(hdwnd, IDC_CHECK_PIPIx0 + j, BST_CHECKED);
			CheckDlgButton(hdwnd, IDC_CHECK_PIPIx0 + j + 541, BST_UNCHECKED);
		}
		break;
	}
	case 2:
	{
		for (j = 0; j < 8; j++)
		{
			CheckDlgButton(hdwnd, IDC_CHECK_PIPIx0 + j, BST_UNCHECKED);
			CheckDlgButton(hdwnd, IDC_CHECK_PIPIx0 + j + 541, BST_CHECKED);
		}
		break;
	}
	case 3:
	{
		for (j = 0; j < 8; j++)
		{
			CheckDlgButton(hdwnd, IDC_CHECK_PIPIx0 + j, BST_CHECKED);
			CheckDlgButton(hdwnd, IDC_CHECK_PIPIx0 + j + 541, BST_CHECKED);
		}
		break;
	}

	}
}


BOOL CALLBACK DialogWave(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i, j;
	/*long mouse_x;
	long mouse_y;
	long mx, my,*/long n;
	char str[16];
	TCHAR* p;
	static MUSICINFO mi;
	static int iLastLBox = 0;
	static char PiPiMode;
	switch (message) {
	case WM_INITDIALOG://ダイアログが呼ばれた
		org_data.GetMusicInfo(&mi);
		//FREQ & PIPI
		for (j = 0; j < MAXMELODY; j++) {
			if (j < MaxFlag && OrgFlagsUndo[j][0] != mi.tdata[j].wave_no) {
				mi.tdata[j].freq = OrgFlagsUndo[j][2];
				mi.tdata[j].pipi = OrgFlagsUndo[j][1];
			}
			i = mi.tdata[j].freq; itoa(i, str, 10);
			if (j > 7)
			{
				SetDlgItemText(hdwnd, IDD_SETFREQx0 + j + 489, str);
				CheckDlgButton(hdwnd, IDC_CHECK_PIPIx0 + j + 533, (mi.tdata[j].pipi ? 1 : 0));
			}
			else
			{
				SetDlgItemText(hdwnd, IDD_SETFREQx0 + j, str);
				CheckDlgButton(hdwnd, IDC_CHECK_PIPIx0 + j, (mi.tdata[j].pipi ? 1 : 0));
			}
		}
		//メロディリストボックスの初期化

		for (j = 0; j < MAXMELODY; j++) {
			if (j < MaxFlag && OrgFlagsUndo[j][0] != mi.tdata[j].wave_no) mi.tdata[j].wave_no = OrgFlagsUndo[j][0];
			SendDlgItemMessage(hdwnd, freqbox[j], LB_RESETCONTENT, 0, 0);//初期化
			for (i = 0; i < MAXWAVE; i++) {
				if (i == mi.tdata[j].wave_no) {
					sprintf(str, MessageString[IDS_STRING117], i);	// 2010.09.30 D "Wave-%02d*"
				}
				else {
					sprintf(str, MessageString[IDS_STRING118], i);	// 2010.09.30 D "Wave-%02d"
				}
				SendDlgItemMessage(hdwnd, freqbox[j], LB_ADDSTRING, 0, (LPARAM)str);//(LPARAM)wave_name[i].name);
			}

			//最初のアイテムを選択
			SendDlgItemMessage(hdwnd, freqbox[j], LB_SETCURSEL, mi.tdata[j].wave_no, 0);
			SendDlgItemMessage(hdwnd, freqbox[j], LB_SETTOPINDEX, maxx(0, mi.tdata[j].wave_no - 9), 0);
		}
		//ドラムリストボックスの初期化
		for (j = MAXMELODY; j < MAXTRACK; j++) {
			SendDlgItemMessage(hdwnd, freqbox[j], LB_RESETCONTENT, 0, 0);//初期化
			for (i = 0; i < NUMDRAMITEM; i++) {
				SendDlgItemMessage(hdwnd, freqbox[j], LB_ADDSTRING, 0, (LPARAM)dram_name[Wave_no_to_List_no[i]]);
			}
			//最初のアイテムを選択
			SendDlgItemMessage(hdwnd, freqbox[j], LB_SETCURSEL, List_no_to_Wave_no[mi.tdata[j].wave_no], 0);
			SendDlgItemMessage(hdwnd, freqbox[j], LB_SETTOPINDEX, maxx(0, List_no_to_Wave_no[mi.tdata[j].wave_no] - 9), 0);
		}
		EnableDialogWindow(FALSE);
		iLastLBox = 0;
		for (j = 0; j < MAXMELODY; j++) {
			if (j == iLastLBox)SetDlgItemText(hdwnd, iLastLBox + IDC_LABEL_TRACK_0, strBracketTrack[iLastLBox]);
			else SetDlgItemText(hdwnd, j + IDC_LABEL_TRACK_0, strTrack[j]);
		}
		return 1;
	case WM_COMMAND:
		if ((LOWORD(wParam) >= IDD_SETFREQx0 && LOWORD(wParam) <= IDD_SETFREQx15) && (HIWORD(wParam) == EN_SETFOCUS)) {	// 2014.10.19 
			PostMessage(GetDlgItem(hdwnd, LOWORD(wParam)), EM_SETSEL, 0, -1); //フォーカス時にテキストを全選択する
			return -1;
		}
		if ((HWND)lParam == GetDlgItem(hdwnd, VS_VERSION_INFO) || (HWND)lParam == GetDlgItem(hdwnd, IDCANCEL)) {

			switch (LOWORD(wParam)) {
			case IDOK: //Save Button
				Rxo_StopAllSoundNow();
				SetUndo();
				n = 0;
				for (j = 0; j < MAXMELODY; j++) {
					if (j < 8)
					{
						GetDlgItemTextA(hdwnd, IDD_SETFREQx0 + j, str, MAXTRACK); i = atol(str); mi.tdata[j].freq = (i > 0x7530) ? 0x7530 : i;
					}
					else
					{
						GetDlgItemTextA(hdwnd, IDD_SETFREQx0 + j + 489, str, MAXTRACK); i = atol(str); mi.tdata[j].freq = (i > 0x7530) ? 0x7530 : i;
					}
					n |= (i > 30000) ? 1 : 0;
					if (j < 8)
					{
						mi.tdata[j].pipi = (IsDlgButtonChecked(hdwnd, IDC_CHECK_PIPIx0 + j)) ? 1 : 0;
					}
					else
					{
						mi.tdata[j].pipi = (IsDlgButtonChecked(hdwnd, IDC_CHECK_PIPIx0 + j + 533)) ? 1 : 0;
					}
					MakeOrganyaWave(j, mi.tdata[j].wave_no, mi.tdata[j].pipi); // add this so it updates if pipi changes
				}

				if (n)
				{
					MessageBox(hWnd, "Frequency must be lower than 30,000", "Could not set Frequency", MB_OK | MB_ICONERROR);	// 2014.10.19 A
					return 0; //Thank you for the return.
				}
				else
				{
					for (i = 0; i < MAXTRACK; i++)
					{
						if (i < MaxFlag && OrgFlagsUndo[i][0] != mi.tdata[i].wave_no) {
							OrgFlagsUndo[i][0] = mi.tdata[i].wave_no;
							if (i < MAXMELODY)
							{
								OrgFlagsUndo[i][1] = mi.tdata[i].pipi;
								OrgFlagsUndo[i][2] = mi.tdata[i].freq;
							}
						}
					}
					org_data.SetMusicInfo(&mi, SETFREQ | SETPIPI | SETWAVE);
					EndDialog(hdwnd, 0);
					EnableDialogWindow(TRUE);
					return 1;
				}
			case IDCANCEL: //Cancel Button
				Rxo_StopAllSoundNow();
				org_data.GetMusicInfo(&mi);
				for (j = 0; j < MAXMELODY; j++) {
					MakeOrganyaWave(j, mi.tdata[j].wave_no, mi.tdata[j].pipi);
				}
				for (j = MAXMELODY; j < MAXTRACK; j++) {
					InitDramObject(mi.tdata[j].wave_no, j - MAXMELODY);
				}
				EndDialog(hdwnd, 0);
				EnableDialogWindow(TRUE);
				return 1;
			}
		}
		else if ((HWND)lParam == GetDlgItem(hdwnd, IDC_EXTRAPIPI)) { //PiPi set button
			PiPiMode = (PiPiMode + 1) % 4;
			ChangePiPiActive(hdwnd, PiPiMode);
			return 1;
		}
		else if ((HWND)lParam == GetDlgItem(hdwnd, IDC_RANDOMFREQ)) //Sets random frequency
		{
			Rxo_StopAllSoundNow();
			for (i = 0; i < 8; i++)
			{
				n = rand() % (1900 - 100 + 1) + 100; //Wow, thanks stack overflow for telling me min->max randomization!
				SetText(hdwnd, IDD_SETFREQx0 + i, n);
				n = rand() % (1900 - 100 + 1) + 100;
				SetText(hdwnd, IDD_SETFREQx0 + i + 497, n);
			}
			return 1;
		}
		/*else if ((HWND)lParam == GetDlgItem(hdwnd, IDC_WAVEFORM))
		{
			DialogBox(hInst, "DLGWAVEFORM", hdwnd, DialogWaveform);
		}*/
		for (j = 0; j < MAXMELODY; j++) {
			if (freqbox[j] == LOWORD(wParam)) {
				if (HIWORD(wParam) == LBN_SELCHANGE) {
					SetDlgItemText(hdwnd, iLastLBox + IDC_LABEL_TRACK_0, strTrack[iLastLBox]);
					iLastLBox = j;
					SetDlgItemText(hdwnd, iLastLBox + IDC_LABEL_TRACK_0, strBracketTrack[iLastLBox]);
					Rxo_StopAllSoundNow();
					//インデックスを得る
					i = SendDlgItemMessage(hdwnd, freqbox[j], LB_GETCURSEL, 0, 0);
					mi.tdata[j].wave_no = i;
					if (j < 8) { GetDlgItemTextA(hdwnd, IDD_SETFREQx0 + j, str, MAXTRACK); i = atol(str); mi.tdata[j].freq = (i > 0x7530) ? 0x7530 : i; }
					else GetDlgItemTextA(hdwnd, IDD_SETFREQx0 + j + 489, str, MAXTRACK); i = atol(str); mi.tdata[j].freq = (i > 0x7530) ? 0x7530 : i;
					if (j < 8) mi.tdata[j].pipi = (IsDlgButtonChecked(hdwnd, IDC_CHECK_PIPIx0 + j)) ? 1 : 0;
					else mi.tdata[j].pipi = (IsDlgButtonChecked(hdwnd, IDC_CHECK_PIPIx0 + j + 533)) ? 1 : 0;
					MakeOrganyaWave(j, mi.tdata[j].wave_no, mi.tdata[j].pipi);
					PlayOrganKey(SamplePlayHeight, j, mi.tdata[j].freq, 240, mi.tdata[j].pipi);
				}
				else if (HIWORD(wParam) == LBN_SETFOCUS) {
					if (j != iLastLBox) {
						SetDlgItemText(hdwnd, iLastLBox + IDC_LABEL_TRACK_0, strTrack[iLastLBox]);
						iLastLBox = j;
						SetDlgItemText(hdwnd, iLastLBox + IDC_LABEL_TRACK_0, strBracketTrack[iLastLBox]);
					}
				}
			}
			else {
				SetDlgItemText(hdwnd, j + IDC_LABEL_TRACK_0, strTrack[j]);
			}
		}
		for (j = MAXMELODY; j < MAXTRACK; j++) {
			if (freqbox[j] == LOWORD(wParam)) {
				if (HIWORD(wParam) == LBN_SELCHANGE) {
					SetDlgItemText(hdwnd, iLastLBox + IDC_LABEL_TRACK_0, strTrack[iLastLBox]);
					iLastLBox = j;
					SetDlgItemText(hdwnd, iLastLBox + IDC_LABEL_TRACK_0, strBracketTrack[iLastLBox]);
					Rxo_StopAllSoundNow();
					i = SendDlgItemMessage(hdwnd, freqbox[j], LB_GETCURSEL, 0, 0);//インデックスを得る
					i = Wave_no_to_List_no[i]; //アルファベット順(List順)からWave順に変換する。これ重要。
					InitDramObject(i, j - MAXMELODY);
					PlayOrganKey(SamplePlayHeight, j, 1000, 240);
					mi.tdata[j].wave_no = i;
					iLastLBox = j;
				}
				else if (HIWORD(wParam) == LBN_SETFOCUS) {
					if (j != iLastLBox) {
						SetDlgItemText(hdwnd, iLastLBox + IDC_LABEL_TRACK_0, strTrack[iLastLBox]);
						iLastLBox = j;
						SetDlgItemText(hdwnd, iLastLBox + IDC_LABEL_TRACK_0, strBracketTrack[iLastLBox]);
					}
				}
			}
		}
		return TRUE;
	/*case WM_LBUTTONDOWN:
		mouse_x = LOWORD(lParam);  mouse_y = HIWORD(lParam);
		if (mouse_y > 73) {

			mx = (mouse_x - 646 + 4 - ((mouse_x > 246 + 36 * 5) ? 4 : 0)) / 36;
			my = (mouse_y - 89 + 16) / 48;
			n = (mx + my * 10) - 19;
			if (mx <= 29 && mx >= 19 && my >= 0 && my <= 10 && n >= 0 && n < 100 && iLastLBox >= 0 && iLastLBox < MAXMELODY) {
				SendDlgItemMessage(hdwnd, freqbox[iLastLBox], LB_SETTOPINDEX, maxx(0, n - 9), 0);
				SendDlgItemMessage(hdwnd, freqbox[iLastLBox], LB_SETCURSEL, n, 0);
				MakeOrganyaWave(iLastLBox, (unsigned char)n, mi.tdata[iLastLBox].pipi);
				Rxo_StopAllSoundNow();
				Rxo_PlayKey(SamplePlayHeight, iLastLBox, 1000, 1);
				mi.tdata[iLastLBox].wave_no = (unsigned char)n;

			}
		}
		return 1;*/
	case WM_LBUTTONUP:
		Rxo_StopAllSoundNow();
		return TRUE;
	}
	return FALSE;
}
/*
BOOL CALLBACK DialogWaveform(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static MUSICINFO mi;
	HBITMAP oldBmp = (HBITMAP)SendDlgItemMessage(hdwnd, IDC_WAVE100, STM_SETIMAGE, IMAGE_BITMAP, (long)waveBmp);
	
	long mouse_x, mouse_y, mx, my;
	unsigned char n;
	char* strTone[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
	char str[20];
	switch (message) {

	case WM_INITDIALOG:
		if (waveBmp != NULL) {
			if (oldBmp != NULL) {
				DeleteObject(oldBmp);
				GenerateWaveGraphic(wave_data);
				oldBmp = (HBITMAP)SendDlgItemMessage(hdwnd, IDC_WAVE100, STM_SETIMAGE, IMAGE_BITMAP, (long)waveBmp);
			}
		}
		Sl_Reset(hdwnd);
		return TRUE;

	case WM_HSCROLL:
		if ((HWND)lParam == GetDlgItem(hdwnd, IDC_SLIDER1)) {
			SamplePlayHeight = SendDlgItemMessage(hdwnd, IDC_SLIDER1, TBM_GETPOS, 0, 0);
			SetDlgItemText(hdwnd, IDC_NOTE, strTone[SamplePlayHeight % 12]);
		}
		return TRUE;
	case WM_LBUTTONDOWN:
		mouse_x = LOWORD(lParam);  mouse_y = HIWORD(lParam);
		
		if (mouse_y > 59) {

			mx = (mouse_x - 54 + 10 - ((mouse_x > 200 + 36 * 5) ? 4 : 0)) / 34;
			my = (mouse_y - 89 + 14) / 72;
			n = (mx + my * 10) + 6;
			if ((mx >= 0 && mx <= 10) && (my >= 0 && my <= 7) && (n < 100 && org_data.track < MAXMELODY)) {
				SendDlgItemMessage(hdwnd, freqbox[org_data.track], LB_SETTOPINDEX, maxx(0, n - 9), 0);
				SendDlgItemMessage(hdwnd, freqbox[org_data.track], LB_SETCURSEL, n, 0);
				MakeOrganyaWave(org_data.track, (unsigned char)n, mi.tdata[org_data.track].pipi);
				Rxo_StopAllSoundNow();
				PlayOrganKey(SamplePlayHeight, org_data.track,mi.tdata[org_data.track].freq,240,mi.tdata[org_data.track].pipi);
				mi.tdata[org_data.track].wave_no = (unsigned char)n;

			}
		}
		return TRUE;
	case IDCANCEL:
		EndDialog(hdwnd, 0);
		EnableDialogWindow(FALSE);
		return TRUE;
	}
	return FALSE;
}
*/
int usebox[MAXTRACK] = {
	IDE_USE0,
	IDE_USE1,
	IDE_USE2,
	IDE_USE3,
	IDE_USE4,
	IDE_USE5,
	IDE_USE6,
	IDE_USE7,
	IDE_USE8,
	IDE_USE9,
	IDE_USE10,
	IDE_USE11,
	IDE_USE12,
	IDE_USE13,
	IDE_USE14,
	IDE_USE15,

	IDE_USEDxQ,
	IDE_USEDxW,
	IDE_USEDxE,
	IDE_USEDxR,
	IDE_USEDxT,
	IDE_USEDxY,
	IDE_USEDxU,
	IDE_USEDxI,
	IDE_USEDxA,
	IDE_USEDxS,
	IDE_USEDxD,
	IDE_USEDxF,
	IDE_USEDxG,
	IDE_USEDxH,
	IDE_USEDxJ,
	IDE_USEDxK
}; //The amount of memory used in a TRACK
int leftbox[MAXTRACK] = {
	IDE_LEFT0,
	IDE_LEFT1,
	IDE_LEFT2,
	IDE_LEFT3,
	IDE_LEFT4,
	IDE_LEFT5,
	IDE_LEFT6,
	IDE_LEFT7,
	IDE_LEFT8,
	IDE_LEFT9,
	IDE_LEFT10,
	IDE_LEFT11,
	IDE_LEFT12,
	IDE_LEFT13,
	IDE_LEFT14,
	IDE_LEFT15,

	IDE_LEFTDxQ,
	IDE_LEFTDxW,
	IDE_LEFTDxE,
	IDE_LEFTDxR,
	IDE_LEFTDxT,
	IDE_LEFTDxY,
	IDE_LEFTDxU,
	IDE_LEFTDxI,
	IDE_LEFTDxA,
	IDE_LEFTDxS,
	IDE_LEFTDxD,
	IDE_LEFTDxF,
	IDE_LEFTDxG,
	IDE_LEFTDxH,
	IDE_LEFTDxJ,
	IDE_LEFTDxK
}; //The amount of memory left in a TRACK
BOOL CALLBACK DialogNoteUsed(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	char str[10] = {NULL};
	long u,l;
//	unsigned short a;
	switch(message){
	case WM_INITDIALOG://ダイアログが呼ばれた
		for(i = 0; i < MAXTRACK; i++){
			org_data.GetNoteUsed(&u,&l,i);
			itoa(u,str,10);
			SetDlgItemText(hdwnd,usebox[i],str);
			itoa(l,str,10);
			SetDlgItemText(hdwnd,leftbox[i],str);
		}
		org_data.GetFlagUsed(&u,&l);
		itoa(u, str, 10);
		SetDlgItemText(hdwnd, IDE_USEDFLAG, str);
		itoa(l, str, 10);
		SetDlgItemText(hdwnd, IDE_LEFTFLAG, str);
		EnableDialogWindow(FALSE);
		return 1;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDOK:
			EnableDialogWindow(TRUE);
			EndDialog(hdwnd,0);
			return 1;
		}
	}
	return 0;
}

BOOL CALLBACK DialogTheme(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int i, j;
	switch (message) {
	case WM_INITDIALOG://ダイアログが呼ばれた
	{
		SendDlgItemMessage(hdwnd, IDD_THEMES, LB_ADDSTRING, 0, (LPARAM)"OrgMaker 16 (default)");

		WIN32_FIND_DATA fdFile;
		HANDLE hFind = NULL;
		
		char sDir[MAX_PATH];
		GetApplicationPath(sDir);
		strcat(sDir, "themes");

		char sPath[MAX_PATH];
		sprintf(sPath, "%s\\*.*", sDir);

		if ((hFind = FindFirstFile(sPath, &fdFile)) != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (strcmp(fdFile.cFileName, ".") != 0
					&& strcmp(fdFile.cFileName, "..") != 0)
				{
					sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);

					if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // It should be a theme folder
					{
						SendDlgItemMessage(hdwnd, IDD_THEMES, LB_ADDSTRING, 0, (LPARAM)fdFile.cFileName);
					}
				}
			} while (FindNextFile(hFind, &fdFile));

			FindClose(hFind);
		}

		EnableDialogWindow(FALSE);
		return 1;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDOK:
			gSelectedTheme[0] = 0;

			char sDir[MAX_PATH];
			GetApplicationPath(sDir);

			i = SendDlgItemMessage(hdwnd, IDD_THEMES, LB_GETCURSEL, 0, 0);
			if (i != 0) {
				j = SendDlgItemMessage(hdwnd, IDD_THEMES, LB_GETTEXTLEN, i, 0);
				if (j != LB_ERR) {
					char* nam = (char*)malloc(j + 1);
					if (nam != NULL) {
						memset(nam, '\0', sizeof(nam));
						SendDlgItemMessage(hdwnd, IDD_THEMES, LB_GETTEXT, i, (LPARAM)nam);
						strcat(sDir, "themes\\");
						strcat(sDir, nam);
						free(nam);
					}
				}

				DWORD dwAttrib = GetFileAttributes(sDir);
				if (dwAttrib != INVALID_FILE_ATTRIBUTES && dwAttrib & FILE_ATTRIBUTE_DIRECTORY)
					strcpy(gSelectedTheme, sDir);
			}

			ReloadBitmaps();

			EnableDialogWindow(TRUE);
			EndDialog(hdwnd, 0);
			return 1;
		case IDCANCEL:
			EnableDialogWindow(TRUE);
			EndDialog(hdwnd, 0);
			return 1;
		case IDC_OPNTHMFLD:
			char themePath[MAX_PATH];
			GetApplicationPath(themePath);
			strcat(themePath, "themes");
			ShellExecute(NULL, "open", themePath, NULL, NULL, SW_SHOWDEFAULT);
			break;
		}
		return 1;
	}
	}
	return 0;
}

BOOL CALLBACK DialogWaveDB(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	MUSICINFO mi;
	int i, j;
	switch (message) {
	case WM_INITDIALOG://ダイアログが呼ばれた
	{
		SendDlgItemMessage(hdwnd, IDD_WAVEDBS, LB_ADDSTRING, 0, (LPARAM)"Organya (default)");

		WIN32_FIND_DATA fdFile;
		HANDLE hFind = NULL;

		char sDir[MAX_PATH];
		GetApplicationPath(sDir);
		strcat(sDir, "soundbanks");

		char sPath[MAX_PATH];
		sprintf(sPath, "%s\\*.wdb", sDir);

		if ((hFind = FindFirstFile(sPath, &fdFile)) != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (strcmp(fdFile.cFileName, ".") != 0
					&& strcmp(fdFile.cFileName, "..") != 0)
				{
					sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);

					if (!(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) // It should be a .sb(SoundBank) file
					{
						SendDlgItemMessage(hdwnd, IDD_WAVEDBS, LB_ADDSTRING, 0, (LPARAM)fdFile.cFileName);
					}
				}
			} while (FindNextFile(hFind, &fdFile));

			FindClose(hFind);
		}

		EnableDialogWindow(FALSE);
		return 1;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDOK:
			gSelectedWave[0] = 0;

			char sDir[MAX_PATH];
			GetApplicationPath(sDir);

			i = SendDlgItemMessage(hdwnd, IDD_WAVEDBS, LB_GETCURSEL, 0, 0);
			if (i != 0) {
				j = SendDlgItemMessage(hdwnd, IDD_WAVEDBS, LB_GETTEXTLEN, i, 0);
				if (j != LB_ERR) {
					char* nam = (char*)malloc(j + 1);
					if (nam != NULL) {
						memset(nam, '\0', sizeof(nam));
						SendDlgItemMessage(hdwnd, IDD_WAVEDBS, LB_GETTEXT, i, (LPARAM)nam);
						strcat(sDir, "soundbanks\\");
						strcat(sDir, nam);
						free(nam);
					}
				}

				DWORD dwAttrib = GetFileAttributes(sDir);
				if (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
					strcpy(gSelectedWave, sDir);
			}

			LoadWaveData100(gSelectedWave);

			org_data.GetMusicInfo(&mi);

			for (j = 0; j < MAXMELODY; j++)
				MakeOrganyaWave(j, mi.tdata[j].wave_no, mi.tdata[j].pipi);
			for (j = MAXMELODY; j < MAXTRACK; j++) {
				i = mi.tdata[j].wave_no;
				InitDramObject(i, j - MAXMELODY);
			}

			EnableDialogWindow(TRUE);
			EndDialog(hdwnd, 0);
			return 1;
		case IDCANCEL:
			EnableDialogWindow(TRUE);
			EndDialog(hdwnd, 0);
			return 1;
		case IDC_OPNTHMFLD:
			char wavePath[MAX_PATH];
			GetApplicationPath(wavePath);
			strcat(wavePath, "soundbanks");
			ShellExecute(NULL, "open", wavePath, NULL, NULL, SW_SHOWDEFAULT);
			break;
		}
		return 1;
	}
	}
	return 0;
}

BOOL CALLBACK DialogMemo(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	char str[10] = {NULL};
	switch(message){
	case WM_INITDIALOG://ダイアログが呼ばれた

		EnableDialogWindow(FALSE);
		return 1;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDOK:
			EnableDialogWindow(TRUE);
			EndDialog(hdwnd,0);
			return 1;
		case ID_ICON_ORG:
			PlaySound( "CAT" , GetModuleHandle(NULL),SND_RESOURCE | SND_ASYNC); 
			return 1;
		}


		return 1;
	
	}
	return 0;
}


static unsigned long sample_rate = 0;
static unsigned long loop_count = 0;
static unsigned long fade_mseconds = 0;

#define WRITE_16_LE(p, v) \
    do { \
        *p++ = ((v) & 0xFF); \
        *p++ = ((v) >> 8 & 0xFF); \
    } while(0)

#define WRITE_32_LE(p, v) \
    do { \
        *p++ = ((v) & 0xFF); \
        *p++ = ((v) >> 8 & 0xFF); \
        *p++ = ((v) >> 16 & 0xFF); \
        *p++ = ((v) >> 24 & 0xFF); \
    } while(0)

bool ExportWave(unsigned int streamsize, unsigned int samples, const char *strPath) {
	char* stream = (char*)malloc(44 + streamsize);
	if (stream != NULL) {
		char* p = stream;

		WRITE_32_LE(p, 0x46464952); /* "RIFF" */
		WRITE_32_LE(p, 36 + streamsize); /* "RIFF" length */
		WRITE_32_LE(p, 0x45564157); /* "WAVE" */
		WRITE_32_LE(p, 0x20746D66); /* "fmt " */
		WRITE_32_LE(p, 0x10); /* "fmt " length */
		WRITE_16_LE(p, 3); /* floating point PCM format */
		WRITE_16_LE(p, 2); /* 2 channels for stereo */
		WRITE_32_LE(p, sample_rate); /* Samples per second */
		WRITE_32_LE(p, sample_rate * sizeof(float) * 2); /* Bytes per second */
		WRITE_16_LE(p, 8); /* Bytes per sample */
		WRITE_16_LE(p, 32); /* Bits per sample */
		WRITE_32_LE(p, 0x61746164); /* "data" */
		WRITE_32_LE(p, streamsize); /* "data" length */

		memset(p, 0, streamsize);
		ExportOrganyaBuffer(sample_rate, (float*)p, samples, (fade_mseconds * sample_rate / 1000));

		FILE* fl = fopen(strPath, "wb");
		if (fl != NULL) {
			fwrite(stream, 1, 44 + streamsize, fl);
			fclose(fl);
		}
		else {
			free(stream);
			return false;
		}
		free(stream);
	}
	else {
		return false;
	}
	return true;
}

BOOL CALLBACK DialogWavExport(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MUSICINFO mi;
	char res;
	char strPath[MAX_PATH] = {NULL};
	switch (message) {
	case WM_INITDIALOG: {
		char str[10] = { NULL };
		itoa(sample_rate, str, 10);
		SetDlgItemText(hdwnd, IDE_SAMPLE_RATE, str);
		itoa(loop_count, str, 10);
		SetDlgItemText(hdwnd, IDE_LOOP_COUNT, str);
		itoa(fade_mseconds, str, 10);
		SetDlgItemText(hdwnd, IDE_FADE_MSECONDS, str);
		CheckDlgButton(hdwnd, IDC_EXPORTSEPARATE, 0);
		EnableDialogWindow(FALSE);
		return 1;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK: {
			bool separateChannels = IsDlgButtonChecked(hdwnd, IDC_EXPORTSEPARATE);
			char str[10] = { NULL };
			GetDlgItemText(hdwnd, IDE_SAMPLE_RATE, str, 7);
			sample_rate = atol(str);
			GetDlgItemText(hdwnd, IDE_LOOP_COUNT, str, 7);
			loop_count = atol(str);
			GetDlgItemText(hdwnd, IDE_FADE_MSECONDS, str, 7);
			fade_mseconds = atol(str);

			if (sample_rate < 1000 || sample_rate > 192000) {
				msgbox(hdwnd, IDS_STRING122, IDS_ERROR, MB_OK); // Not in range
				return 1;
			}
			if (loop_count < 0) {
				msgbox(hdwnd, IDS_STRING121, IDS_ERROR, MB_OK);
				return 1;
			}
			if (fade_mseconds < 0) fade_mseconds = 0;

			res = GetFileNameExportWav(hdwnd, MessageString[IDS_STRING124], strPath); // "WAV Export"
			if (res == MSGCANCEL) return 1;
			if (res == MSGEXISFILE) {
				if (msgbox(hdwnd, IDS_NOTIFY_OVERWRITE, IDS_INFO_SAME_FILE, MB_YESNO | MB_ICONEXCLAMATION) == IDNO) return 1;
			}

			SetDlgItemText(hdwnd, IDC_EXPORTTEXT, "Exporting...");

			org_data.GetMusicInfo(&mi, 1);
			unsigned int samples = ((mi.wait * sample_rate) / 1000) * (mi.end_x + ((mi.end_x - mi.repeat_x) * loop_count)) + (fade_mseconds * sample_rate / 1000);
			unsigned int streamsize = samples * sizeof(float) * 2;
			
			if (separateChannels) {
				for (int i = 0; i < MAXTRACK; ++i) {
					if (mi.tdata[i].note_list == NULL) {
						continue;
					}

					char* pC;
					char tstr[15] = { NULL };
					char chPath[MAX_PATH] = { NULL };

					snprintf(chPath, MAX_PATH, "%s", strPath);
					snprintf(tstr, 15, " - Track %c.wav", TrackN[i]);

					pC = strrchr(chPath, '.');
					if (pC != NULL) pC[0] = '\0';

					strncat(chPath, tstr, MAX_PATH - 1);

					SetExportChannel(i);
					if (!ExportWave(streamsize, samples, chPath)) {
						msgbox(hdwnd, IDS_STRING121, IDS_STRING120, MB_OK);
						break;
					}
				}

				SetExportChannel(-1);
			}
			
			if (!ExportWave(streamsize, samples, strPath)) {
				msgbox(hdwnd, IDS_STRING121, IDS_STRING120, MB_OK);
			}

			EnableDialogWindow(TRUE);
			EndDialog(hdwnd, 0);
			return 1;
		}
		case IDCANCEL:
			EnableDialogWindow(TRUE);
			EndDialog(hdwnd, 0);
			return 1;
		}
		return 1;
	}
	return 0;
}

BOOL CALLBACK DialogDecayLength(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char strPath[MAX_PATH] = { NULL };
	switch (message) {
	case WM_INITDIALOG: {
		char str[10] = { NULL };
		itoa(volChangeLength, str, 10);
		SetDlgItemText(hdwnd, IDE_VOL_LENGTH, str);
		CheckDlgButton(hdwnd, IDC_VOL_USENOTELEN, volChangeUseNoteLength ? 1 : 0);
		CheckDlgButton(hdwnd, IDC_VOL_SETNOTELEN, volChangeSetNoteLength ? 1 : 0);
		EnableDialogWindow(FALSE);
		return 1;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK: {
			char str[10] = { NULL };

			GetDlgItemText(hdwnd, IDE_VOL_LENGTH, str, 7);
			volChangeLength = atol(str);

			volChangeUseNoteLength = IsDlgButtonChecked(hdwnd, IDC_VOL_USENOTELEN);
			volChangeSetNoteLength = IsDlgButtonChecked(hdwnd, IDC_VOL_SETNOTELEN);

			EnableDialogWindow(TRUE);
			EndDialog(hdwnd, 0);
			return 1;
		}
		case IDCANCEL:
			EnableDialogWindow(TRUE);
			EndDialog(hdwnd, 0);
			return 1;
		}
		return 1;
	}
	return 0;
}

bool CheckDefPanVol(HWND hdwnd, int WithSet)
{
	int t,a,b;
	for(t=0;t<MAXTRACK;t++){
		a = GetText(hdwnd , txt_Pan[t]);
		if(a > 12 || a < 0){
			MessageBox(hdwnd,"Default Pan must be between 0 and 12","Could not set Default Pan",MB_OK | MB_ICONERROR);	// 2014.10.19 D
			return false;
		}
		b = GetText(hdwnd , txt_Vol[t]);
		if(b > 254 || b < 0){
			MessageBox(hdwnd,"Default Volume must be between 0 and 254.","Could not set Default Volume",MB_OK | MB_ICONERROR);	// 2014.10.19 D
			return false;
		}
	}
	if(WithSet>0){
		for(t=0;t<MAXTRACK;t++){
			a = GetText(hdwnd , txt_Pan[t]);
			b = GetText(hdwnd , txt_Vol[t]);
			org_data.def_pan[t] = (unsigned char)a;
			org_data.def_volume[t] = (unsigned char)b;
		}
	}
	return true;
}

bool LoadPVIFile(HWND hdwnd, char *FileName)
{
	FILE *fp;
	fp = fopen(FileName, "rt");
	if(fp==NULL) return false;
	int t,r;
	for(t=0;t<MAXTRACK;t++){
		fscanf(fp,"%d",&r);
		SetText(hdwnd,txt_Pan[t],r);
		fscanf(fp,"%d",&r);
		SetText(hdwnd,txt_Vol[t],r);
	}
	fclose(fp);
	return true;
}


bool SavePVIFile(HWND hdwnd, char *FileName)
{
	FILE *fp;
	fp = fopen(FileName, "wt");
	if(fp==NULL) return false;
	int t,r;
	for(t=0;t<MAXTRACK;t++){
		r = GetText(hdwnd , txt_Pan[t]);
		fprintf(fp,"%d\n",r);
		r = GetText(hdwnd , txt_Vol[t]);
		fprintf(fp,"%d\n",r);
	}
	fclose(fp);
	return true;
}

bool GetSetPVInitFile(HWND hdwnd, int IsSave, int iQuiet)
{
	OPENFILENAME ofn;
	FILE *fp;
	char init_file[512];
	int ml;
	strcpy(init_file, "Default");
	ml = strlen(init_file);
	if(ml>4){
		if(init_file[ml-4]=='.'){ //pvi付加
			//init_file[ml-4]='.';
			init_file[ml-3]='p';
			init_file[ml-2]='v';
			init_file[ml-1]='i';
		}
	}

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner   = hdwnd;
	ofn.hInstance   = hInst;
	//ofn.lpstrFilter = "パン／ボリューム初期値データ[*.pvi]\0*.pvi\0全ての形式 [*.*]\0*.*\0\0"; //パン／ボリューム初期値データ[*.pvi]\0*.pvi\0全ての形式 [*.*]\0*.*\0\0	// 2014.10.19 D
	ofn.lpstrFilter = "Default Pan/Volume Data (*.pvi)\0*.pvi\0All Files (*.*)\0*.*\0\0";
	ofn.lpstrFile   = init_file;
	ofn.nMaxFile    = MAX_PATH;
	ofn.lpstrDefExt = "pvi";
	
	if(IsSave>0){ //セーブ
		ofn.Flags       = OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
		ofn.lpstrTitle  = MessageString[IDS_STRING65]; //パン／ボリューム初期値データの保存
		if(GetSaveFileName(&ofn));	//InvalidateRect(hwnd,NULL,TRUE);
		else return false;			//キャンセルで0が返る
		
	}else{			//ロード
		ofn.Flags       = OFN_HIDEREADONLY;
		ofn.lpstrTitle  = MessageString[IDS_STRING66]; //パン／ボリューム初期値データの読み込み
		if(GetOpenFileName(&ofn));	//InvalidateRect(hwnd,NULL,TRUE);
		else return false;			//キャンセルで0が返る
	}
	fp = fopen(init_file,"rb");		//検査
	if(fp == NULL){
		if(IsSave==0){
			if(iQuiet == 0) MessageBox(hdwnd,"PVI file does not exist.","Could not read inital PVI file",MB_OK | MB_ICONERROR);	// 2014.10.19 D
			return false;			//指定ファイルが存在しない
		}
	}else{
		fclose(fp);
	}
	bool b;
	if(IsSave==0){
		b = LoadPVIFile(hdwnd, init_file);
	}else{
		b = SavePVIFile(hdwnd, init_file);
	}
	return b;
	
}


//PVI Default thing
BOOL CALLBACK DialogDefault(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char str[32] = {NULL};
	int i,a,b;
	switch(message){
	case WM_INITDIALOG://ダイアログが呼ばれた
		//リピート範囲の初期化//////////////////
		/*
		itoa(org_data.def_pan,str,10);
		SetDlgItemText(hdwnd,IDD_DEFPAN,str);
		itoa(org_data.def_volume,str,10);
		SetDlgItemText(hdwnd,IDD_DEFVOLUME,str);
		*/
		for(i=0;i<MAXTRACK;i++){
			SetText(hdwnd, txt_Pan[i] , (int) org_data.def_pan[i] );
			SetText(hdwnd, txt_Vol[i] , (int) org_data.def_volume[i] );
			
		}
		EnableDialogWindow(FALSE);
		return 1;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDC_PANPOT1: case IDC_PANPOT2: case IDC_PANPOT3: case IDC_PANPOT4: case IDC_PANPOT5: case IDC_PANPOT6: case IDC_PANPOT7: case IDC_PANPOT8: case IDC_PANPOT9: case IDC_PANPOT10: case IDC_PANPOT11: case IDC_PANPOT12: case IDC_PANPOT13: case IDC_PANPOT14: case IDC_PANPOT15: case IDC_PANPOT16:
		case IDC_PANPOTDxQ: case IDC_PANPOTDxW: case IDC_PANPOTDxE: case IDC_PANPOTDxR: case IDC_PANPOTDxT: case IDC_PANPOTDxY: case IDC_PANPOTDxU: case IDC_PANPOTDxI: case IDC_PANPOTDxA: case IDC_PANPOTDxS: case IDC_PANPOTDxD: case IDC_PANPOTDxF: case IDC_PANPOTDxG: case IDC_PANPOTDxH: case IDC_PANPOTDxJ: case IDC_PANPOTDxK:
		case IDC_VOLUME1: case IDC_VOLUME2: case IDC_VOLUME3: case IDC_VOLUME4: case IDC_VOLUME5: case IDC_VOLUME6: case IDC_VOLUME7: case IDC_VOLUME8: case IDC_VOLUME9: case IDC_VOLUME10: case IDC_VOLUME11: case IDC_VOLUME12: case IDC_VOLUME13: case IDC_VOLUME14: case IDC_VOLUME15: case IDC_VOLUME16:
		case IDC_VOLUMEDxQ: case IDC_VOLUMEDxW: case IDC_VOLUMEDxE: case IDC_VOLUMEDxR: case IDC_VOLUMEDxT: case IDC_VOLUMEDxY: case IDC_VOLUMEDxU: case IDC_VOLUMEDxI: case IDC_VOLUMEDxA: case IDC_VOLUMEDxS: case IDC_VOLUMEDxD: case IDC_VOLUMEDxF: case IDC_VOLUMEDxG: case IDC_VOLUMEDxH: case IDC_VOLUMEDxJ: case IDC_VOLUMEDxK:
			if(HIWORD(wParam) == EN_SETFOCUS)PostMessage(GetDlgItem(hdwnd, LOWORD(wParam)), EM_SETSEL, 0, -1); //フォーカス時にテキストを全選択する	// 2014.10.19 
			return 1;
		case IDC_DEF_NOWLOAD:
			GetSetPVInitFile(hdwnd, 0, 0);
			break;
		case IDC_DEF_NOWSAVE:
			GetSetPVInitFile(hdwnd, 1, 0);
			break;
		case IDCANCEL:
			EndDialog(hdwnd,0);
			EnableDialogWindow(TRUE);
			return 1;
		case IDC_BTN_SORT:
			a = GetText(hdwnd,txt_Pan[0]);
			b = GetText(hdwnd,txt_Vol[0]);
			for(i=0;i<MAXTRACK;i++){
				SetText(hdwnd, txt_Pan[i] , a );
				SetText(hdwnd, txt_Vol[i] , b );
			}			
			break;
		case IDC_PRESET1: //Preset 1
			for(i=0;i<MAXTRACK;i++){
				SetText(hdwnd, txt_Pan[i] , Preset1_Pan[i] );
				SetText(hdwnd, txt_Vol[i] , Preset1_Vol[i] );
			}
			break;
		case IDC_PRESET2: //Vol 95%
			for(i=0;i<MAXTRACK;i++){
				b = GetText(hdwnd,txt_Vol[i]);
				SetText(hdwnd, txt_Vol[i] , (b*95/100) );
			}
			break;
		case IDC_PRESET3: //Random Pan
			for(i=0;i<MAXTRACK;i++){
				a = (int)(GetNormDistRand() * 2.0 + 6.0); a = (a < 0) ? 0 : ((a > 12) ? 12 : a);
				SetText(hdwnd, txt_Pan[i] , a );
			}
			break;
		case IDC_PRESET4: //Random Volume
			for(i=0;i<MAXTRACK;i++){
				b = (int)(GetNormDistRand() * 18.0 + 185.0); b = (b < 0) ? 0 : ((b > 253) ? 253 : b);;
				SetText(hdwnd, txt_Vol[i] , b );
			}
			break;
		case IDC_PRESET5: //Reset
			for(i=0;i<MAXTRACK;i++){
				SetText(hdwnd, txt_Pan[i] , 6 );
				SetText(hdwnd, txt_Vol[i] , 200 );
			}
			break;
		case IDOK:
			if( !CheckDefPanVol(hdwnd, 1) )return FALSE; //検査とセッティング
			EndDialog(hdwnd,0);
			EnableDialogWindow(TRUE);
			return 1;
		}
	}
	return 0;
} //PVI  //
//Help item
BOOL CALLBACK DialogHelp(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	char str[10] = {NULL};
//	char HelpString[10000]; //とりあえず10kB
	char *HelpString; //A 2010.9.22 ヘルプファイルの容量拡大に伴い、動的確保に変更。
	HRSRC hrscr;
	HWND haDlg;
	MINMAXINFO *pmmi;
	//DWORD *lpdword;//リソースのアドレス
	char *cc,*ce,*cf;
	//int i;
	int DlgWidth, DlgHeight;
	static int t=0;
	if(GetDlgItem(hdwnd, IDC_HELPTEXT)!=NULL){
		t++;
		if(t<5)SendMessage(GetDlgItem(hdwnd, IDC_HELPTEXT), EM_SETSEL, 0, 0);
		//SendMessage(GetDlgItem(hdwnd, IDC_HELPTEXT), EM_SCROLLCARET, 0, 0);
	}
	switch(message){
	case WM_INITDIALOG://ダイアログが呼ばれた
		t = 0;
		HelpString = (char *)malloc(1048576 * sizeof(char));//動的容量確保(とりあえず1MB) A 2010.09.22
		//EnableDialogWindow(FALSE); //D 2014.05.25
		hrscr = FindResource(NULL, "TEXTHELP" , "TEXT");// リソースの検索
		cc = (char*)LockResource(LoadResource(NULL, hrscr));// リソースのアドレスを取得
		cf = HelpString; ce = cc;
		do{
			if(*ce=='\n'){ //改行コード変換（ラインフィードを加えないといけないらしい）
				*cf='\r';
				cf++;
			}
			*cf = *ce;
			cf++;
			ce++;
		}while(*ce !=0);
		*cf = *ce;
		SetDlgItemText(hdwnd,IDC_HELPTEXT,HelpString);
		free(HelpString); //開放 A 2010.09.22

		if(GetDlgItem(hdwnd, IDC_HELPTEXT)!=NULL)
			SendMessage(GetDlgItem(hdwnd, IDC_HELPTEXT), EM_SETSEL, 1, 2);
			SendMessage(GetDlgItem(hdwnd, IDC_HELPTEXT), EM_SCROLLCARET, 0, 0);

		return 1;
	case WM_SIZE:
		DlgWidth  = LOWORD(lParam);	//クライアント領域のサイズ
		DlgHeight = HIWORD(lParam);
		haDlg = GetDlgItem(hdwnd, IDC_HELPTEXT);
		SetWindowPos(haDlg, HWND_TOP, 1, 1, DlgWidth-4, DlgHeight-36, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
		
		return 1;
	case WM_GETMINMAXINFO:
		pmmi = (MINMAXINFO *)lParam;
		if ( pmmi )
		{
			pmmi->ptMinTrackSize.x = 648;  // 最小幅
			pmmi->ptMinTrackSize.y = 329;  // 最小高
		}
		return 1;
	case WM_MOUSEMOVE:
		SendMessage(GetDlgItem(hdwnd, IDC_HELPTEXT), EM_SETSEL, 0,0);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDCANCEL:
			//EnableDialogWindow(TRUE); //D 2014.05.25
			//EndDialog(hdwnd,0);
			ShowWindow(hdwnd, SW_HIDE);
			return 1;
		}
		return 1;
	
	}
	return 0;
} //Help Item

BOOL CALLBACK DialogFlags(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char str[24] = { NULL };
	char i;
	MUSICINFO mi;
	
	switch (message) {
	case WM_INITDIALOG: {
		org_data.GetMusicInfo(&mi);
		if (OrgFlagsDlg == -1)
		{
			itoa(mi.tdata[org_data.track].freq, str, 10);
			SetDlgItemText(hdwnd, IDC_FFREQ, str);
			SetDlgItemText(hdwnd, IDC_FTRACK, TrackCode[org_data.track]);
			CheckDlgButton(hdwnd, IDC_TRACKCHANGE, BST_CHECKED);
			SendDlgItemMessage(hdwnd, IDC_FWAVE, LB_RESETCONTENT, 0, 0);//初期化
		}
		else {
			if (OrgFlags[OrgFlagsDlg][4] == NULL) itoa(mi.tdata[org_data.track].freq, str, 10);
			else itoa(OrgFlags[OrgFlagsDlg][4], str, 10);
			SetDlgItemText(hdwnd, IDC_FFREQ, str);
			SetDlgItemText(hdwnd, IDC_FTRACK, TrackCode[org_data.track]);
			if (OrgFlags[OrgFlagsDlg][0] == 1)CheckDlgButton(hdwnd, IDC_TRACKCHANGE, BST_CHECKED);
			else CheckDlgButton(hdwnd, IDC_WAITCHANGE, BST_CHECKED);
			if (OrgFlags[OrgFlagsDlg][3] == 1) CheckDlgButton(hdwnd, IDC_FPIPI, BST_CHECKED);
			SendDlgItemMessage(hdwnd, IDC_FWAVE, LB_RESETCONTENT, 0, 0);
		}
		if (org_data.track < MAXMELODY)
		{
			for (i = 0; i < MAXWAVE; i++) {
				if (OrgFlagsDlg == -1) {
					if (i == mi.tdata[org_data.track].wave_no) {
						sprintf(str, MessageString[IDS_STRING117], i);	// 2010.09.30 D "Wave-%02d*"
					}
					else {
						sprintf(str, MessageString[IDS_STRING118], i);	// 2010.09.30 D "Wave-%02d"
					}
					SendDlgItemMessage(hdwnd, IDC_FWAVE, LB_ADDSTRING, 0, (LPARAM)str);
					SendDlgItemMessage(hdwnd, IDC_FWAVE, LB_SETCURSEL, mi.tdata[org_data.track].wave_no, 0);
				}
				else {
					if (i == OrgFlags[OrgFlagsDlg][2]) {
						sprintf(str, MessageString[IDS_STRING117], i);	// 2010.09.30 D "Wave-%02d*"
					}
					else {
						sprintf(str, MessageString[IDS_STRING118], i);	// 2010.09.30 D "Wave-%02d"
					}
					SendDlgItemMessage(hdwnd, IDC_FWAVE, LB_ADDSTRING, 0, (LPARAM)str);
					SendDlgItemMessage(hdwnd, IDC_FWAVE, LB_SETCURSEL, OrgFlags[OrgFlagsDlg][2], 0);
				}
			}
			
		}
		else
		{
			for (i = 0; i < NUMDRAMITEM; i++) {
				SendDlgItemMessage(hdwnd, IDC_FWAVE, LB_ADDSTRING, 0, (LPARAM)dram_name[Wave_no_to_List_no[i]]);
			}
			if (OrgFlagsDlg == -1) SendDlgItemMessage(hdwnd, IDC_FWAVE, LB_SETCURSEL, List_no_to_Wave_no[mi.tdata[org_data.track].wave_no], 0);
			else SendDlgItemMessage(hdwnd, IDC_FWAVE, LB_SETCURSEL, List_no_to_Wave_no[OrgFlags[OrgFlagsDlg][2]], 0);
		}
		EnableDialogWindow(FALSE);
		return 1;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_TRACKCHANGE:
		{
			CheckDlgButton(hdwnd, IDC_WAITCHANGE, BST_UNCHECKED);
			CheckDlgButton(hdwnd, IDC_TRACKCHANGE, BST_CHECKED);
			return TRUE;
		}
		case IDC_WAITCHANGE:
		{
			if (org_data.track < MAXMELODY) {
				CheckDlgButton(hdwnd, IDC_WAITCHANGE, BST_CHECKED);
				CheckDlgButton(hdwnd, IDC_TRACKCHANGE, BST_UNCHECKED);
			}
			else { MessageBox(hdwnd, "Frequency does not apply to drams.", "Flag could not set Frequency", MB_OK | MB_ICONERROR); CheckDlgButton(hdwnd, IDC_WAITCHANGE, BST_UNCHECKED); }
			return TRUE;
		}
		case IDC_FPIPI:
			if (org_data.track > MAXMELODY) { 
				MessageBox(hdwnd, "Pipi does not apply to drams.", "Flag could not set Pipi", MB_OK | MB_ICONERROR); 
				CheckDlgButton(hdwnd, IDC_FPIPI, BST_UNCHECKED);
			}
			return TRUE;
		case IDOK: {
			if (OrgFlagsDlg == -1) {
				for (i = 0; i < MaxFlag; i++)
				{
					if (OrgFlags[i][0] == NULL) //First Free Flag
					{
						if (!IsDlgButtonChecked(hdwnd, IDC_WAITCHANGE)) //Track Change
						{
							OrgFlags[i][0] = 1;
							OrgFlags[i][1] = org_data.track;
							if (org_data.track < MAXMELODY) OrgFlags[i][2] = SendDlgItemMessage(hdwnd, IDC_FWAVE, LB_GETCURSEL, 0, 0);
							else { OrgFlags[i][2] = Wave_no_to_List_no[SendDlgItemMessage(hdwnd, IDC_FWAVE, LB_GETCURSEL, 0, 0)]; }
							if (IsDlgButtonChecked(hdwnd, IDC_FPIPI)) OrgFlags[i][3] = 1;
						}
						else
						{
							OrgFlags[i][0] = 2;
							OrgFlags[i][1] = org_data.track;
							GetDlgItemText(hdwnd, IDC_FFREQ, str, 6);
							OrgFlags[i][4] = atol(str);
							if (OrgFlags[i][4] > 0x7530)
							{
								MessageBox(hWnd, "Frequency cannot be above 30,000.", "Could not set Frequency", MB_OK | MB_ICONWARNING);
								return TRUE;
							}
						}
						break;
					}
				}
			}
			else {
				if (!IsDlgButtonChecked(hdwnd, IDC_WAITCHANGE)) //Track Change
				{
					OrgFlags[OrgFlagsDlg][0] = 1;
					OrgFlags[OrgFlagsDlg][1] = org_data.track;
					if (org_data.track < MAXMELODY) OrgFlags[OrgFlagsDlg][2] = SendDlgItemMessage(hdwnd, IDC_FWAVE, LB_GETCURSEL, 0, 0);
					else { OrgFlags[OrgFlagsDlg][2] = Wave_no_to_List_no[SendDlgItemMessage(hdwnd, IDC_FWAVE, LB_GETCURSEL, 0, 0)]; }
					if (IsDlgButtonChecked(hdwnd, IDC_FPIPI) && org_data.track < MAXMELODY)
					{
						OrgFlags[OrgFlagsDlg][3] = 1;
					}
					else if (org_data.track >= MAXMELODY && IsDlgButtonChecked(hdwnd, IDC_FPIPI) == TRUE) {
						MessageBox(hWnd, "Pipi does not apply to Drams.", "Flag could not set Pipi", MB_OK | MB_ICONWARNING); return TRUE;
					}
				}
				else
				{
					OrgFlags[OrgFlagsDlg][0] = 2;
					OrgFlags[OrgFlagsDlg][1] = org_data.track;
					OrgFlags[OrgFlagsDlg][2] = 0;
					OrgFlags[OrgFlagsDlg][3] = 0;
					GetDlgItemText(hdwnd, IDC_FFREQ, str, 6);
					OrgFlags[OrgFlagsDlg][4] = atol(str);
					if (OrgFlags[OrgFlagsDlg][4] > 0x7530)
					{
						MessageBox(hWnd, "Frequency cannot be above 30,000.", "Flag could not set Frequency", MB_OK | MB_ICONWARNING);
						return TRUE;
					}
				}
			}
			org_data.GetMusicInfo(&mi);
			for (i = 0; i < MAXMELODY; i++) MakeOrganyaWave(i, mi.tdata[i].wave_no, mi.tdata[i].pipi);
			for (i = 16; i < MAXTRACK; i++) InitDramObject(mi.tdata[i].wave_no, i - MAXMELODY);
			EnableDialogWindow(TRUE);
			EndDialog(hdwnd, 0);
			FlagR = true;
			return TRUE;
		}
		case IDCANCEL:
			org_data.GetMusicInfo(&mi);
			for (i = 0; i < MAXMELODY; i++) MakeOrganyaWave(i, mi.tdata[i].wave_no, mi.tdata[i].pipi);
			for (i = 16; i < MAXTRACK; i++) InitDramObject(mi.tdata[i].wave_no, i - MAXMELODY);
			EnableDialogWindow(TRUE);
			EndDialog(hdwnd, 0);
			FlagR = false;
			return FALSE;
		}
		if (HIWORD(wParam) == LBN_SELCHANGE) {
			Rxo_StopAllSoundNow();
			i = SendDlgItemMessage(hdwnd, IDC_FWAVE, LB_GETCURSEL, 0, 0);
			if (org_data.track < MAXMELODY)
			{
				mi.tdata[org_data.track].wave_no = i;
				mi.tdata[org_data.track].pipi = (IsDlgButtonChecked(hdwnd, IDC_FPIPI)) ? 1 : 0;
				MakeOrganyaWave(org_data.track, mi.tdata[org_data.track].wave_no, mi.tdata[org_data.track].pipi);
				PlayOrganKey(SamplePlayHeight, org_data.track, 1000, 240, mi.tdata[org_data.track].pipi);
			}
			else
			{
				InitDramObject(Wave_no_to_List_no[i], org_data.track - MAXMELODY);
				PlayOrganKey(SamplePlayHeight, org_data.track, 1000, 240);
			}
		}
		return TRUE;
	}
	return FALSE;
}
