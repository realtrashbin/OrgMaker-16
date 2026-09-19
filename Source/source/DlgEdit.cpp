#include "Setting.h"
#include "DefOrg.h"
#include <string.h>
#include <stdio.h>
#include "Sound.h"
#include "resource.h"
#include "OrgData.h"
#include "Gdi.h"
#include "rxoFunction.h"
#include "TrackFlag.h"

extern HWND hDlgPlayer; //The player
extern HWND hDlgTrack; //The thing that holds the Tracks, mute, etc
extern NOTECOPY nc_Select; //Notecopy Select
extern int tra, ful ,haba; //TRAck,
extern char TrackN[]; //Track Number
extern char *dram_name[]; //Letter for the dram I think

//Delete TRACK parts function
BOOL CALLBACK DialogDelete(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char str[128] = {NULL};
	int v;
	long a,b,c;
	RECT rect = {64,0,WWidth,WHeight};//更新する領域
	MUSICINFO mi;
	PARCHANGE pc;
	switch(message){
	case WM_INITDIALOG://ダイアログが呼ばれた
		EnableWindow(hDlgPlayer,FALSE);

		//itoa(org_data.track,str,10);
		//SetDlgItemText(hdwnd,IDE_DELTRACK,str);
		SetDlgItemText(hdwnd,IDE_DELTRACK,TrackCode[org_data.track]);
		if(tra<0){
			SetDlgItemText(hdwnd,IDE_DELFROM,"");
			SetDlgItemText(hdwnd,IDE_DELTO,"");
		}else{
			v = GetSelectMeasBeat(GET_MEAS1);
			SetDlgItemInt(hdwnd,IDE_DELFROM,v,FALSE);
			v = GetSelectMeasBeat(GET_MEAS2);
			SetDlgItemInt(hdwnd,IDE_DELTO,v,FALSE);
		}
		
		return 1;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDCANCEL:
			EndDialog(hdwnd,0);
			EnableWindow(hDlgPlayer,TRUE);
			return 1;
		case IDOK:
			GetDlgItemText(hdwnd, IDE_DELTRACK, str, 4);
			a = binTrackCode(str);
			GetDlgItemText(hdwnd,IDE_DELTRACK,str,4);
			if(binTrackCode(str) == 99){
				MessageBox(hdwnd,"Track is invalid.","Could not Delete",MB_OK | MB_ICONERROR);

				return 1;
			}
			//パラメータ生成
			
			pc.track = a;
			org_data.GetMusicInfo(&mi);
			GetDlgItemText(hdwnd,IDE_DELFROM,str,4);
			b = atol(str);
			pc.x1 = b * mi.dot*mi.line;
			GetDlgItemText(hdwnd,IDE_DELTO,str,4);
			c = atol(str);
			pc.x2 = c * mi.dot*mi.line-1;
			if(b >= c){
				MessageBox(hdwnd,"Range is invalid.","Could not Delete",MB_OK | MB_ICONERROR);
				return 1;
			}
			SetUndo();
			//消去
			org_data.DelateNoteData(&pc);
			//表示
			//org_data.PutMusic();
			//RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
			//MessageBox(hdwnd,"Track range deleted.","Done",MB_OK);	// 2014.10.18 D
			return 1;
		}
	}
	return 0;
}

