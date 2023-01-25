#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXSIZE 1000
#define LINELEN 81
/* prototypes */
void printing(char* buffer,FILE *fp, FILE *fpNew ,char** macNames , char** macConts, int* maci);
void saveCon(char* buffer , char*** macConts,int *maci,int i,int *mcrBoolPtr);
void saveName(char* buffer, char*** macNames , char*** macConts,int* maci, int i, int* mcrBool); 
int main( int argc, char *argv[])
{	
	int numberOfArguments = argc;
	int i = 0;
	int mcrBool = 0;
	int maci = -1; /* macro  index */
	char **macConts = NULL; /*dynamicly initializing size of string array */
	char **macNames = NULL;/*dynamicly initializing size of string array */
	char buffer[LINELEN];
	FILE *fpNew = NULL;
	FILE *fp = NULL; /* file pointer */	
	char kelet1[30];
	char pelet1[30];
	strcpy(kelet1,argv[1]);
	strcat(kelet1,".as");
	strcpy(pelet1,argv[1]);
	strcat(pelet1,".am");
	
	if (numberOfArguments != 2) 
	{
		printf("number of arguments should be 2,\n");
		printf("1 for the program name,\nand 1 for  the file name.\n");
		return -1;
	}
	fp = fopen(kelet1, "r");
	
	if (fp == NULL)
	{	
		printf("reading failed\n");
        return -1;   
	} 
	fpNew = fopen (pelet1, "w");
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
			saveName(buffer,  &macNames ,  &macConts,&maci,i,&mcrBool);	
		}
		/*reading a macro*/
		else if ( mcrBool == 1) 
		{
			saveCon(buffer,&macConts,&maci,i,&mcrBool);	 
		}
		/*not reading a macro and is not a new macro definition line */
		else if (!(buffer[i] == 'm' && buffer[i+1] == 'c' && buffer[i+2] == 'r') && mcrBool == 0)
		{	
			printing(buffer,fp, fpNew , macNames , macConts,&maci); 
		}			
	}

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
void saveName(char* buffer, char*** macNames , char*** macConts,int* maci,int i , int* mcrBool) 
{
	
	int j;

	*mcrBool = 1;
	(*maci)++;
	/* extending the array for each macro */
	*macNames = realloc(*macNames,((*maci)+1)*sizeof(char*));
	*macConts = realloc(*macConts,((*maci)+1)*sizeof(char*));
	(*macConts)[*maci] =(char*)malloc((strlen(buffer)+1)*sizeof(char*));
	(*macNames)[*maci] =(char*)malloc((strlen(buffer)+1)*sizeof(char));

	i+=3; /* skipping the word mcr */
			
	for (; isspace(buffer[i]) ; i++); /* arrived to the macro name */
	for (j = 0; !isspace(buffer[i]) ;j++, i++) /* saving macro name */
	(*macNames)[(*maci)][j] = buffer[i]; /* to my array of strings */ 
}
/* this function save the contents of the macros in a dynamic array
for each content line we'll reallocate another space for it,
as the size of the line*/
void saveCon(char* buffer , char*** macConts,int *maci,int i,int* mcrBool)
{
	if(strncmp(buffer + i, "endmcr", 6)) /* not endmcr */
		{/* inserting the content of the macro to maccontent */
		 (*macConts)[*maci] = realloc((*macConts)[*maci],(strlen((*macConts)[*maci]+1)+(strlen(buffer)+1))*sizeof(char*));
		strcat((*macConts)[*maci],buffer);
	 
		}
	else /* endmcr */
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
