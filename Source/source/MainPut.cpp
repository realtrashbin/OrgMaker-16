#include <string.h>
#include "Setting.h"
#include "DefOrg.h"
#include "Gdi.h"
#include "OrgData.h"
#include "Scroll.h"
#include "Sound.h"
#include "resource.h"

#include "rxoFunction.h"

extern int sACrnt;	//範囲選択は常にｶﾚﾝﾄﾄﾗｯｸ

extern HWND hDlgPlayer;
extern char timer_sw;
extern int sSmoothScroll;


RECT symbol_rect[] = {
	{ 0, 0, 16, 5},//Volume/Panning symbol
	{  0, 6, 15, 16},//Repeat symbol 1
	{ 16, 6, 31, 16},//Repeat symbol 2
	{ 32, 0, 47, 7} //FLAG symbol
};

RECT note_rect[]={ //殖MPNOTE
	{ 48,  0, 64,  6}, //Blue Note
	{64,  0, 80,  6}, //Red Note
	{ 48,  8, 64,  14}, //Purple Note
	{64,  8, 80,  14}, //Green Note
	{ 48,  16, 64,  22}, //Cyan Note
	{64,  16, 80,  22}, //Yellow Note
	{ 48,  24, 64,  30}, //Gray Note
	{64,  24, 80,  30}, //Beige Note


	{131,  128,  147,  134}, //9th Note(Bi_N)
	{147,  128,  163,  134}, //Mint Note(Bi_N)
	{131,  136,  147,  142}, //Blueish Note(Bi_N)
	{147,  136,  163,  142}, //Pinkish Note(Bi_N)
	{131,  144,  147,  150}, //Beige Pinkish Note(Bi_N)
	{147,  144,  163,  150}, //Niko Note(Bi_N) kitty!!~~
	{131,  152,  147,  158}, //Purple-ish Note(Bi_N)
	{147,  152,  163,  158}, //Another Purple-ish Note(Bi_N)

	{ 97,  0, 113,  6}, //Blue Note(Dram)
	{113,  0, 129,  6}, //Red Note(Dram)
	{ 97,  8, 113,  14}, //Purple Note(Dram)
	{113,  8, 129,  14}, //Green Note(Dram)
	{ 97,  16, 113, 22}, //Cyan Note(Dram)
	{113,  16, 129, 22}, //Yellow Note(Dram)
	{ 97, 24, 113, 30}, //Gray Note(Dram)
	{113, 24, 129, 30}, //Beige Note(Dram)


	{ 180,128,196,134}, //9th Note(Dram)
	{196,128,212,134}, //Mint Note(Dram)
	{ 180,136,196,142}, //Blueish Note(Dram)
	{196,136,212,142}, //Pinkish Note(Dram)
	{ 180,144,196,150}, //Beige pinkish Note(Dram)
	{196,144,212,150}, //Niko Note(Dram) Kityy!!!~~~~~
	{ 180,152,196,158}, //Purple-ish Note(Dram)
	{196,152,212,158}, //Another purple-ish Note(Dram)
	
	{ 48, 37, 64, 43}, //Blue Note light
	{64, 37, 80, 43}, //Red Note light
	{ 48, 45, 64, 51}, //Purple Note light
	{64, 45, 80, 51}, //Green Note light
	{ 48, 53, 64, 59}, //Cyan Note light
	{64, 53, 80, 59}, //Yellow Note light
	{ 48, 61, 64, 67}, //Gray Note light
	{64, 61, 80, 67}, //Beige Note light


	{131,  165,  147,  171}, //9th Note light(Bi_N)
	{147,  165,  163,  171}, //Mint Note light(Bi_N)
	{131,  173,  147,  179}, //Blueish Note light(Bi_N)
	{147,  173,  163,  179}, //Pinkish Note light(Bi_N)
	{131,  181,  147,  187}, //Beige Pinkish Note light(Bi_N)
	{147,  181,  163,  187}, //Niko Note light(Bi_N) kitty!!~~
	{131,  189,  147,  195}, //Purple-ish Note light(Bi_N)
	{147,  189,  163,  195}, //Another Purple-ish Note light(Bi_N)

	{ 97, 37, 113, 43}, //Blue Note(Dram) light
	{113, 37, 129, 43}, //Red Note(Dram) light
	{ 97, 45, 113, 51}, //Purple Note(Dram) light
	{113, 45, 129, 51}, //Green Note(Dram) light
	{ 97, 53, 113, 59}, //Cyan Note(Dram) light
	{113, 53, 129, 59}, //Yellow Note(Dram) light
	{ 97, 61, 113, 67}, //Gray Note(Dram) light
	{113, 61, 129, 67}, //Beige Note(Dram) light


	{ 180,165,196,171}, //9th Note(Dram) light
	{196,165,212,171}, //Mint Note(Dram) light
	{ 180,173,196,179}, //Blueish Note(Dram) light
	{196,173,212,179}, //Pinkish Note(Dram) light
	{ 180,181,196,187}, //Beige pinkish Note(Dram) light
	{196,181,212,187}, //Niko Note(Dram) Kityy!!!~~~~~ light
	{ 180,189,196,195}, //Purple-ish Note(Dram) light
	{196,189,212,195}, //Another purple-ish Note(Dram) light
};