//Copy TRACK function
BOOL CALLBACK DialogCopy(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char str[128] = {NULL};
	long a,b,c;
	RECT rect = {64,0,WWidth,WHeight};//更新する領域
	MUSICINFO mi;
//	PARCHANGE pc;
	NOTECOPY nc;
	switch(message){
	case WM_INITDIALOG://ダイアログが呼ばれた
		//itoa(org_data.track,str,10);
		//SetDlgItemText(hdwnd,IDE_TRACK1,str);
		//SetDlgItemText(hdwnd,IDE_TRACK2,str);
		if(tra<0){
			SetDlgItemText(hdwnd,IDE_TRACK1,TrackCode[org_data.track]);
			SetDlgItemText(hdwnd,IDE_TRACK2,TrackCode[org_data.track]);
			SetDlgItemText(hdwnd,IDE_MEAS1_1,"");
			{
				char c[64];
				GetDlgItemText(hDlgPlayer,IDE_VIEWMEAS, c , 32);
				SetDlgItemText(hdwnd,IDE_MEAS1_1,c);
			}
			SetDlgItemText(hdwnd,IDE_MEAS1_2,"");
			SetDlgItemText(hdwnd,IDE_BEAT1_1,"0");
			SetDlgItemText(hdwnd,IDE_BEAT1_2,"0");
			SetDlgItemText(hdwnd,IDE_MEAS2,"");
			SetDlgItemText(hdwnd,IDE_BEAT2,"0");
		}else{
			SetDlgItemText(hdwnd,IDE_TRACK1,TrackCode[tra]);
			SetDlgItemText(hdwnd,IDE_TRACK2,TrackCode[org_data.track]);
			{
				char c[64];
				GetDlgItemText(hDlgPlayer,IDE_VIEWMEAS, c , 32);
				SetDlgItemText(hdwnd,IDE_MEAS1_1,c);
			}
			SetDlgItemInt(hdwnd,IDE_MEAS1_1,GetSelectMeasBeat(GET_MEAS1),FALSE);
			SetDlgItemInt(hdwnd,IDE_MEAS1_2,GetSelectMeasBeat(GET_MEAS2),FALSE);
			SetDlgItemInt(hdwnd,IDE_BEAT1_1,GetSelectMeasBeat(GET_BEAT1),FALSE);
			SetDlgItemInt(hdwnd,IDE_BEAT1_2,GetSelectMeasBeat(GET_BEAT2),FALSE);
			SetDlgItemText(hdwnd,IDE_MEAS2,"");
			SetDlgItemText(hdwnd,IDE_BEAT2,"0");
			
		}
		SetDlgItemText(hdwnd,IDE_COPYNUM,"1");
		EnableWindow(hDlgPlayer,FALSE);
		return 1;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDCANCEL:
			EndDialog(hdwnd,0);
			EnableWindow(hDlgPlayer,TRUE);
			return 1;
		case IDOK:
			//コピー回数のチェック
			GetDlgItemText(hdwnd,IDE_COPYNUM,str,4);
			if(atol(str) < 1){
				MessageBox(hdwnd,"Copy range is invalid.","Could not copy Track",MB_OK | MB_ICONERROR);	// 2014.10.18 D
				return 1;
			}
			//トラックのチェック
			GetDlgItemText(hdwnd,IDE_TRACK1,str,4);
			if(binTrackCode(str) == 99){
				MessageBox(hdwnd,"'From' Track is not valid.","Could not copy Track",MB_OK | MB_ICONERROR);	// 2014.10.18 D
				return 1;
			}
			else
			{
				nc.track1 = binTrackCode(str);
			}
			GetDlgItemText(hdwnd,IDE_TRACK2,str,4);
			if (binTrackCode(str) == 99) {
				MessageBox(hdwnd, "'To' Track is not valid.", "Could not copy Track", MB_OK | MB_ICONERROR);	// 2014.10.18 D
				return 1;
			}
			else nc.track2 = binTrackCode(str);
			//コピー範囲のチェック
			org_data.GetMusicInfo(&mi);
			GetDlgItemText(hdwnd,IDE_MEAS1_1,str,4);//範囲from
			a = atol(str)*mi.dot*mi.line;
			GetDlgItemText(hdwnd,IDE_BEAT1_1,str,4);
			if(atol(str) >= mi.dot*mi.line){
				//MessageBox(hdwnd,"From Beatが異常です","Error(Copy)",MB_OK);	// 2014.10.19 D
				msgbox(hdwnd,IDS_WARNING_FROM_BEAT,IDS_ERROR_COPY,MB_OK);	// 2014.10.19 A
				return 1;
			}
			a += atol(str);
			GetDlgItemText(hdwnd,IDE_MEAS1_2,str,4);//範囲to
			b = atol(str)*mi.dot*mi.line;
			GetDlgItemText(hdwnd,IDE_BEAT1_2,str,4);
			if(atol(str) >= mi.dot*mi.line){
				//MessageBox(hdwnd,"From Beatが異常です","Error(Copy)",MB_OK);	// 2014.10.19 D
				msgbox(hdwnd,IDS_WARNING_FROM_BEAT,IDS_ERROR_COPY,MB_OK);	// 2014.10.19 A
				return 1;
			}
			b += atol(str)-1;
			GetDlgItemText(hdwnd,IDE_MEAS2,str,4);//コピー先to
			c = atol(str)*mi.dot*mi.line;
			GetDlgItemText(hdwnd,IDE_BEAT2,str,4);
			if(atol(str) >= mi.dot*mi.line){
				//MessageBox(hdwnd,"To Beatが異常です","Error(Copy)",MB_OK);	// 2014.10.19 D
				msgbox(hdwnd,IDS_WARNING_TO_BEAT,IDS_ERROR_COPY,MB_OK);	// 2014.10.19 A
				return 1;
			}
			if(a >= b){
				//MessageBox(hdwnd,"コピー範囲が異常です","Error(Copy)",MB_OK);	// 2014.10.19 D
				msgbox(hdwnd,IDS_WARNING_COPY_HANI,IDS_ERROR_COPY,MB_OK);	// 2014.10.19 A
				return 1;
			}
			c += atol(str);
			nc.x1_1 = a;
			nc.x1_2 = b;
			nc.x2 = c;
			GetDlgItemText(hdwnd,IDE_COPYNUM,str,4);//コピー先to
			nc.num = atol(str);

			SetUndo();
			org_data.CopyNoteData(&nc);
			org_data.CheckNoteTail(nc.track2);
			//MessageBox(hdwnd,"Track has been copied.","Done",MB_OK);	// 2014.10.19 D
			return 1;
		}
	}
	return 0;
}

