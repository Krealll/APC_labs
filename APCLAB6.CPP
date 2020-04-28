#include <dos.h>
#include <stdio.h>
#include <iostream.h>
#include <conio.h>
#include <io.h>

struct VIDEO
{
	unsigned char symb;
	unsigned char attr;
};

VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);

int temp;
int i;
int curVal=0x60;
int prevVal=0x60;
int flagStartComand;
char* array;

void interrupt far  (*oldInt9)(...);					// Указатель на обработчик прерывания
void interrupt far  newInt9handler(...);					// Функция обработки прерывания
int showReg(int reg);
void main() {

	unsigned far *fp;	

	clrscr();
	disable();
	oldInt9 = getvect(0x09);						// Сохраняем указатель на старый обработчик
	setvect(0x09, newInt9handler);						// Меняем его на новый  
	enable();

	FP_SEG(fp) = _psp;
	FP_OFF(fp) = 0x2c;
	_dos_freemem(*fp);
	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);

	return;
}

int scanNumtoInt(char* array){
	int n;
	 n = 0;
    for (i = 0; array[i] !='\0'; ++i){
		if(array[i]< 0x01 && array[i]> 0x0B){
			return -1;
		}
		if(array[i]==0x0B)
	 		n=n*10;
		else
	   		n = 10 * n + (array[i] - 1);
	}
	if(i==0){
		return -1;
	}
	return n ;
}

// Функция обработки прерывания
void interrupt far newInt9handler(...) {
	unsigned char value = 0;
	unsigned char tempArray;
	value = inp(0x60);						// Получаем значение из порта 60h

	if(value==0x1C&&flagStartComand){		//enter
		clrscr();
		if(array[0]==0x2E&&array[1]==0x23){ 
			if(array[2]==0x20){				// if chd was typed
				curVal=scanNumtoInt(array+3);
				if(curVal>-1&&curVal<4){
					prevVal=prevVal%32;
					curVal*=32;
					curVal+=prevVal;
					prevVal=curVal;
				}
				else{
					curVal=-1;
				}
			}
			else{							// if ch was typed
				curVal=scanNumtoInt(array+2);
				if(curVal>=0&&curVal<32){
					prevVal=prevVal/32;
					prevVal*=32;
					curVal+=prevVal;
					prevVal=curVal;
				}	
				else{
					curVal=-1;
				}
			}
		}
		else{
			curVal=-1;
		}
		for(i=0;i<temp;i++){
			array[i]='\0';
		}
		temp=0;
		flagStartComand=0;
		if(curVal>=0){
			curVal=showReg(curVal);
			while ((inp(0x64) & 0x02) != 0x00);
			outp(0x60, 0xF3);
			while ((inp(0x64) & 0x02) != 0x00);
			outp(0x60, curVal);
		}
		else{
			curVal=prevVal;
			screen+=10;
			screen->symb=0x3F;
			screen->attr=0x3F;
			screen-=10;	
		}
	}
	if(value==0x58){		//f12
		clrscr();
	}
	if(flagStartComand&&value<0x80){
		if(value==0x0E){
			array[temp]='\0';
			if(temp>0)
				temp--;
		}
		else{
			array[temp]=value;
			temp++;
			array[temp]='\0';
		}
	}
	if(value==0x0C){				// '-' minus symbol
		flagStartComand=1;
	}
	oldInt9();
}

int showReg(int reg){
	int val;
	val=reg;
	screen+=40;
	screen->symb=reg%2;
	reg/=2;
	screen->attr=0x3F;
	screen++;
	screen->symb=reg%2;
	reg/=2;
	screen->attr=0x3F;
	screen++;
	screen->symb=reg%2;
	reg/=2;
	screen->attr=0x3F;
	screen++;
	screen->symb=reg%2;
	reg/=2;
	screen->attr=0x3F;
	screen++;
	screen->symb=reg%2;
	reg/=2;
	screen->attr=0x3F;
	screen++;
	screen->symb=reg%2;
	reg/=2;
	screen->attr=0x1F;
	screen++;
	screen->symb=reg%2;
	reg/=2;
	screen->attr=0x1F;
	screen++;
	screen->symb=reg%2;
	reg/=2;
	screen->attr=0x1F;
	screen-=47;	
	return val;
}
