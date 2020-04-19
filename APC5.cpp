#include <dos.h>
#include <stdio.h>
#include <iostream.h>
#include <conio.h>
#include <io.h>

//************************************************************************
// This code is supposed  to be launched on MS DOS virtual machine with downgraded CPU performance
//************************************************************************

struct VIDEO
{
	unsigned char symb;
	unsigned char attr;
};
VIDEO  far* screen  = (VIDEO far*)MK_FP(0xB800,0);
int msCounter = 0;

int flag =0;
int flagMin=0;
int flagHour=0;
int flagDay=0;
int flagNoPause=1;
int flagStopSW=0;
static int h=0;
static int m=0;
int s;
int oldValue=100;

void interrupt far (*oldInt70h)(...);		// OLD RTC handler
void interrupt far (*oldInt4Ah)(...);		// old alarm handler
void interrupt far newInt4Ahandler(...);	// new rtc handler
void interrupt far newInt70handler(...);	// new alarm handler

void freeze(void);
void unfreeze(void);
void _delay(unsigned long delayPeriod);
int BCDToInteger(int bcd);
unsigned char IntToBCD(int value);
void getTime(void);
void setTimeManually(void);
void printch(unsigned char c);


void delay(void);
void wait(void);
void enableAlarm(void);
void disableAlarm(void);
void stopwatch(void);
void getTimeStopwatch(void);
void printAlarm(void);

void menu(){
	printf("\n1. Show time\n2. Set time\n3. Delay time\n4. Enable alarm\n5. Disable alarm\n6. Stopwatch\n7. Refresh screen\n0. Exit\n\n");
}

void main() {
	char c;
	clrscr();
	menu();
	while (c != 0) {
		c = getch();
		switch (c) {
			case '1': getTime(); break;
			case '2': setTimeManually(); break;
			case '3': delay(); break;
			case '4': enableAlarm(); break;
			case '5': disableAlarm(); break;
			case '6': stopwatch();clrscr();menu(); break;
			case '7': clrscr();menu(); break;
			case '8': freeze(); break;
			case '9': unfreeze(); break;
			case '0': return;
		}
	}
}

void wait(void) {
	do {
		outp(0x70, 0x0A);
	} while (inp(0x71) & 0x80);			// 0x80 = 10000000 while 7-th bit - 1, rtc is busy
}

void stpwtchmenu(){
	printf("\ns - Start\nt - Stop\np - Pause(unpause)\ne - exit stopwatch\n");
}

void getValues(){
	int value;

	wait();
	outp(0x70, 0x00);					// sec
	value = inp(0x71); 
	s=BCDToInteger(value);
	m=0;
	h=0;
}

void stopwatch(void){
	char a;
	flagNoPause=1;
	flagStopSW=1;
	flag=0;
	flagMin=0;
	flagHour=0;
	flagDay=0;
	s=0;
	m=0;
	h=0;
	clrscr();
	stpwtchmenu();
	a=getch();
	while(a!='e'){
		switch(a){
			case 's':
			if(!flag){
				getValues();
				flag=1;
				flagMin=0;
				flagHour=0;
				flagDay=0;
				flagNoPause=0;
				flagStopSW=0;
			}
			break;
			case 't':
			if(flag){
				flag=0;
				flagNoPause=1;
				flagStopSW=1;
				printf("\nFinish!");
			}
			break;
			case 'p':
			if(!flagNoPause){
				if(flag){
					flagStopSW=1;
					flag=0;
				}
				else{
					flagStopSW=0;
					flag=1;
				}
			}
			break;
		}
	
		do{
			getTimeStopwatch();
			if (kbhit()){
				a=getch();
				break;
			}
		}while(1);
	}
}