//Change pan for TRACK function
BOOL CALLBACK DialogPan(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char str[128] = {NULL};
//	long a,b,c,d;
	RECT rect = {64,0,WWidth,WHeight};//更新する領域
	PARCHANGE pc;
	MUSICINFO mi;
	switch(message){
	case WM_INITDIALOG://ダイアログが呼ばれた
		SendDlgItemMessage(hdwnd,IDR_ADD,BM_SETCHECK,1,0);
		//itoa(org_data.track,str,10);
		//SetDlgItemText(hdwnd,IDE_TRACK,str);
		SetDlgItemText(hdwnd,IDE_TRACK,TrackCode[org_data.track]);
		if(tra<0){
			SetDlgItemText(hdwnd,IDE_MEAS1,"");
			SetDlgItemText(hdwnd,IDE_MEAS2,"");
		}else{
			SetDlgItemInt(hdwnd,IDE_MEAS1,GetSelectMeasBeat(GET_MEAS1),FALSE);
			SetDlgItemInt(hdwnd,IDE_MEAS2,GetSelectMeasBeat(GET_MEAS2),FALSE);
		}
		SetDlgItemText(hdwnd,IDE_PAR,"1");
		EnableWindow(hDlgPlayer,FALSE);
		return 1;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDCANCEL:
			EndDialog(hdwnd,0);
			EnableWindow(hDlgPlayer,TRUE);
			return 1;
		case IDOK:
			if(SendDlgItemMessage(hdwnd,IDR_ADD,BM_GETCHECK,0,0))
				pc.mode = MODEPARADD;
			else if(SendDlgItemMessage(hdwnd,IDR_SUB,BM_GETCHECK,0,0))
				pc.mode = MODEPARSUB;
			//トラックのチェック
			GetDlgItemText(hdwnd,IDE_TRACK,str,4);
			if(binTrackCode(str) == 99){
				MessageBox(hdwnd,"Track is invalid.","Could not change Panning",MB_OK | MB_ICONERROR);	// 2014.10.19 D
				return 1;
			}
			else pc.track = binTrackCode(str);
			//範囲のチェック
			org_data.GetMusicInfo(&mi);
			GetDlgItemText(hdwnd,IDE_MEAS1,str,4);//範囲from
			pc.x1 = atol(str)*mi.dot*mi.line;
			GetDlgItemText(hdwnd,IDE_MEAS2,str,4);//範囲to
			pc.x2 = atol(str)*mi.dot*mi.line - 1;
			GetDlgItemText(hdwnd,IDE_PAR,str,4);//値
			pc.a = (unsigned char)atol(str);
			if(pc.x1 >= pc.x2){
				MessageBox(hdwnd,"Range is invalid.","Could not change Panning",MB_OK | MB_ICONERROR);	// 2014.10.19 D
				return 1;
			}
			SetUndo();
			org_data.ChangePanData(&pc);
			//表示
			//org_data.PutMusic();
			//RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
			//MessageBox(hdwnd,"Panning has been changed.","Done",MB_OK);	// 2014.10.19 D
			return 1;
		}
	}
	return 0;
}

