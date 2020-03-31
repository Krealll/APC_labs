#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#define delim 3
#define delTime 5


unsigned int keyBoard[] = {
	65,69,73,78,82,87,92,98,104,110,117,124,
	131,139,147,156,165,175,185,196,208,220,233,247,
	262,277,294,311,329,349,370,392,415,440,466,493,1
};

unsigned int chooseKey(char a){
	unsigned int value;
	switch (a)
	{
	case '1':
	value=keyBoard[0];
		break;
	case '2':
	value=keyBoard[1];
		break;	
	case '3':
	value=keyBoard[2];
		break;		
	case '4':
	value=keyBoard[3];
		break;
	case '5':
	value=keyBoard[4];
		break;
	case '6':
	value=keyBoard[5];
		break;	
	case '7':
	value=keyBoard[6];
		break;
	case '8':
	value=keyBoard[7];
		break;	
	case '9':
	value=keyBoard[8];
		break;	
	case '0':
	value=keyBoard[9];
		break;
	case '-':
	value=keyBoard[10];
		break;
	case '=':
	value=keyBoard[11];
		break;	
	case 'q':
	value=keyBoard[12];
		break;
	case 'w':
	value=keyBoard[13];
		break;	
	case 'e':
	value=keyBoard[14];
		break;		
	case 'r':
	value=keyBoard[15];
		break;
	case 't':
	value=keyBoard[16];
		break;
	case 'y':
	value=keyBoard[17];
		break;	
	case 'u':
	value=keyBoard[18];
		break;
	case 'i':
	value=keyBoard[19];
		break;	
	case 'o':
	value=keyBoard[20];
		break;	
	case 'p':
	value=keyBoard[21];
		break;
	case '[':
	value=keyBoard[22];
		break;
	case ']':
	value=keyBoard[23];
		break;	
	case 'a':
	value=keyBoard[24];
		break;
	case 's':
	value=keyBoard[25];
		break;	
	case 'd':
	value=keyBoard[26];
		break;		
	case 'f':
	value=keyBoard[27];
		break;
	case 'g':
	value=keyBoard[28];
		break;
	case 'h':
	value=keyBoard[29];
		break;	
	case 'j':
	value=keyBoard[30];
		break;
	case 'k':
	value=keyBoard[31];
		break;	
	case 'l':
	value=keyBoard[32];
		break;	
	case ';':
	value=keyBoard[33];
		break;
	case '\'':
	value=keyBoard[34];
		break;
	case '\r':
	value=keyBoard[35];
		break;	
	case '`':
	value=keyBoard[36];
		break;	
	case 'c':
	value=-1;
		break;	
	case 'm':
	value=0;
		break;				
	}
	return value;
}

unsigned int MorroWind[] = {
	242, 272, 288, delim, 
	288, 323, 362, delim,
	362, 431, 323,
	288, 272, 242, delim,

	242, 272, 288, delim, 
	288, 323, 362, delim,
	362, 431, 484,
	431, 543, 484, delim,

	484, 543, 575,
	543, 484, 431,
	384, 362, 323,
	288, 362, 323,
	288, 272, 242,
	1

};

unsigned int MorrDelay[] = {
	250, 250, 700,delTime,
	270, 270, 800,delTime,
	 280, 270, 750,
	350, 350, 1000,delTime,

	250, 250, 700,delTime,
	 270, 270, 800 ,delTime,
	280, 280, 700, 
	500, 350, 800,delTime,

	 500, 450, 700,
	600, 625, 650, 
	675, 725, 1000,
	 300, 350, 800,
	600, 500, 3000,

};

unsigned int lab_notes[] = {
	196, 261, 329,
	196, 261, 329,
	196, 261, 329,
	196, 261, 329,delim,

	329, 261, 196,
	329, 261, 196,
	329, 261, 196,delim,

	196, 261, 329,
	110, 1

};

unsigned int ndelay[] = {
	400, 200, 200,
	400, 200, 200,
	400, 200, 200,
	400, 200, 200,delTime,

	400, 200, 200, 
	400, 200, 200, 
	400, 200, 200, delTime,

	300, 200, 100,  
	200
};


void Menu(void) {
	printf("\n1. Play sound");
	printf("\n2. Print channels kd");
	printf("\n3. Print state words");
	printf("\n4. Get random number");
	printf("\n5. Exit.");
	printf("\n\nEnter choice: ");
	return;
}

