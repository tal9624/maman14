#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXSIZE 1000
#define LINELEN 81
#define VARLEN 30 
/* prototypes */
void printing(char* buffer,FILE *fp, FILE *fpNew ,char** macNames , char** macConts, int* maci);
void saveCon(char* buffer , char** macConts,int maci,int i,int *mcrBoolPtr);
void saveName(char* buffer, char** macNames , char** macConts,int* maci, int i, int* mcrBool); 
int main( int argc, char *argv[])
{	
	int numberOfArguments = argc;
	int i = 0;
	int mcrBool = 0;
	int maci = -1; /* macro  index */
	char **macConts = malloc(MAXSIZE * sizeof(char)); /*dynamicly initializing size of string array */
	char **macNames = malloc(MAXSIZE * sizeof(char)); /*dynamicly initializing size of string array */
	char buffer[LINELEN];
	char *kelet = argv[1];
	FILE *fpNew = NULL;
	FILE *fp = NULL; /* file pointer */	
	if (numberOfArguments != 2) 
	{
		printf("number of arguments should be 2,\n");
		printf("1 for the program name,\nand 1 for  the file name.\n");
		return -1;
	}
	fp = fopen(kelet, "r");
	
	if (fp == NULL)
	{	
		printf("reading failed\n");
        return -1;   
	} 
	fpNew = fopen ("temp.txt", "w");
	if (fpNew == NULL)
	{	
		printf("reading failed\n");
        return -1;   
	} 
	/* saving macros name in macNames array of strings
	and also saving macro contents in maccontencts array of strings */ 
	while (fgets(buffer,LINELEN, fp))  
	{ 
		/* going to the macro name with i */
		for (i = 0; isspace(buffer[i]) ; i++); /* going to the first word in line */
		/* identify mcr in this if statement */
		if (buffer[i] == 'm' && buffer[i+1] == 'c' && buffer[i+2] == 'r' && mcrBool == 0) 
		{
			/*saving the name of it*/
			saveName(buffer,  macNames ,  macConts,&maci,i,&mcrBool);	
		}
		/*reading a macro*/
		else if ( mcrBool == 1) 
		{
			saveCon(buffer,macConts,maci,i,&mcrBool);	 
		}
		/*not reading a macro and is not a new macro definition line */
		else if (!(buffer[i] == 'm' && buffer[i+1] == 'c' && buffer[i+2] == 'r') && mcrBool == 0)
		{	
			printing(buffer,fp, fpNew , macNames , macConts,&maci); 
		}			
	}
	remove(kelet);
	rename("temp.txt", kelet);
	for (i=0;i<=maci;i++)
	free(macNames[i]);
	free(macNames);
	for (i=0;i<=maci;i++)
	free(macConts[i]);
	free(macConts);
	fclose(fp);
	fp = NULL;
	fclose(fpNew);
	fpNew = NULL;
	return 0;	
}	
/* this function save the names of the macros in a dynamic array
for each name well reallocate another space for it
well expend also the contents array for each name well define */
void saveName(char* buffer, char** macNames , char** macConts,int* maci,int i , int* mcrBool) 
{
	int j;
	*mcrBool = 1;
	(*maci)++;
	macNames[*maci] =(char*)malloc(VARLEN*sizeof(char));
	i+=3; /* skipping the word mcr */
			
	for (; isspace(buffer[i]) ; i++); /* arrived to the macro name */
	for (j = 0; !isspace(buffer[i]) ;j++, i++) /* saving macro name */
	macNames[(*maci)][j] = buffer[i]; /* to my array of strings */ 
}
/* this function save the contents of the macros in a dynamic array
for each content line we'll reallocate another space for it,
as the size of the line*/
void saveCon(char* buffer , char** macConts,int maci,int i,int* mcrBool)
{
	if (!(buffer[i] == 'e' && buffer[i+1] == 'n' && buffer[i+2] == 'd' /* not... */
	&& buffer[i+3] == 'm' && buffer[i+4] == 'c' &&  buffer[i+5] == 'r')) /* ...endmcr*/
		{/* inserting the content of the macro to maccontent */
		macConts[maci] = realloc(macConts[maci],MAXSIZE*sizeof(char));
		strcat(macConts[maci],buffer);
		macConts[maci+1] = '\0';	
		}
	else if (buffer[i] == 'e' && buffer[i+1] == 'n' && buffer[i+2] == 'd'
	&& buffer[i+3] == 'm' && buffer[i+4] == 'c' &&  buffer[i+5] == 'r')/* endmcr */
			{
			*mcrBool = 0;
			}
}
/*printing to a temp file the requested */				
void printing(char* buffer,FILE *fp, FILE *fpNew ,char** macNames , char** macConts, int *maci)	
{	
	int i;
	int equal = 0;
	char firstRead[LINELEN];
	sscanf(buffer,"%s",firstRead); /* initialzing first word in line */
	/*printing to the file, with macros as should be */
	for ( i = 0 ; i <= (*maci) ; i ++ )
	{
		/*checking if the first word in the line is equal
		to one of the macro names*/
		/* if equal*/
		if((strcmp(firstRead,macNames[i]) == 0))
		{
		equal = 1;
		fputs(macConts[i],fpNew); 
		}	
	}
	/* if not equal*/
	if ((equal) == 0)
	{
	fputs(buffer,fpNew);
	}			
}						