RECT note_tail_rect[]={ //しっぽ
	{80, 37, 96, 40}, //Blue Note Tail light
	{80, 41, 96, 44}, //Red Note Tail light
	{80, 45, 96, 48}, //Purple Note Tail light
	{80, 49, 96, 52}, //Green Note Tail light
	{80, 53, 96, 56}, //Cyan Note Tail light
	{80, 57, 96, 60}, //Yellow Note Tail light
	{80, 61, 96, 64}, //Gray Note Tail light
	{80, 65, 96, 68}, //Beige Note Tail light
	

	{163, 165, 179, 168}, //Midnight Note Tail light(Bi_N)
	{163, 169, 179, 172}, //Red Note Tail light
	{163, 173, 179, 176}, //Purple Note Tail light
	{163, 177, 179, 180}, //Green Note Tail light
	{163, 181, 179, 184}, //Cyan Note Tail light
	{163, 185, 179, 188}, //Yellow Note Tail light
	{163, 189, 179, 192}, //Gray Note Tail light
	{163, 193, 179, 196}, //Beige Note Tail light


	{80, 0, 96, 3}, //Blue Note Tail
	{80, 4, 96, 7}, //Red Note Tail
	{80, 8, 96, 12}, //Purple Note Tail
	{80, 12, 96, 16}, //Green Note Tail
	{80, 16, 96, 20}, //Cyan Note Tail
	{80, 20, 96, 24}, //Yellow Note Tail
	{80, 24, 96, 28}, //Gray Note Tail
	{80, 28, 96, 32}, //Beige Note Tail
	

	{163, 128, 179, 131}, //Midnight Note Tail(Bi_N)
	{163, 132, 179, 135}, //Red Note Tail
	{163, 136, 179, 139}, //Purple Note Tail
	{163, 140, 179, 143}, //Green Note Tail
	{163, 144, 179, 147}, //Cyan Note Tail
	{163, 148, 179, 151}, //Yellow Note Tail
	{163, 152, 179, 155}, //Gray Note Tail
	{163, 156, 179, 159}, //Beige Note Tail
	
};
RECT msc_rect[] = { //Other BMP stuff
	{0,0,64,160},// Pan, Volume, Select
	{0,160,65,174},//FLAG
	{0, 109, 63, 125}, //Current Select
	{0, 125, 63, 196}, //Active Pan
	{64, 125, 127, 196} //Active Volume
};
RECT num_rect[] = {
	{  0,  0,  8, 12}, //White Numbers
	{  8,  0, 16, 12},
	{ 16,  0, 24, 12},
	{ 24,  0, 32, 12},
	{ 32,  0, 40, 12},
	{ 40,  0, 48, 12},
	{ 48,  0, 56, 12},
	{ 56,  0, 64, 12},
	{ 64,  0, 72, 12},
	{ 72,  0, 80, 12},

	{  0, 12,  8, 24}, //Black Numbers
	{  8, 12, 16, 24},
	{ 16, 12, 24, 24},
	{ 24, 12, 32, 24},
	{ 32, 12, 40, 24},
	{ 40, 12, 48, 24},
	{ 48, 12, 56, 24},
	{ 56, 12, 64, 24},
	{ 64, 12, 72, 24},
	{ 72, 12, 80, 24},
};
RECT rc_SelArea[] ={
	{0, 77, 16, 87}, //Select beginning light
	{16, 77, 32, 87}, //Select repeating light
	{32, 77, 48, 88}, //Select tail light
	{0, 93, 16, 103},  //Select beginning
	{16, 93, 32, 103}, //Repeating Select
	{32, 93, 48, 104}, //Select tail
	{53, 93, 68, 104}, //Select all
	{53, 77, 69, 88}, //Select one note
};

