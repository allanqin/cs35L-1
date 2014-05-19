#include <stdio.h>
#include <stdlib.h>

int rot13cmp(const void *a, const void *b)
{
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

int main()
{
	//read input and put into dynamically reallocated array
	int bufsize = 2048;
	int count = 0;
	char *p = (char*) malloc(sizeof(char) * bufsize);

	if (p == NULL){
		fprintf(stderr, "Error allocating dynamic memory.");
		exit(1);
	}
	
	int character;
	int notempty = 0;
	while(1)
	{
		character = getchar();
		if (character == EOF)
			break;
		notempty = 1;
		p[count++] = (char) character;
		if (count == bufsize){
			p = (char*) realloc(p, bufsize * 2);
			if (p == NULL){
				fprintf(stderr, "Error with memory reallocation.");
				exit(1);
			}
			bufsize *= 2;
		}
	}
	
	//notempty detects if an empty file was passed in
	if (notempty == 0)
		return 0;	
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
		if (k == 0 && *auxp == '\n')//account for starting newline //should take this out of the loop and start from k = 1
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
	//what was wrong is that lengths array doesnt get sorted when aux is sorted. Just implement the same logic
	
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

	//print out encoded but sorted array
	for (int k = 0; k < newlines; k++)
	{
		char *toprint = aux[k];
		for (int i = 0; i < lengths[k]; i++)
		{
			printf("%c", *toprint);
			toprint++;
		}
	}

	//free allocated memory
	free(p);
	free(aux);
	free(lengths);
	return 0;
}

/* Notes: E.g. when we call quicksort via qsort(arr, 10, sizeof(char*), rot13cmp)
   what qsort does is pass pointers to each element of arr into rot13cmp. So in the
   example above, where arr holds pointers to chars, rot13cmp receives pointers to
   pointers to chars (char**). That's why in rot13cmp we have to cast the arguments
   from const void * to const char**. Then you dereference the argument to get the
   actualy character pointer inside. This can be confusing. Just remember that a
   pointer to each element is passed to the compare function. */