void MenuChoice(int *number) {
	for (; ; ) {
		while (scanf("%d", number) != 1) {
			printf("\nIncorrect input. Enter only digits: ");
			while (getchar() != '\n');
			continue;
		}
		if (*number != 1 && *number != 2 && *number != 3
			&& *number != 4 && *number != 5) {
			printf("\nIncorrect input. Choose one of points in menu: ");
			continue;
		}
		if (getchar() != '\n') {
			printf("\nIncorrect input. Enter only digits: ");
			while (getchar() != '\n');
			continue;
		}
		else {
			break;
		}
	}
	return;
}

void RandomBoardEnter(long *number) {
	printf("\nEnter random boarder: ");
	for (; ; ) {
		while (scanf("%ld", number) != 1) {
			printf("Incorrect input. Enter only digits: ");
			while (getchar() != '\n');
			continue;
		}
		if (*number <= 0 || *number >= 65535) {
			printf("Incorrect input. Enter right value (less then 65535): ");
			continue;
		}
		if (getchar() != '\n') {
			printf("Incorrect input. Enter only digits: ");
			while (getchar() != '\n');
			continue;
		}
		else {
			break;
		}
	}
	return;
}

void TurnSpeaker(int isActive) {
	if (isActive) {
		outp(0x61, inp(0x61) | 3);
		return;
	}
	else {
		outp(0x61, inp(0x61) & 0xFC);
		return;
	}
}

void SetSoundFreq(unsigned int freq) {
	long base = 1193180; //частоты генератора сигналов таймера
	long kd;
	outp(0x43, 0x0B6);//10110110 канал 10, операция 11, режим 011, формат 0
	kd = base / freq;
	outp(0x42, kd % 256);
	kd /= 256;
	outp(0x42, kd);
	return;
}

void getN(){
	int i;
	system("cls");
	i=56;
	printf("%s","\nc - decrease delay          m - increase delay\n");
	
	printf("%c",'+');
	while (i)
	{
		printf("%c",'=');
		i--;
	}
	printf("%s","+\n|   1   2   3   4   5   6   7    8    9   0   -    =    e|");
	printf("%s","\n|  Do Dod  Re Red Mee  Fa Fad  Sol Sold  La Lad  See quit| Big octave");
	printf("%s","\n|   q   w   e   r   t   y   u    i    o   p   [    ]     |");
	printf("%s","\n|  Do Dod  Re Red Mee  Fa Fad  Sol Sold  La Lad  See     | Small octave");
	printf("%s","\n|   a   s   d   f   g   h   j    k    l   ;   '  Enter   |");
	printf("%s","\n|  Do Dod  Re Red Mee  Fa Fad  Sol Sold  La Lad  See     | First octave\n");
	i=56;
	printf("%c",'+');
	while (i)
	{
		printf("%c",'=');
		i--;
	}
	printf("%s","+\nand other notes\n");
	return;
}

void PlaySound() {
	int i;
	int c;
	char b;
	printf ("1 - lab melody\n2 - Morrowind's Theme\n3 - User keyboard\n");
	scanf ("%d", &c);
	if (c==1)
	{
		for (i = 0; lab_notes[i] != 1; i++)
		{
		SetSoundFreq(lab_notes[i]);
		TurnSpeaker(1);
		delay(ndelay[i]);
		TurnSpeaker(0);
		}
	}

	if (c==2)
	{
		for (i = 0; MorroWind[i] != 1; i++)
		{
		SetSoundFreq(MorroWind[i]);
		TurnSpeaker(1);
		delay(MorrDelay[i]);
		TurnSpeaker(0);
		}
	}
	if (c==3){
		c=150; // delay
		getN();
		while (1)
		{
		for (; ; ) {
			b=getch();
			if ((((b)>='0')&&((b)<='9'))
			||((b>='a'&&b<='z')
			&&(b!='z')&&(b!='x')&&(b!='v')&&(b!='b')&&(b!='n'))
			||((b)=='-')||((b)=='=')
			||(b=='[')||((b)==']')
			||((b)==';')||((b)=='\'')||((b)=='\r')
			||((b)=='`')) {
				break;
			}
			else
			{
				printf("\nIncorrect input\n");
				delay(100);
				system("cls");
				getN();
				continue;
			}
		}
		i=chooseKey(b);
		if (i==1){           // if ` is pushed - exit
			break;
		}
		else if(i==0){
			c+=5; 			//increase delay if m is pushed
		}
		else if (i==-1){
			c-=5;			// decrease delay if c is pushed
			if (c<0){
				c=5;
			}
		}
		else
		{
			SetSoundFreq(i);
			TurnSpeaker(1);
			delay(c);
			TurnSpeaker(0);
		}
		}
	}
}