RECT rc_TCPY[]={ 
	{0,19,15,24}, //Gray Note
	{0,35,15,40}, //Gray Note(Black Key)
	{0,27,16,30}, //Gray Note Tail(White Key)
	{0,31,16,34}, //Gray Note Tail (Black Key)
};

//PushKB means Push Keyboard.
RECT rc_PushKB[] ={    //BMP MUSIC
	{112, 132, 208, 144}, //
	{112, 120, 208, 132}, //
	{112, 108, 208, 120}, //
	{112, 96, 208, 108}, //
	{112, 84, 208, 96}, //
	{112, 72, 208, 84}, //
	{112, 60, 208, 72}, //
	{112, 48, 208, 60}, //
	{112, 36, 208, 48}, //
	{112, 24, 208, 36}, //
	{112, 12, 208, 24}, //
	{112, 0, 208, 12}, //
};

RECT rc_PushKB2[] ={	//BMP MUSIC
	{112, 132, 176, 144}, //
	{112, 120, 176, 132}, //
	{112, 108, 176, 120}, //
	{112, 96, 176, 108}, //
	{112, 84, 176, 96}, //
	{112, 72, 176, 84}, //
	{112, 60, 176, 72}, //
	{112, 48, 176, 60}, //
	{112, 36, 176, 48}, //
	{112, 24, 176, 36}, //
	{112, 12, 176, 24}, //
	{112, 0, 176, 12}, //
};

RECT rc_PushKB3[] ={    //BMP MUSIC
	{208, 132, 272, 144}, //
	{208, 120, 272, 132}, //
	{208, 108, 272, 120}, //
	{208, 96, 272, 108}, //
	{208, 84, 272, 96}, //
	{208, 72, 272, 84}, //
	{208, 60, 272, 72}, //
	{208, 48, 272, 60}, //
	{208, 36, 272, 48}, //
	{208, 24, 272, 36}, //
	{208, 12, 272, 24}, //
	{208, 0, 272, 12}, //
};


extern NOTECOPY nc_Select; //選択範囲
extern int tra, ful ,haba; 
extern int gDrawDouble;	//両方のトラックグループを描画する

extern int iKeyPushDown[256]; // 2010.09.22 A ピアノキーの押され具合