//Transpose note for TRACK function
BOOL CALLBACK DialogTrans(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char str[128] = {NULL};
//	long a,b,c,d;
	RECT rect = {64,0,WWidth,WHeight};//更新する領域
	PARCHANGE pc;
	MUSICINFO mi;
	switch(message){
	case WM_INITDIALOG://ダイアログが呼ばれた
		SendDlgItemMessage(hdwnd,IDR_ADD,BM_SETCHECK,1,0);
		//itoa(org_data.track,str,10);
		//SetDlgItemText(hdwnd,IDE_TRACK,str);
		SetDlgItemText(hdwnd,IDE_TRACK,TrackCode[org_data.track]);
		if(tra<0){
			SetDlgItemText(hdwnd,IDE_MEAS1,"");
			SetDlgItemText(hdwnd,IDE_MEAS2,"");
		}else{
			SetDlgItemInt(hdwnd,IDE_MEAS1,GetSelectMeasBeat(GET_MEAS1),FALSE);
			SetDlgItemInt(hdwnd,IDE_MEAS2,GetSelectMeasBeat(GET_MEAS2),FALSE);
		}
		SetDlgItemText(hdwnd,IDE_PAR,"1");
		EnableWindow(hDlgPlayer,FALSE);
		return 1;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDCANCEL:
			EndDialog(hdwnd,0);
			EnableWindow(hDlgPlayer,TRUE);
			return 1;
		case IDOK:
			if(SendDlgItemMessage(hdwnd,IDR_ADD,BM_GETCHECK,0,0))
				pc.mode = MODEPARADD;
			else if(SendDlgItemMessage(hdwnd,IDR_SUB,BM_GETCHECK,0,0))
				pc.mode = MODEPARSUB;
			
			//トラックのチェック
			GetDlgItemTextA(hdwnd,IDE_TRACK,str,4);
			if(binTrackCode(str) == 99){
				MessageBox(hdwnd,"Track is invalid.","Could not Transpose",MB_OK | MB_ICONERROR);	// 2014.10.19 D
				return 1;
			}
			else pc.track = binTrackCode(str);
			//範囲のチェック
			org_data.GetMusicInfo(&mi);
			GetDlgItemText(hdwnd,IDE_MEAS1,str,4);//範囲from
			pc.x1 = atol(str)*mi.dot*mi.line;
			GetDlgItemText(hdwnd,IDE_MEAS2,str,4);//範囲to
			pc.x2 = atol(str)*mi.dot*mi.line - 1;
			GetDlgItemText(hdwnd,IDE_PAR,str,4);//値
			pc.a = (unsigned char)atol(str);
			if(pc.x1 >= pc.x2){
				MessageBox(hdwnd,"Range is invalid.","Could not Transpose",MB_OK | MB_ICONERROR);	// 2014.10.19 D
				return 1;
			}
			SetUndo();
			org_data.ChangeTransData(&pc);
			//表示
			//org_data.PutMusic();
			//RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
			MessageBox(hdwnd,"Track has been transposed.","Done",MB_OK);	// 2014.10.19 D
			return 1;
		}
	}
	return 0;
}

