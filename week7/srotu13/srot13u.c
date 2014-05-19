#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

// This version converted to using system calls is finished.

//global variable to print comparisons to stderr per spec
int comparisons = 0;

int rot13cmp(const void *a, const void *b)
{
	comparisons++;
	const char *one = *(const char **)a;
	const char *two = *(const char **)b;
	while (1)
	{
	
		int anum = (int) *one;
		int bnum = (int) *two;
		//printf("chara = %c anum = %d, charb = %c bnum = %d\n",*one, anum,*two,bnum);
		
		//if character was in the alphabet, "decode" its ROT13 ASCII value
		if (anum == 10 && bnum == 10)//10 is the newline ASCII value
			return 0;
		else if (anum == 10)
			return -1;
		else if (bnum == 10)
			return 1;

		if ((anum > 64 && anum < 78) || (anum > 96 && anum < 110))
			anum += 13;
		else if ((anum > 77 && anum < 91) || (anum > 109 && anum < 123))
			anum -= 13;
		if ((bnum > 64 && bnum < 78) || (bnum > 96 && bnum < 110))
			bnum += 13;
		else if ((bnum > 77 && bnum < 91) || (bnum > 109 && bnum < 123))
			bnum -= 13;

		int diff = anum - bnum;
		if (diff != 0)
			return diff;
		one++;
		two++;
		//why doesn't recursion with rot13cmp(one++, two++) work?
	}
}


