#include <stdlib.h>
#include <stdio.h>

int bufindexr;
int bufindexw;

int bufsize;
int *bufr;
int *bufw;

/* Program is just an exercise. Doesn't work well with
   reading/writing from/to multiple files. */

int putchar(int towrite, int writeto)	//write
{
	/* Write char to be written to write buffer. If the write buffer
	 is full, then perform the system call to actually write to file. */

	bufw[bufindexw++] = towrite;

	if (bufindexw == bufsize)
	{
		write(writeto, bufw, bufsize);
		bufindexw = 0;
	}

	return towrite;
}

int getchar(int readfrom)	//read
{
	/* If we have read to the end of the read buffer, then 
	 fill in the buffer again and read the next char. */

	if (bufindexr == bufsize)
	{
		read(readfrom, bufr, bufsize);
		bufindexr = 0;
	}
	 
	return bufr[bufindexr++];
}

int main()
{
	bufsize = 2048;
	bufindexr = bufsize;
	bufindexw = 0;
	bufr = bufw = (char*)malloc(sizeof(char)*bufsize);
}