//Change Volume for TRACK function
BOOL CALLBACK DialogVolume(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char str[128] = {NULL};
//	long a,b,c,d;
	RECT rect = {64,0,WWidth,WHeight};//更新する領域
	PARCHANGE pc;
	MUSICINFO mi;
	switch(message){
	case WM_INITDIALOG://ダイアログが呼ばれた
		SendDlgItemMessage(hdwnd,IDR_ADD,BM_SETCHECK,1,0);
		//itoa(org_data.track,str,10);
		//SetDlgItemText(hdwnd,IDE_TRACK,str);
		SetDlgItemText(hdwnd,IDE_TRACK,TrackCode[org_data.track]);
		if(tra<0){
			SetDlgItemText(hdwnd,IDE_MEAS1,"");
			SetDlgItemText(hdwnd,IDE_MEAS2,"");
		}else{
			SetDlgItemInt(hdwnd,IDE_MEAS1,GetSelectMeasBeat(GET_MEAS1),FALSE);
			SetDlgItemInt(hdwnd,IDE_MEAS2,GetSelectMeasBeat(GET_MEAS2),FALSE);
		}
		SetDlgItemText(hdwnd,IDE_PAR,"1");
		EnableWindow(hDlgPlayer,FALSE);
		return 1;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDCANCEL:
			EndDialog(hdwnd,0);
			EnableWindow(hDlgPlayer,TRUE);
			return 1;
		case IDOK:
			if(SendDlgItemMessage(hdwnd,IDR_ADD,BM_GETCHECK,0,0))
				pc.mode = MODEPARADD;
			else if(SendDlgItemMessage(hdwnd,IDR_SUB,BM_GETCHECK,0,0))
				pc.mode = MODEPARSUB;
			//トラックのチェック
			GetDlgItemText(hdwnd,IDE_TRACK,str,4);
			if(binTrackCode(str) == 99){
				MessageBox(hdwnd,"Track is invalid.","Could not change Volume",MB_OK | MB_ICONERROR);	// 2014.10.19 D
				return 1;
			}else pc.track = binTrackCode(str);
			//範囲のチェック
			org_data.GetMusicInfo(&mi);
			GetDlgItemText(hdwnd,IDE_MEAS1,str,4);//範囲from
			pc.x1 = atol(str)*mi.dot*mi.line;
			GetDlgItemText(hdwnd,IDE_MEAS2,str,4);//範囲to
			pc.x2 = atol(str)*mi.dot*mi.line - 1;
			GetDlgItemText(hdwnd,IDE_PAR,str,4);//値
			pc.a = (unsigned char)atol(str);
			if(pc.x1 >= pc.x2){
				MessageBox(hdwnd,"Range is invalid.","Could not change Volume",MB_OK | MB_ICONERROR);	// 2014.10.19 D
				return 1;
			}
			SetUndo();
			org_data.ChangeVolumeData(&pc, 0);
			//表示
			//org_data.PutMusic();
			//RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
			//MessageBox(hdwnd,"Volume has been changed.","Volume edit completed",MB_OK | MB_ICONINFORMATION);	// 2014.10.19 D
			return 1;
		}
	}
	return 0;
}

//Tracks Usage in
int cbox[MAXTRACK] = {
	IDC_USE0,
	IDC_USE1,
	IDC_USE2,
	IDC_USE3,
	IDC_USE4,
	IDC_USE5,
	IDC_USE6,
	IDC_USE7,
	IDC_USE8,
	IDC_USE9,
	IDC_USE10,
	IDC_USE11,
	IDC_USE12,
	IDC_USE13,
	IDC_USE14,
	IDC_USE15,

	IDC_USEDxQ,
	IDC_USEDxW,
	IDC_USEDxE,
	IDC_USEDxR,
	IDC_USEDxT,
	IDC_USEDxY,
	IDC_USEDxU,
	IDC_USEDxI,
	IDC_USEDxA,
	IDC_USEDxS,
	IDC_USEDxD,
	IDC_USEDxF,
	IDC_USEDxG,
	IDC_USEDxH,
	IDC_USEDxJ,
	IDC_USEDxK,
};