/* problem with /proc/self/maps is that it is detected as a regular file but its size is apparently 0*/
int main()
{
	//initialize starting variables and use fstat to determine filesize
	int count = 0;		//size of p array
	char readin;		//char used with read
	int bufsize = 2048;	//buffer size for realloc
	int notempty = 0;	//test if empty file passed in
	char *p;		//pointer to array of characters read in
	struct stat fileStats;
	fstat(0, &fileStats);

	if (S_ISREG(fileStats.st_mode))
	{
		//printf("stdin is a regular file\n----------\n");
		
		int size = fileStats.st_size;
		if (size == 0)
		{
			/* code in here handles empty files and supposedly empty files like /proc/self/maps
			   and is basically the same as homework 5 */
			notempty = read(0, &readin, 1);
			if (!notempty){
				fprintf(stderr, "Number of comparisons: %d\n", 0);
				return 0;
			}
			else
			{	
				p = (char*) malloc(sizeof(char) * bufsize);

				if (p == NULL){
					fprintf(stderr, "Error allocating dynamic memory.");
					exit(1);
				}

				/* Manually take care of first readin in this line because 
				   we used read once int the test condition above. */
				p[count++] = readin;

				while(read(0, &readin, 1) > 0)
				{
					p[count++] = readin;
					if (count == bufsize){
						p = (char*) realloc(p, bufsize * 2);
						if (p == NULL){
							fprintf(stderr, "Error with memory reallocation.");
							exit(1);
						}
						bufsize *= 2;
					}
				}
			}
		}
		else	//else regular file that has a positive size
		{
			p = (char*) malloc(sizeof(char) * fileStats.st_size);
			
			if (p == NULL){
				fprintf(stderr, "Error allocating dynamic memory.");
				exit(1);
			}

			for (int k = 0; k < size; k++)
			{
				//use system calls to read chars into p array
				read(0, &readin, 1);
				p[count++] = readin;
				/* mistake was using readin = read(0, &readin, 1). This assigns readin an int
				   value of how many bytes were read by read instead of the char */
			}

			//use fstat again to see if file has mutated size
			//if so, do realloc method until done
			fstat(0, &fileStats);
			if (fileStats.st_size != size)
			{
				//set the buffer size appropriately after we have already allocated size bytes
				bufsize = ((size/2048)+1) * 2048;
				while(read(0, &readin, 1) > 0)
				{
					p[count++] = readin;
					if (count == bufsize){
						p = (char*) realloc(p, bufsize * 2);
						if (p == NULL){
							fprintf(stderr, "Error with memory reallocation.");
							exit(1);
						}
						bufsize *= 2;
					}
				}
			}
		}
		
	}
	else //if file passed in isn't regular, revert to homework 5 methods
	{
		//printf("stdin is not a regular file\n--------\n");

		//read input and put into dynamically reallocated array
		p = (char*) malloc(sizeof(char) * bufsize);

		if (p == NULL){
			fprintf(stderr, "Error allocating dynamic memory.");
			exit(1);
		}
		
		while(read(0, &readin, 1) > 0)
		{
			/* previous mistake was using:
			   read(0, &readin, 1);
			   if ((int)readin == EOF)
			   	break;
			   The problem is that if read hits EOF,it doesn't read anything and returns 0
			   instead of reading EOF and putting it in readin. Therefore, readin can never
			   be EOF and we are stuck in an infinite loop. It's better to check if read
			   returned a positive integer indicating it read some bytes. */
			notempty = 1;
			p[count++] = readin;
			if (count == bufsize){
				p = (char*) realloc(p, bufsize * 2);
				if (p == NULL){
					fprintf(stderr, "Error with memory reallocation.");
					exit(1);
				}
				bufsize *= 2;
			}
		}
		if (!notempty){
			fprintf(stderr, "Number of comparisons: %d\n", 0);
			return 0;
		}
	}

	//append '\n' if there is no newline at the end
	if (p[count-1] != '\n')
		p[count++] = '\n';
	

	/* create an auxilliary array of char pointers pointing to the 'words' in p
	   with enough space corresponding to the number of newlines. lengths array
	   is for use later on to help with printing. */

	//you had you condition as p[k] != 0 before... you stupid
	int newlines = 0;
	for (int k = 0; k < count; k++)
		if (p[k] == '\n')
			newlines++;
	
	char **aux = (char**) malloc(sizeof(char*) * newlines);
	int *lengths = (int*) malloc(sizeof(int) * newlines);
	if (aux == NULL){
		fprintf(stderr, "Error allocating dynamic memory.");
		exit(1);
	}
	if (lengths == NULL){
		fprintf(stderr, "Error allocating dynamic memory.");
		exit(1);
	}

	//put char pointers to each word into aux by detecing the newlines in array p	
	int auxindex = 0;
	char *auxp = p;
	for (int k = 0; k < newlines; k++)
	{
		if (k == 0 && *auxp == '\n')//account for starting newline //should take this out of the loop and start from k = 1 for better efficiency
		{
			aux[auxindex++] = auxp;
			k++;
			auxp++;
		}
		aux[auxindex++] = auxp; 
		while (*auxp != '\n')
			auxp++;
		
		auxp++;	
	}

	//use qsort to sort auxilliary array
	qsort(aux, auxindex, sizeof(char*), rot13cmp);
	
	//next: print out encoded but sorted array
	/* What was wrong ibefore is that lengths array doesnt get sorted
	   when aux is sorted. Just create lengths after the sort. */
	
	//fill lengths array with appropriate lenghts for printing
	int lindex = 0;
	int length = 1;
	for (int k = 0; k < newlines; k++)
	{
		auxp = aux[k];
		while (*auxp != '\n')
		{
			auxp++;
			length++;
		}
		lengths[lindex++] = length;
		length = 1;
	}

	//print out encoded but sorted array using system calls
	for (int k = 0; k < newlines; k++)
	{
		char *toprint = aux[k];
		for (int i = 0; i < lengths[k]; i++)
		{
			write(1, &(*toprint), 1);
			toprint++;
		}
	}

	//free allocated memory
	free(p);
	free(aux);
	free(lengths);

	fprintf(stderr, "Number of comparisons: %d\n", comparisons);
	return 0;
}

/* Notes: E.g. when we call quicksort via qsort(arr, 10, sizeof(char*), rot13cmp)
   what qsort does is pass pointers to each element of arr into rot13cmp. So in the
   example above, where arr holds pointers to chars, rot13cmp receives pointers to
   pointers to chars (char**). That's why in rot13cmp we have to cast the arguments
   from const void * to const char**. Then you dereference the argument to get the
   actualy character pointer inside. This can be confusing. Just remember that a
   pointer to each element is passed to the compare function. */
