#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct color{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};


void printVal(const unsigned int number, struct color field)
{
	printf( "%u\n%u\n%u\n%u\n", number, field.red, field.blue, field.green);
}

int main( int argc, char** argv)
{
	struct color field;
	unsigned int x, y, size;

	if( argc != 3 )
	{
		printf("Usage: %s xfields yfields\n", argv[0]);
		return 1;
	}

	x = strtol(argv[1], NULL, 10);
	y = strtol(argv[2], NULL, 10);
	size=x*y;
	
	while (1) 
	{
		field.red   = (unsigned char)rand();
		field.green = (unsigned char)rand();
		field.blue  = (unsigned char)rand();
		printVal( rand()%size, field);
		
		usleep(1000);
	}

	return 0;
}