BOOL CALLBACK DialogMultiDelete(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char str[128] = { NULL };
	RECT rect = {64,0,WWidth,WHeight};
	int  i;
	bool use;
	PARCHANGE pc;
	MUSICINFO mi;
	switch (message)
	{
	case WM_INITDIALOG:
		GetDlgItemText(hDlgPlayer, GET_MEAS1,str,4);
		SetDlgItemText(hdwnd, IDE_MEAS1_1, str);

		EnableWindow(hDlgPlayer, FALSE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_SETCOPY_FULLCLICK: //All
			for (i = 0; i < MAXTRACK; i++)SendDlgItemMessage(hdwnd, cbox[i], BM_SETCHECK, 1, 0);
			return 1;
		case IDC_SETCOPY_FULLCLICK2: //Current
			for (i = 0; i < MAXTRACK; i++)SendDlgItemMessage(hdwnd, cbox[i], BM_SETCHECK, 0, 0);
			SendDlgItemMessage(hdwnd, cbox[org_data.track], BM_SETCHECK, 1, 0);
			return 1;
		case IDC_SETCOPY_FULLCLICK3: //Melody
			for (i = 0; i < MAXMELODY; i++)
			{
				SendDlgItemMessage(hdwnd, cbox[i], BM_SETCHECK, 1, 0);
			}
			for (i = MAXMELODY; i < MAXTRACK; i++)
			{
				SendDlgItemMessage(hdwnd, cbox[i], BM_SETCHECK, 0, 0);
			}
			return 1;
		case IDC_SETCOPY_FULLCLICK4: //Drams
			for (i = 0; i < MAXMELODY; i++)
			{
				SendDlgItemMessage(hdwnd, cbox[i], BM_SETCHECK, 0, 0);
			}
			for (i = MAXMELODY; i < MAXTRACK; i++)
			{
				SendDlgItemMessage(hdwnd, cbox[i], BM_SETCHECK, 1, 0);
			}
			return 1;
		case IDCANCEL:
			EndDialog(hdwnd, 0);
			EnableWindow(hDlgPlayer, true);
			return 1;
			break;
		case IDOK:
			org_data.GetMusicInfo(&mi);
			GetDlgItemText(hdwnd, IDE_MEAS1_1, str, 4);
			pc.x1 = atoi(str)*mi.dot*mi.line;
			GetDlgItemText(hdwnd, IDE_MEAS1_2, str, 4);
			pc.x2 = atoi(str)*mi.dot*mi.line-1;
			if (pc.x1 >= pc.x2)
			{
				MessageBox(hdwnd, "Range is invalid.", "Could not Delete", MB_OK | MB_ICONERROR);
				return 1;
			}
			use = false;
			for (i = 0; i < MAXTRACK; i++)
			{
				if (SendDlgItemMessageA(hdwnd, cbox[i], BM_GETCHECK, 0, 0) == 1)
				{
					use = true;
					break;
				}
			}
			if (use == false)
			{
				MessageBox(hdwnd, "You must select one track.", "Could not Delete", MB_OK | MB_ICONERROR);
				return 1;
			}
			SetUndo();
			for (i = 0; i < MAXTRACK; i++)
			{
				pc.track = i;
				if (SendDlgItemMessageA(hdwnd, cbox[i], BM_GETCHECK, 0, 0) == 1)
				{
					org_data.DelateNoteData(&pc);
				}
			}
			//MessageBox(hdwnd, "Selected Tracks deleted.", "Done", MB_OK);
			
			break;
		}
		break;
	}

	return 0;
}