void getTimeStopwatch(void){

	int value;
	int temp;
	wait();
	outp(0x70, 0x00);					// sec
	value = inp(0x71);
	temp=BCDToInteger(value)-s;
	if(temp<0)
		temp+=60;
	if(oldValue!=temp){
			if(!flagStopSW){
			oldValue=temp;
			clrscr();
			stpwtchmenu();

			printf("\nTime - %d:", h);

			printf("%d:", m);

			printf("%d", temp); 

			if(temp==0)
			{
				if(flagMin){
				flagMin=0;
				m++;
				}
			}	
			else
				flagMin=1;

			if(m==60)
			{
				if(flagHour){
				flagHour=0;
				h++;
				m=0;
				}
			}	
			else
				flagHour=1;

			if(h==60)
			{
				if(flagDay){
				flagDay=0;
				h=0;
				}
			}	
			else
				flagDay=1;
		}
		else {
			s++;
			if(s==60)
			s=0;
		}
	}
}

void getTime(void) {
	int value;

	wait();
	outp(0x70, 0x04);					// hour
	value = inp(0x71); 
	printf("%d:", BCDToInteger(value)); 
	
	wait();
	outp(0x70, 0x02);					// min
	value = inp(0x71); 
	printf("%d:", BCDToInteger(value)); 

	wait();
	outp(0x70, 0x00);					// sec
	value = inp(0x71); 
	printf("%d   ", BCDToInteger(value)); 
	
	wait();
	outp(0x70, 0x07);					// day
	value = inp(0x71); 
	printf("%d.", BCDToInteger(value)); 
	
	wait();
	outp(0x70, 0x08);					// month
	value = inp(0x71); 
	printf("%d.", BCDToInteger(value)); 
	
	wait();
	outp(0x70, 0x09);					// year
	value = inp(0x71); 
	printf("%d   ", BCDToInteger(value)); 
	
	wait();
	outp(0x70, 0x06);					// weekday
	value = inp(0x71);
	switch (BCDToInteger(value)) {
		case 2: printf("Monday\n"); break;
		case 3: printf("Tuesday\n"); break;
		case 4: printf("Wednesday\n"); break;
		case 5: printf("Thursday\n"); break;
		case 6: printf("Friday\n"); break;
		case 7: printf("Saturday\n"); break;
		case 1: printf("Sunday\n"); break;
		default: printf("Day of week is not set\n"); break;
	}
}


