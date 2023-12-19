#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int samples = 500000;

int main()
{

	srand(time(NULL));

	int hist[25] = {0};
	int counter = 0;
	int random;

	clock_t start;
	start = clock();

	for (int i = 0; i < samples; i++)
	{
		counter = 0;
		for (int j = 0; j < 12; j++)
		{
			random = rand() % 101;
			if (random >= 49)
			{
				counter++;
			}
			else
			{
				counter--;
			}
		}
		hist[counter + 12]++;
	}

	clock_t end;
	end = clock();

	printf("%.6f", (double)(end - start)/CLOCKS_PER_SEC);

	return 0;
}