//キーボードとピアノロールの位置
void OrgData::PutNumber(void)
{
	MUSICINFO mi;
	long scr_h,scr_v;
	long scalepos;
	int off, x;
	short k1000,k100,k10,k1;
	short i,j;
	short k = info.dot*info.line;
	short rr = 0;
	GetMusicInfo(&mi);
	scr_data.GetScrollPosition(&scr_h, &scr_v);
	j = (WWidth/NoteWidth)/k + (scr_h % k != 0 ? 1 : 0);
	//小節を表示
	for(i = 0; i <= j; i++){
		scalepos = scr_h / (mi.dot * mi.line);
		scalepos += i;
		if (scalepos < 0) continue;

		k1000 = k100 = k10 = k1 = 0;
		while (scalepos >= 1000) {
			k1000++;
			scalepos -= 1000;
		}
		while(scalepos >= 100){
			k100++;
			scalepos -= 100;
		}
		while(scalepos >= 10){
			k10++;
			scalepos -= 10;
		}
		
		off = (-(scr_h % (info.dot * info.line)) * NoteWidth);
		if (k1000 > 0) {
			x = k * i * NoteWidth + 0 + KEYWIDTH + 1 + off;
			if (x >= KEYWIDTH)
				PutBitmap(x, 15, &num_rect[k1000], BMPNUMBER);
			rr = 8;
		}
		x = k * i * NoteWidth + 0 + rr + KEYWIDTH + 1 + off;
		if (x >= KEYWIDTH)
			PutBitmap(x, 15, &num_rect[k100], BMPNUMBER);
		x = k * i * NoteWidth + 8 + rr + KEYWIDTH + 1 + off;
		if (x >= KEYWIDTH)
			PutBitmap(x, 15, &num_rect[k10], BMPNUMBER);
		x = k * i * NoteWidth + 16 + rr + KEYWIDTH + 1 + off;
		if (x >= KEYWIDTH)
			PutBitmap(x, 15, &num_rect[scalepos], BMPNUMBER);
		if(WHeight>550){
			rr = 0;
			if (k1000 > 0) {
				x = k * i * NoteWidth + 0 + KEYWIDTH + 1 + off;
				if (x >= KEYWIDTH)
					PutBitmap(x, WHeight + 288 - WHNM - 12, &num_rect[k1000], BMPNUMBER);
				rr = 8;
			}
			x = k * i * NoteWidth + 0 + rr + KEYWIDTH + 1 + off;
			if (x >= KEYWIDTH)
				PutBitmap(x, WHeight + 288 - WHNM - 12, &num_rect[k100], BMPNUMBER);
			x = k * i * NoteWidth + 8 + rr + KEYWIDTH + 1 + off;
			if (x >= KEYWIDTH)
				PutBitmap(x, WHeight + 288 - WHNM - 12, &num_rect[k10], BMPNUMBER);
			x = k * i * NoteWidth + 16 + rr + KEYWIDTH + 1 + off;
			if (x >= KEYWIDTH)
				PutBitmap(x, WHeight + 288 - WHNM - 12, &num_rect[scalepos], BMPNUMBER);
		}
	}
	//キーを表示
	for(i = 0; i <  8; i++){
		PutBitmap(55,(95 - scr_v - i*12)*12,&num_rect[i+10],BMPNUMBER);
	}
}
void OrgData::PutRepeat(void)
{
	long scr_h,scr_v;
	long x;
	scr_data.GetScrollPosition(&scr_h,&scr_v);
	x = (info.repeat_x - scr_h)*NoteWidth + KEYWIDTH;
	if (x >= KEYWIDTH)
		PutBitmap(x, WHeight + 276 - WHNM - (WHeight > 550 ? 12 : 0), &symbol_rect[1], BMPNOTE);
	x = (info.end_x - scr_h)*NoteWidth + KEYWIDTH;
	if (x >= KEYWIDTH)
		PutBitmap(x,WHeight+276-WHNM-(WHeight > 550 ? 12 : 0),&symbol_rect[2],BMPNOTE);
}