void setTimeManually(void) {
	int value;
	freeze();							// disable rtc update

	do {
		printf("Enter hour: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 23 || value < 0);
	outp(0x70, 0x04);
	outp(0x71, IntToBCD(value));		
 
	do {
		printf("Enter minute: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 59 || value < 0);
	outp(0x70, 0x02);
	outp(0x71, IntToBCD(value));

	do {
		printf("Enter second: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 59 || value < 0);
	outp(0x70, 0x00);
	outp(0x71, IntToBCD(value));
 
	do {
		printf("Enter week day number: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 7 || value < 1);
	outp(0x70, 0x06);
	outp(0x71, IntToBCD(value));

	do {
		printf("Enter day of month: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 31 || value < 1);
	outp(0x70, 0x07);
	outp(0x71, IntToBCD(value));

	do {
		printf("Enter mounth: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 12 || value < 1);
	outp(0x70, 0x08);
	outp(0x71, IntToBCD(value));

	do {
		printf("Enter year: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 99 || value < 0);
	outp(0x70, 0x09);
	outp(0x71, IntToBCD(value));

	unfreeze();							// enable rtc update
}

void enableAlarm(void) {
	int value, second, minute, hour;

	do {
		printf("Enter second: ");
		fflush(stdin);
		scanf("%d", &second);
	} while (second > 59 || second < 0);

	do{
		printf("Enter minute: ");
		fflush(stdin);
		scanf("%d", &minute);
	} while (minute > 59 || minute < 0);

	do {
		printf("Enter hour: ");
		fflush(stdin);
		scanf("%d", &hour);
	} while (hour > 23 || hour < 0);

	outp(0x70, 0x01);
	outp(0x71, IntToBCD(second));

	outp(0x70, 0x03);
	outp(0x71, IntToBCD(minute));

	outp(0x70, 0x05);
	outp(0x71, IntToBCD(hour));

	_disable();
	oldInt4Ah = getvect(0x4A);
	setvect(0x4A, newInt4Ahandler);
	_enable(); 	

	outp(0x70, 0xB);
	outp(0x71, inp(0x71) | 0x20);


	outp(0xA1, inp(0xA1) & 0xFE);		
	
	// 0xFE = 11111110, ICW1
}

void disableAlarm() {
	if (oldInt4Ah == NULL) {
		printf("Alarm is already disable or not set.\n");
		return;
	}
	_disable();
	setvect(0x4A, oldInt4Ah);
	outp(0xA1, (inp(0xA0) | 0x01));
	outp(0x70, 0x0B);
	outp(0xA1, inp(0x71) & 0xDF);		// 0xDF = 11011111, bit 5 to 0 to disable alarm 
	_enable();

}
// rtc update disabling
void freeze(void) {
	unsigned char value;
	wait();								// until rtc are busy
	outp(0x70, 0x0B);
	value = inp(0x71);					// get B register
	value |= 0x80;						// change bit 7 to 1 to disable rtc update
	outp(0x71, value);					// write new B register value
}
// rtc update enabling
void unfreeze(void) {
	unsigned char value;
	wait();								// until rtc are busy
	outp(0x70, 0x0B);
	value = inp(0x71);					// get B register
	value -= 0x80;						// change bit 7 to  to allow rtc update
	outp(0x70, 0x0B);
	outp(0x71, value);					// write new B register value
}

void interrupt far newInt70handler(...) {
	msCounter++;					
	oldInt70h();
}

void interrupt far newInt4Ahandler(...) {
		
	printAlarm();
	oldInt4Ah();
}

void printch(unsigned char c){

		screen->symb= c;
		screen->attr=0x5E;
		screen++;
}

void printAlarm(){

		screen->symb= 'A';
		screen->attr=0x5E;
		screen++;

		screen->symb= 'L';
		screen->attr=0x5E;
		screen++;

		screen->symb= 'A';
		screen->attr=0x5E;
		screen++;

		screen->symb= 'R';
		screen->attr=0x5E;
		screen++;

		screen->symb= 'M';
		screen->attr=0x5E;
		screen++;

		screen->symb= '!';
		screen->attr=0x5E;
		screen++;

		screen->symb= '!';
		screen->attr=0x5E;
		screen++;

		screen->symb= '!';
		screen->attr=0x5E;
		screen++;
}

void _delay(unsigned long delayPeriod) {

	_disable();							//restrict interrupts
	oldInt70h = getvect(0x70);
	setvect(0x70, newInt70handler);
	_enable();							// allow interrupts

	outp(0xA1, inp(0xA1) & 0xFE);			// 0xFE = 11111110(ICW1)

	// enable periodic interrupts
	outp(0x70, 0x0B);					
	outp(0x71, inp(0x71) | 0x40);			// 0x40 = 01000000, set bit 6 to 1 to enable

	msCounter = 0;
	while (msCounter != delayPeriod);	// ms counting
	_disable();
	setvect(0x70, oldInt70h);			// get previous handler
	_enable();
}


void delay(void) {
	unsigned long delayPeriod;
	unsigned char value;

	do {
		printf("Enter delay time in milliseconds: ");
		fflush(stdin);
		scanf("%ld", &delayPeriod);
	} while (delayPeriod < 1);
	printf("Time before delay: ");
	getTime();
	printf("Delaying ...\n");

	_disable();							//restrict interrupts
	oldInt70h = getvect(0x70);
	setvect(0x70, newInt70handler);
	_enable();							// allow interrupts

	// enable IRQ8 CMOS 
	outp(0xA1, inp(0xA1) & 0xFE);			// 0xFE = 11111110(ICW1)

	// enable periodic interrupts
	outp(0x70, 0x0B);					
	outp(0x71, inp(0x71) | 0x40);			// 0x40 = 01000000, set bit 6 to 1 to enable

	msCounter = 0;
	while (msCounter != delayPeriod);	// ms counting
	printf("End delay of %d ms\n", msCounter);
	_disable();
	setvect(0x70, oldInt70h);			// get previous handler
	_enable();
	printf("Time after delay: ");
	getTime();
}

int BCDToInteger(int bcd) {
	return bcd % 16 + bcd / 16 * 10;
}

unsigned char IntToBCD(int value) {
	return (unsigned char)((value / 10) << 4) | (value % 10);
}
