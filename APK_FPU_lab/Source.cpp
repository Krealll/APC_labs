#include <stdio.h>
#include <time.h>
#include<math.h>


//F(x) = (2*x + 1) / 3 + (x^2 + 1)
int main()
{
	double a = -1000, b = 1000, step = 0.0001, x = a, f = 0;
	int two = 2, three = 3;
	clock_t start, end;

	start = clock();
	_asm {
		fld b
		fld x
		loop_ :
		fcomi st(0), st(1)
			ja	 _end

			fld x
			fimul two
			fld1
			faddp st(1), st(0)
			fidiv three
			fld x
			fld x
			fmulp st(1), st(0)
			fld1
			faddp st(1), st(0)
			faddp st(1), st(0)

			fadd f
			fstp f

			fadd step
			fst x
			jmp loop_
			_end :
	}

	end = clock();
	printf("Assembler loop+ computing\nvalue:%f\ntime:%f\n\n", f, (double)(end - start) / CLK_TCK);

	x = a;
	f = 0;
	start = clock();
	while (x <= b)
	{
		f += (2 * x + 1) / 3 + (x*x + 1);
		x += step;
	};
	end = clock();
	printf("C while+computing\nvalue:%f\ntime:%f\n\n", f, (double)(end - start) / CLK_TCK);
	//F(x) = (2*x + 1) / 3 + (x^2 + 1)

	x = a;
	f = 0;
	start = clock();
	while (x <= b)
	{
		_asm
		{
			fld x
			fimul two
			fld1
			faddp st(1), st(0)
			fidiv three
			fld x
			fld x
			fmulp st(1), st(0)
			fld1
			faddp st(1), st(0)
			faddp st(1), st(0)

			fadd f
			fstp f

		}
		x += step;
	};
	end = clock();
	printf("C while + Assembler computing\nvalue:%f\ntime:%f\n\n", f, (double)(end - start) / CLK_TCK);
	getchar();
}