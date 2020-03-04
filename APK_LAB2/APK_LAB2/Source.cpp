#include "windows.h"
#include <stdio.h>
#include <time.h>

#define ArrSize 64
#define newSize ArrSize/4

void show(short* arr)
{
	int i = 0;
	for (; i < ArrSize; i++)
		printf("%3d", arr[i]);
	printf("\n");
}

void setNull(short* arr)
{
	int i = 0;
	for (; i < ArrSize; i++)
		arr[i] = 0;
}

int main(int)
{
	srand(time(NULL));
	int i;
	long  N = 100000000000,temp;
	short arr1[ArrSize],
		arr2[ArrSize],
		arr3[ArrSize];
	for (i = 0; i < ArrSize; i++)
	{
		arr1[i] = rand() % 99;
		arr2[i] = rand() % 99;
	}
	show(arr1);
	show(arr2);

	clock_t start, end;
	i = 0;
	temp = N;
	start = clock();
	while (temp--)
	{
		while (i < ArrSize)
		{
			if (arr1[i] > arr2[i])
				arr3[i] = arr1[i];
			else 
				arr3[i] = arr2[i];
			i++;
		}
	}
	end = clock();
	printf("Time: %lf\n", (double)(end-start)/CLOCKS_PER_SEC);
	
	show(arr3);

	start = clock();
	_asm {
		mov edx,N
	Cycle:
		xor esi,esi
		mov ecx,ArrSize		
	Cycle2:
		mov ax,arr1[esi]
		mov bx,arr2[esi]
		cmp ax,bx
		ja BIGax

		mov arr3[esi],ax
		jmp endCycle
	BIGax:
		mov arr3[esi],bx
	endCycle:
		inc esi
		inc esi
		dec ecx
		cmp ecx, 0
		jne Cycle2

		dec edx
		cmp edx,0
		jne Cycle
	}
	end = clock();
	printf("ASM time: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);
	show(arr3);

	for (i = 0; i < ArrSize; i++)
		arr3[i] = 0;

	start = clock();
	_asm {
		mov ecx, N
	Cycle_:
		xor esi, esi
		mov edx, newSize
	_cycle:
		movq MM0,arr1[esi]
		movq MM1, arr2[esi]
		
		pminub MM0,MM1
		movq arr3[esi],MM0
		add si,8
		dec edx
		cmp edx,0
		jne _cycle
					
		dec ecx
		cmp ecx,0
		jne Cycle_

		emms
	}
	end = clock();
	printf("ASM time: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);
	show(arr3);

	system("pause");
}



	