//音符の表示
void OrgData::PutNotes(int TPCY, bool vol)
{
	int i,j,k,t,addY;
	int n; //2010.09.23 A
	NOTELIST *p;
	long xpos,ypos;
	long scr_h,scr_v;
	unsigned char line,dot;
	MUSICINFO mi;


	GetMusicInfo(&mi);line = mi.line;dot = mi.dot;
	scr_data.GetScrollPosition(&scr_h,&scr_v);


	if (!vol) {
		for (n = 0; n < MAXMELODY; n++) {
			k = getRecentTrack(n, 0);
			if (k == track)continue;
			if ((p = info.tdata[k].note_list) == NULL)continue;
			while (p != NULL && p->to != NULL) {
				xpos = (p->x - scr_h) * NoteWidth + KEYWIDTH;
				if (xpos >= KEYWIDTH)break;
				p = p->to;
			}

			if (p == NULL)continue;
			if (p->from != NULL)p = p->from;
			while (p->from != NULL && p->y == KEYDUMMY)p = p->from;

			while (p != NULL) {
				ypos = (95 - p->y - scr_v) * 12;
				xpos = (p->x - scr_h) * NoteWidth + KEYWIDTH;
				t = (p->y % 12); if (t == 1 || t == 3 || t == 6 || t == 8 || t == 10)t = 1;else t = 0;
				if (xpos > WWidth)break;
				if (ypos >= 0 && ypos < WHeight + 286 - WHNM) {
					addY = 0;

					addY=-n*3; if(addY>=0)addY++;
					if (iSlideOverlapNotes != 0) {
						addY = -k / 2 + 2; 
						if (k >= 4)addY--;
					}

					for (j = 0, i = p->length - 1; i > 0; i--, j++) {//尻尾
						if (TPCY == 0) PutBitmap2(xpos + j * NoteWidth + NoteWidth, ypos + 3 + addY, &note_tail_rect[k + MAXMELODY], BMPNOTE);
						else PutBitmap2(xpos + j * NoteWidth + NoteWidth, ypos + 3 + addY, &rc_TCPY[2 + t], BMPNOTE);
					}
					if (TPCY == 0) PutBitmapHead(xpos, ypos + 2 + addY, &note_rect[k], BMPNOTE, p->length);
					else PutBitmapHead(xpos, ypos + 2 + addY, &rc_TCPY[t], BMPNOTE, p->length);//音符(新)
				}
				p = p->to;
			}
		}
	}

	if((p = info.tdata[track].note_list) == NULL || TPCY==1){

	}else{
		while(p != NULL && p->to != NULL){
			xpos = (p->x - scr_h)*NoteWidth + KEYWIDTH;
			if(xpos >= KEYWIDTH)break;
			p = p->to;
		}
		if(p == NULL){
		}else{
			if(p->from != NULL)p = p->from;
			while(p->from != NULL && p->y == KEYDUMMY)p = p->from;
			while(p != NULL){
				ypos = (95 - p->y - scr_v)*12;
				xpos = (p->x - scr_h)*NoteWidth + KEYWIDTH;
				if(xpos > WWidth)break;
				if(!vol && ypos >= 0 && ypos < WHeight+286-WHNM){
					for(j = 0,i = p->length-1; i > 0; i--,j++){
						PutBitmap2(xpos+j*NoteWidth+NoteWidth,ypos+3,&note_tail_rect[track],BMPNOTE);
					}
					PutBitmapHead(xpos,ypos+2,&note_rect[track+MAXTRACK],BMPNOTE,p->length);
				}
				if (vol) {
					if (p->pan != PANDUMMY) {
						ypos = WHeight + 351 - WHNM - (p->pan * 5);//パン
						PutBitmapCenter(xpos, ypos, &symbol_rect[0], BMPNOTE);
					}
					if (p->volume != VOLDUMMY) {
						ypos = WHeight + 426 - WHNM - (p->volume / 4);//ボリューム
						PutBitmapCenter(xpos, ypos, &symbol_rect[0], BMPNOTE);
					}
					if (p->flag != FLAGDUMMY) {
						PutBitmapCenter(xpos, 4, &symbol_rect[3], BMPNOTE);
					}
				}

				p = p->to;
			}
		}
	}
}
//音符(ドラム)の表示
void OrgData::PutNotes2(int TPCY, bool vol)
{
	int i,j,k,t,addY;
	int n; //2010.09.23 A
	NOTELIST *p;
	long xpos,ypos;
	long scr_h,scr_v;
	unsigned char line,dot;
	MUSICINFO mi;

	GetMusicInfo(&mi);line = mi.line;dot = mi.dot;
	scr_data.GetScrollPosition(&scr_h,&scr_v);


	if (!vol) {
		for (n = MAXMELODY; n < MAXTRACK; n++) {
			k = getRecentTrack(n - MAXMELODY, 1);
			if (k == track)continue;
			if ((p = info.tdata[k].note_list) == NULL) {

				continue;
			}
			while (p != NULL && p->to != NULL) {
				xpos = (p->x - scr_h) * NoteWidth + KEYWIDTH;
				if (xpos >= KEYWIDTH)break;
				p = p->to;
			}

			if (p == NULL)continue;
			if (p->from != NULL)p = p->from;
			while (p->from != NULL && p->y == KEYDUMMY)p = p->from;

			while (p != NULL) {
				ypos = (95 - p->y - scr_v) * 12;
				xpos = (p->x - scr_h) * NoteWidth + KEYWIDTH;
				t = (p->y % 12); if (t == 1 || t == 3 || t == 6 || t == 8 || t == 10)t = 1;else t = 0; //I think this releates to whether the note is on white or black.
				if (xpos > WWidth)break;

				if (ypos >= 0 && ypos < WHeight + 286 - WHNM) {
					addY = 0;

					for (j = 0, i = p->length - 1; i > 0; i--, j++) {
						if (TPCY == 0) PutBitmap2(xpos + j * NoteWidth + NoteWidth, ypos + 3 + addY, &note_tail_rect[k], BMPNOTE);
						else PutBitmap2(xpos + j * NoteWidth + NoteWidth, ypos + 3 + addY, &rc_TCPY[2 + t], BMPNOTE);
					}
					if (TPCY == 0) PutBitmapHead(xpos, ypos + 2 + addY, &note_rect[k], BMPNOTE, p->length);
					else PutBitmapHead(xpos, ypos + 2 + addY, &rc_TCPY[0 + t], BMPNOTE, p->length);//音符
				}
				p = p->to;
			}
		}
	}
	//編集する音符を表示///////////
	if((p = info.tdata[track].note_list) == NULL || TPCY==1){
		//return;//音符が無ければ中止
	}else{
		while(p != NULL && p->to != NULL){//どこから表示するか
			xpos = (p->x - scr_h)*16 + KEYWIDTH;
			if(xpos >= KEYWIDTH)break;
			p = p->to;
		}
		//尻尾を表示する為に、一つ前の音符から・・・。
		if(p == NULL){
			//return;
		}else{
			if(p->from != NULL)p = p->from;
			while(p->from != NULL && p->y == KEYDUMMY)p = p->from;
			//音符ががなくなるか、X座標が表示領域を超えるまで表示。
			while(p != NULL){
				ypos = (95 - p->y - scr_v)*12;//下が0になる95が最大
				xpos = (p->x - scr_h)*NoteWidth + KEYWIDTH;
				if(xpos > WWidth)break;//表示領域を超えた。
				if(!vol && ypos >= 0 && ypos < WHeight+286-WHNM){//表示範囲YPOS
					//PutBitmap(xpos,ypos+2,&symbol_rect[0],BMPNOTE);//音符
					//PutBitmap2(xpos,ypos+2,&note_rect[track+16],BMPNOTE);//音符	// 2014.05.27 D
					for(j = 0,i = p->length-1; i > 0; i--,j++){//尻尾
						PutBitmap2(xpos+j*NoteWidth+NoteWidth,ypos+3,&note_tail_rect[track-MAXMELODY],BMPNOTE);
					}
					PutBitmapHead(xpos,ypos+2,&note_rect[track+MAXTRACK],BMPNOTE,p->length);
				}
				if (vol) {
					if (p->pan != PANDUMMY) {
						ypos = WHeight + 351 - WHNM - (p->pan * 5);//パン
						PutBitmapCenter(xpos, ypos, &symbol_rect[0], BMPNOTE);
					}
					if (p->volume != VOLDUMMY) {
						ypos = WHeight + 426 - WHNM - (p->volume / 4);//ボリューム
						PutBitmapCenter(xpos, ypos, &symbol_rect[0], BMPNOTE);
					}
					if (p->flag != FLAGDUMMY) {
						PutBitmapCenter(xpos, 4, &symbol_rect[3], BMPNOTE);
					}
				}
				p = p->to;
			}
		}
	}
}
//楽譜の表示
void OrgData::PutMusic(void)
{
	//if (gIsDrawing) return;
	//gIsDrawing = true;

	RECT brect;
	int j;
	int i; // A 2010.09.24
	long x;
	long hpos,vpos,vpos2;

	brect = {0, 0, WWidth, WHeight};
	PutRect(&brect, 0);

	scr_data.GetScrollPosition(&hpos,&vpos);
	vpos2=vpos;
	vpos = -(vpos%12)*12;
	//ここ以降に楽譜表示を記述
	x = (-(hpos % (info.dot * info.line)) * NoteWidth) + KEYWIDTH;

	for (j = 0; j < MAXMELODY; ++j) {
		for (int i = 0; i < (WWidth / NoteWidth) + (info.line * info.dot) + 1; ++i) { // 15
			if (i % (info.line * info.dot) == 0) {
				brect = { 64, 0, 64 + NoteWidth, 144 };
				PutBitmap(x + i * NoteWidth, j * 144 + vpos, &brect, BMPMUSIC);
			}
			else if (i % info.dot == 0) {
				brect = { 64 + 16, 0, (64 + 16) + NoteWidth, 144 };
				PutBitmap(x + i * NoteWidth, j * 144 + vpos, &brect, BMPMUSIC);
			}
			else {
				if (NoteWidth >= 8) {
					brect = { 64 + 32, 0, (64 + 32) + NoteWidth, 144 };
					PutBitmap(x + i * NoteWidth, j * 144 + vpos, &brect, BMPMUSIC);
				}
				else {
					brect = { 64 + 32 + 1, 0, (64 + 32 + 1) + NoteWidth, 144 };
					PutBitmap(x + i * NoteWidth, j * 144 + vpos, &brect, BMPMUSIC);
				}
			}
		}
	}
	for(j = 0; j < 96; j++){ // 2010.09.22 A
		if(iKeyPushDown[j]!=0){
			PutBitmap(0,  (95 - j - vpos2)*12, &rc_PushKB[j%12],BMPMUSIC);//鍵盤
		}
	}

	if(gDrawDouble==0){
		if(track < MAXMELODY)
			PutNotes(0, false);
		else PutNotes2(0, false);
	}else{
		if(track < MAXMELODY){
			PutNotes2(1, false);	//Dram Tracks are hidden
			PutNotes(0, false);		//Melody Tracks are shown
		}
		else{
			PutNotes(1, false);	//Melody Tracks are hidden
			PutNotes2(0, false);	//Dram Tracks are shown
		}
		
	}
	for(j = 0; j < MAXMELODY; j++) PutBitmap(0, j*144 + vpos, &msc_rect[0], BMPMUSIC);

	//キーボード鍵盤（鍵盤部分）
	for(j = 0; j < 96 ; j++){ // 2010.09.22 A
		if(iKeyPushDown[j]!=0){
			//A 2010.09.24
			for(i = 0; i < 8; i++){
				PutBitmap(0,  (95 - (j%12)- i*12 - vpos2)*12, &rc_PushKB3[j%12],BMPMUSIC);//オクターブ違い鍵盤
			}
			PutBitmap(0,  (95 - j - vpos2)*12, &rc_PushKB2[j%12],BMPMUSIC);//鍵盤
		}
	}

	PutNumber();
	PutRepeat();

	for (int i = 0; i < (WWidth / NoteWidth) + (info.line * info.dot) + 1; i++) { // 15
		if (i % (info.line * info.dot) == 0){
			brect = { 64, 0, 64 + NoteWidth, 160};
			PutBitmap(x + i * NoteWidth, WHeight + 288 - WHNM, &brect, BMPPAN); //New num
			brect = { 64, 160, 64 + NoteWidth, 174 };
			PutBitmap(x + i * NoteWidth,0, &brect, BMPPAN); //New num (FLAG)
		} else if (i % info.dot == 0) {
			brect = { 80, 0, 80 + NoteWidth, 160 };
			PutBitmap(x + i * NoteWidth, WHeight + 288 - WHNM, &brect, BMPPAN); //Beat
			brect = { 80, 160, 80 + NoteWidth, 174 };
			PutBitmap(x + i * NoteWidth,0, &brect, BMPPAN); //Beat (FLAG)
		} else {
			if (NoteWidth >= 8) {
				brect = { 64+32, 0, (64+32) + NoteWidth, 160 };
				PutBitmap(x + i * NoteWidth, WHeight + 288 - WHNM, &brect, BMPPAN); //Step
				brect = { 64+32, 160, (64+32) + NoteWidth, 174 };
				PutBitmap(x + i * NoteWidth,0, &brect, BMPPAN); //Step (FLAG)
			} else {
				brect = { 97, 0, 97 + NoteWidth, 160 };
				PutBitmap(x + i * NoteWidth, WHeight + 288 - WHNM, &brect, BMPPAN); //No idea
				brect = { 64 + 32 + 1, 160, (64 + 32 + 1) + NoteWidth, 174 };
				PutBitmap(x + i * NoteWidth,0, &brect, BMPPAN); //No idea
			}
		}
	}

	//PutPanParts(x);//パンライン

	if (gDrawDouble == 0) {
		if (track < MAXMELODY)PutNotes(0, true);
		else PutNotes2(0, true);
	}
	else {
		if (track < MAXMELODY) {
			PutNotes2(1, true);	//半透明表示
			PutNotes(0, true);		//実体表示
		}
		else {
			PutNotes(1, true);	//半透明表示
			PutNotes2(0, true);	//実体表示
		}

	}

	PutSelectArea();

	PutBitmap(0,WHeight+288-WHNM,&msc_rect[0],BMPPAN);//パン
	PutBitmap(0, 0, &msc_rect[1], BMPPAN); //FLAG

	if(sACrnt){ //2014.04.30
		PutBitmap(0,WHeight+288-WHNM+144,&msc_rect[2],BMPNOTE);//範囲選択は常にｶﾚﾝﾄﾄﾗｯｸのとき
	}
	if(iActivatePAN){ //2014.05.01
		PutBitmap(0,WHeight+288-WHNM,&msc_rect[3], BMPNOTE);
	}
	if(iActivateVOL){ //2014.05.01
		PutBitmap(0,WHeight+288-WHNM+72,&msc_rect[4], BMPNOTE);
	}

	if (timer_sw) {
		long playPos;
		char str[12];
		char oldstr[12];

		org_data.GetPlayPos(NULL, &playPos);

		itoa(playPos / (info.dot * info.line), str, 10);
		GetDlgItemText(hDlgPlayer, IDE_VIEWMEAS, oldstr, 10);
		if (strcmp(str, oldstr) != 0) SetDlgItemText(hDlgPlayer, IDE_VIEWMEAS, str);

		itoa(playPos % (info.dot * info.line), str, 10);
		GetDlgItemText(hDlgPlayer, IDE_VIEWXPOS, oldstr, 10);
		if (strcmp(str, oldstr) != 0) SetDlgItemText(hDlgPlayer, IDE_VIEWXPOS, str);

		scr_data.SetHorzScroll(sSmoothScroll ? playPos : ((playPos / (info.dot * info.line)) * (info.dot * info.line)));
	}
	//gIsDrawing = false;
}