//Multi-Copy function
BOOL CALLBACK DialogCopy2(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	char str[128] = {NULL};
	long a,b,c;
	bool use;
	RECT rect = {64,0,WWidth,WHeight};//更新する領域
	MUSICINFO mi;
//	PARCHANGE pc;
	NOTECOPY nc;
	switch(message){
	case WM_INITDIALOG://ダイアログが呼ばれた
//		itoa(org_data.track,str,10);
//		SetDlgItemText(hdwnd,IDE_TRACK1,str);
//		SetDlgItemText(hdwnd,IDE_TRACK2,str);

		//SendDlgItemMessage(hdwnd,cbox[org_data.track],BM_SETCHECK,1,0);
		for(i=0;i<MAXTRACK;i++)SendDlgItemMessage(hdwnd,cbox[i],BM_SETCHECK,1,0);
		if(tra<0){
			SetDlgItemText(hdwnd,IDE_MEAS1_1,"");
			{
				char c[64];
				GetDlgItemText(hDlgPlayer,IDE_VIEWMEAS, c , 32);
				SetDlgItemText(hdwnd,IDE_MEAS1_1,c);
			}
			SetDlgItemText(hdwnd,IDE_MEAS1_2,"");
			SetDlgItemText(hdwnd,IDE_BEAT1_1,"0");
			SetDlgItemText(hdwnd,IDE_BEAT1_2,"0");
		}else{
			SetDlgItemInt(hdwnd,IDE_MEAS1_1,GetSelectMeasBeat(GET_MEAS1),FALSE);
			SetDlgItemInt(hdwnd,IDE_MEAS1_2,GetSelectMeasBeat(GET_MEAS2),FALSE);
			SetDlgItemInt(hdwnd,IDE_BEAT1_1,GetSelectMeasBeat(GET_BEAT1),FALSE);
			SetDlgItemInt(hdwnd,IDE_BEAT1_2,GetSelectMeasBeat(GET_BEAT2),FALSE);
		}
		SetDlgItemText(hdwnd,IDE_MEAS2,"");
		SetDlgItemText(hdwnd,IDE_BEAT2,"0");
		SetDlgItemText(hdwnd,IDE_COPYNUM,"1");
		EnableWindow(hDlgPlayer,FALSE);
		return 1;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDC_SETCOPY_FULLCLICK: //All
			for(i=0;i<MAXTRACK;i++)SendDlgItemMessage(hdwnd,cbox[i],BM_SETCHECK,1,0);

			return 1;
		case IDC_SETCOPY_FULLCLICK2: //Current
			for(i=0;i<MAXTRACK;i++)SendDlgItemMessage(hdwnd,cbox[i],BM_SETCHECK,0,0);
			SendDlgItemMessage(hdwnd,cbox[org_data.track],BM_SETCHECK,1,0);
			return 1;
		case IDC_SETCOPY_FULLCLICK3: //Melody
			for (i = MAXMELODY; i < MAXTRACK; i++)
			{
				SendDlgItemMessage(hdwnd, cbox[i], BM_SETCHECK, 0, 0);
			}
			for (i = 0; i < MAXMELODY; i++)
			{
				SendDlgItemMessage(hdwnd, cbox[i], BM_SETCHECK, 1, 0);
			}
			return 1;
		case IDC_SETCOPY_FULLCLICK4: //Dram
			for (i = 0; i < MAXTRACK; i++)
			{
				SendDlgItemMessage(hdwnd, cbox[i], BM_SETCHECK, 0, 0);
			}
			for (i = MAXMELODY; i < MAXTRACK; i++)
			{
				SendDlgItemMessage(hdwnd, cbox[i], BM_SETCHECK, 1, 0);
			}
			return 1;
		case IDCANCEL:
			EndDialog(hdwnd,0);
			EnableWindow(hDlgPlayer,TRUE);
			return 1;
		case IDOK:
			//コピー回数のチェック
			GetDlgItemText(hdwnd,IDE_COPYNUM,str,4);
			if(atol(str) < 1){
				//MessageBox(hdwnd,"コピー回数が異常です","Error(Copy)",MB_OK);	// 2014.10.19 D
				msgbox(hdwnd,IDS_WARNING_COPY_KAISUU,IDS_ERROR_COPY,MB_OK);	// 2014.10.19 A
				return 1;
			}
			org_data.GetMusicInfo(&mi);
			GetDlgItemText(hdwnd,IDE_MEAS1_1,str,4);//範囲from
			a = atol(str)*mi.dot*mi.line;
			GetDlgItemText(hdwnd,IDE_BEAT1_1,str,4);
			if(atol(str) >= mi.dot*mi.line){
				//MessageBox(hdwnd,"From Beatが異常です","Error(Copy)",MB_OK);	// 2014.10.19 D
				msgbox(hdwnd,IDS_WARNING_FROM_BEAT,IDS_ERROR_COPY,MB_OK);	// 2014.10.19 A
				return 1;
			}
			a += atol(str);
			GetDlgItemText(hdwnd,IDE_MEAS1_2,str,4);//範囲to
			b = atol(str)*mi.dot*mi.line;
			GetDlgItemText(hdwnd,IDE_BEAT1_2,str,4);
			if(atol(str) >= mi.dot*mi.line){
				//MessageBox(hdwnd,"From Beatが異常です","Error(Copy)",MB_OK);	// 2014.10.19 D
				msgbox(hdwnd,IDS_WARNING_FROM_BEAT,IDS_ERROR_COPY,MB_OK);	// 2014.10.19 A
				return 1;
			}
			b += atol(str)-1;
			GetDlgItemText(hdwnd,IDE_MEAS2,str,4);//コピー先to
			c = atol(str)*mi.dot*mi.line;
			GetDlgItemText(hdwnd,IDE_BEAT2,str,4);
			if(atol(str) >= mi.dot*mi.line){
				//MessageBox(hdwnd,"To Beatが異常です","Error(Copy)",MB_OK);	// 2014.10.19 D
				msgbox(hdwnd,IDS_WARNING_TO_BEAT,IDS_ERROR_COPY,MB_OK);	// 2014.10.19 A
				return 1;
			}
			if(a >= b){
				//MessageBox(hdwnd,"コピー範囲が異常です","Error(Copy)",MB_OK);	// 2014.10.19 D
				msgbox(hdwnd,IDS_WARNING_COPY_HANI,IDS_ERROR_COPY,MB_OK);	// 2014.10.19 A
				return 1;
			}
			use = false;
			for (i = 0; i < MAXTRACK; i++)
			{
				if (SendDlgItemMessage(hdwnd, cbox[i], BM_GETCHECK, 0, 0) == 1)use = true;
				else continue;
			}
			if (use == false)
			{
				MessageBox(hdwnd, "You must select one track.", "Could not copy Track(s)", MB_OK | MB_ICONERROR);
				return 1;
			}

			c += atol(str);
			nc.x1_1 = a;
			nc.x1_2 = b;
			nc.x2 = c;
			GetDlgItemText(hdwnd,IDE_COPYNUM,str,4);//コピー先to
			nc.num = atol(str);
			SetUndo();
			for(i = 0; i < MAXTRACK; i++){
				if(SendDlgItemMessage(hdwnd,cbox[i],BM_GETCHECK,0,0) == 1){
					nc.track1 = nc.track2 = i;
					org_data.CopyNoteData(&nc);
					org_data.CheckNoteTail(nc.track2);
					use = true;
				}
			}
			msgbox(hdwnd,IDS_COPY,IDS_NOTIFY_TITLE,MB_OK);	// 2014.10.19 A
			return 1;
		}
	}
	return 0;
}