void CharToBin(unsigned char state, char *str) {
	int i, j;
	char temp;
	for (i = 7; i >= 0; i--) {
		temp = state % 2;
		state /= 2;
		str[i] = temp + '0';
	}
	str[8] = '\0';
}

void PrintKd() {
	int iChannel;
	long j;
	long kd_low, kd_high, kd, kd_max;

	for (iChannel = 0; iChannel < 3; iChannel++) {
		kd_max = 0;
		for (j = 0; j < 65535; j++) {
			switch (iChannel) {
			case 0: {
				outp(0x43, 0x0);
				kd_low = inp(0x40);
				kd_high = inp(0x40);
				kd = kd_high * 256 + kd_low;
				break;
			}
			case 1: {
				outp(0x43, 0x40);
				kd_low = inp(0x41);
				kd_high = inp(0x41);
				kd = kd_high * 256 + kd_low;
				break;
			}
			case 2: {
				outp(0x43, 0x80);
				kd_low = inp(0x42);
				kd_high = inp(0x42);
				kd = kd_high * 256 + kd_low;
				break;
			}
			}
			if (kd_max < kd) {
				kd_max = kd;
			}
		}
		switch (iChannel) {
		case 0: {
			printf("Channel 0x40 kd: %ld\n", kd_max);
			break;
		}
		case 1: {
			printf("Channel 0x41 kd: %ld\n", kd_max);
			break;
		}
		case 2: {
			printf("Channel 0x42 kd: %ld\n", kd_max);
			break;
		}
		}
	}
	return;
}

void StateWords() {
	char *bin_state;
	int iChannel;
	unsigned char state;
	bin_state = (char *)calloc(9, sizeof(char));
	if (bin_state == NULL) {
		printf("Memory allocation error");
		exit(EXIT_FAILURE);
	}

	for (iChannel = 0; iChannel < 3; iChannel++) {
		switch (iChannel) {
		case 0: {
			outp(0x43, 0xE2); 	//11(SC) 1(COUNT) 0(STATUS) 001(CN T) 0  0x40
			state = inp(0x40);
										// _(O - output - current value) 
										// _(NC - NullCount - 0,
										//	when allowed to count) 
										// _._ _._._ _(RW , M, BCD)
			CharToBin(state, bin_state);										
			printf("Channel 0x40 word: %s\n", bin_state);
			break;
		}
		case 1: {
			bin_state[0] = '\0';
			outp(0x43, 0xE4); 	//11100100  0x41
			state = inp(0x41);
			CharToBin(state, bin_state);
			printf("Channel 0x41 word: %s\n", bin_state);
			break;
		}
		case 2: {
			bin_state[0] = '\0';
			outp(0x43, 0xE8); 	//11101000  0x42
			state = inp(0x42);
			CharToBin(state, bin_state);
			printf("Channel 0x42 word: %s\n", bin_state);
			break;
		}
		}
	}
	free(bin_state);
	return;
}

long RandomDigit() {
	long bl, bh;
	outp(0x43, 0x6);//4-5    00    6-7 10 2 порт
	bl = inp(0x42);
	bh = inp(0x42);
	bh *= 256;
	bh += bl;
	outp(0x61, inp(0x61) & 0xFC);
	return bh;
}

void RandTimerSet(long max) {
	outp(0x43, 0x34);//00110100
	outp(0x42, max % 256);
	max /= 256;
	outp(0x42, max);
	outp(0x61, inp(0x61) | 1);
	return;
}


int main() {
	int check_exit = 0;
	system("cls");
	Menu();
	MenuChoice(&check_exit);
	while (check_exit != 5) {
		switch (check_exit) {
		case 1: {
			PlaySound();
			break;
		}
		case 2: {
			PrintKd();
			break;
		}
		case 3: {
			StateWords();
			break;
		}
		case 4: {
			long digit;
			RandomBoardEnter(&digit);
			RandTimerSet(digit);
			printf("Press any key to get number.\n");

			getch();
			digit = RandomDigit();
			printf("\nRandomed number: %ld\n", digit);
			break;
		}
		}
		check_exit = 0;
		system("pause");
		system("cls");
		Menu();
		MenuChoice(&check_exit);
	}
	return 0;
}