//選択範囲の表示
void OrgData::PutSelectArea()
{
	if(tra<0)return; //選択されてへん
	long ypos;
	long scr_h,scr_v;
	unsigned char line,dot;
	MUSICINFO mi;
	//必要なデータを取得
	GetMusicInfo(&mi);line = mi.line;dot = mi.dot;
	scr_data.GetScrollPosition(&scr_h,&scr_v);

	int xSelS, xSelE,xx,t;
	t = 0; if(ful==0 && tra!=track)t=3;
	xSelS = (nc_Select.x1_1 - scr_h)*NoteWidth + KEYWIDTH; //選択開始点
	xSelE = (nc_Select.x1_2 - scr_h)*NoteWidth + KEYWIDTH; //選択終了点
	ypos = WHeight - 13;
	for(xx=KEYWIDTH;xx<=WWidth+NoteWidth;xx+=NoteWidth){
		if(xx==xSelS){
			if(xx==xSelE)PutBitmapCenter16(xx,ypos,&rc_SelArea[7],BMPNOTE);
			else PutBitmapCenter16(xx,ypos,&rc_SelArea[0+t],BMPNOTE);
		}else if(xx>xSelS && xx<xSelE){
			if(((xx-KEYWIDTH)/NoteWidth + scr_h)%4==0 && ful==1 ){
				PutBitmapCenter16(xx,ypos,&rc_SelArea[6],BMPNOTE); //FULL
			}else{
				PutBitmapCenter16(xx,ypos,&rc_SelArea[1+t],BMPNOTE);
			}
		}else if(xx==xSelE){
			PutBitmapCenter16(xx,ypos,&rc_SelArea[2+t],BMPNOTE);
		}
	}


}

void OrgData::RedrawSelectArea()
{
	//long x;
	//long hpos, vpos;
	//scr_data.GetScrollPosition(&hpos, &vpos);
	//x = (-(hpos % (info.dot * info.line)) * NoteWidth) + KEYWIDTH;
	//PutSelectParts(x);//パンライン
	//PutSelectArea();

}