//Swap TRACK function
BOOL CALLBACK DialogSwap(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char str[128] = {NULL};
//	long a,b,c,d;
	RECT rect = {64,0,WWidth,WHeight};//抗ect of the Dialog box?
	MUSICINFO mi;
	NOTECOPY nc;
	switch(message){
	case WM_INITDIALOG://Dialog box that asks what you wanna swap.
		if(tra<0){
			SetDlgItemText(hdwnd,IDE_TRACK1,TrackCode[org_data.track]);
			SetDlgItemText(hdwnd,IDE_TRACK2,TrackCode[org_data.track]);
		}else{
			SetDlgItemText(hdwnd,IDE_TRACK1,TrackCode[tra]);
			SetDlgItemText(hdwnd,IDE_TRACK2,TrackCode[org_data.track]);
		}
		EnableWindow(hDlgPlayer,FALSE);
		return 1; //The dialog box has been initiated.
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDCANCEL://Cancel button
			EndDialog(hdwnd,0);
			EnableWindow(hDlgPlayer,TRUE);
			return 1; //Closes the Dialog Box since you canceled.
		case IDOK:
			//Errors 
			GetDlgItemText(hdwnd,IDE_TRACK1,str,4);
			if (binTrackCode(str) == 99)
			{ 
				MessageBox(hdwnd, "Track does not exist.", "Could not swap tracks", MB_OK | MB_ICONERROR);	// 2014.10.19 D
				return 1; //Return exit code
			}else nc.track1 = binTrackCode(str); //Sets Track1 to the 1st input Track
			GetDlgItemText(hdwnd,IDE_TRACK2,str,4);
			if(binTrackCode(str) == 99){
				MessageBox(hdwnd,"Track does not exist.","Could not swap tracks",MB_OK | MB_ICONERROR);	// 2014.10.19 D
				return 1; //Return exit code
			}else nc.track2 = binTrackCode(str);//Sets Track2 to the 2nd input Track

			SetUndo();
			org_data.SwapTrack(&nc);

			org_data.GetMusicInfo(&mi);

			if (nc.track1 < MAXMELODY) MakeOrganyaWave(nc.track1, mi.tdata[nc.track1].wave_no, mi.tdata[nc.track1].pipi);
			else InitDramObject(mi.tdata[nc.track1].wave_no, nc.track1 - MAXMELODY); //if track 1 is dram

			if (nc.track2 < MAXMELODY) MakeOrganyaWave(nc.track2, mi.tdata[nc.track2].wave_no, mi.tdata[nc.track2].pipi);
			else InitDramObject(mi.tdata[nc.track2].wave_no, nc.track2 - MAXMELODY);//if track 2 is a dram

			//MessageBox(hdwnd,"Tracks Swapped.","Track swap completed",MB_OK | MB_ICONINFORMATION);	// 2014.10.19 D
			return 1;//Ends the function after a job well done.
		}
	}
	return 0;
}
