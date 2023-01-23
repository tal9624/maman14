#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#define LINELEN 81
#define VARLEN 30 


int main( int argc, char *argv[])
{	
	int equal = 0;
	char *firstRead = NULL; /* first word in line */
	size_t lineLen = 81;
	int mcrBool = 0;
	int j = 0;
	int mci = -1; /* macro content index */
	int mni = -1; /* macro name index */
	char **macConts = NULL; /* for macro contenct */
	char **macNames = NULL;
	char buffer[LINELEN];
	FILE *fpNew;
	int i = 0;
	char *kelet = argv[1];
	int numberOfArguments = argc;
	FILE *fp = NULL; /* file pointer */
	macNames = malloc(0 * sizeof(char*)); /* dynamicly initializing size of string array */
	macConts = malloc(0 * sizeof(char*)); /* dynamicly initializing size of string array */
	firstRead = malloc(0 * sizeof(char*));
	
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
		if (buffer[i] == 'm' && buffer[i+1] == 'c' && buffer[i+2] == 'r' && mcrBool == 0) /* identify mcr */
		{
			mcrBool=1; /* turning on mcr flag */
			macNames = realloc(macNames,1*sizeof(char*)); /* extending the array 																	for each macro */
			macConts = realloc(macConts,1*sizeof(char*));
			mci++;
			macConts[mci] =(char*)malloc(0*sizeof(char));
			mni++; 
			macNames[mni] =(char*)malloc(VARLEN*sizeof(char));
			i+=3; /* skipping the word mcr */
			
			for (i +=0 ; isspace(buffer[i]) ; i++); /* arrived to the macro name */
			for (i +=0 , j = 0 ; !isspace(buffer[i]);j++, i++) /* saving macro name */
			macNames[mni][j] = buffer[i]; /* to my array of strings */ 
			macNames[mni+1] = '\0';
		
		}
		else if ( mcrBool == 1) 
		{
				if (!(buffer[i] == 'e' && buffer[i+1] == 'n' && buffer[i+2] == 'd' /* not */
				&& buffer[i+3] == 'm' && buffer[i+4] == 'c' &&  buffer[i+5] == 'r')) /* endmcr*/
					{/* inserting the content of the macro to maccontent */
					macConts[mci] = realloc(macConts[mci],lineLen*sizeof(char));
						strcat(macConts[mci],buffer);
						macConts[mci+1] = '\0';	
					}
				else if (buffer[i] == 'e' && buffer[i+1] == 'n' && buffer[i+2] == 'd'
				&& buffer[i+3] == 'm' && buffer[i+4] == 'c' &&  buffer[i+5] == 'r')/* endmcr */
					{
					mcrBool = 0;
					}
		}
		else if (!(buffer[i] == 'm' && buffer[i+1] == 'c' && buffer[i+2] == 'r') && mcrBool == 0)
		{	
			firstRead = (char*)malloc(LINELEN*sizeof(char)); /* initialzing first word in line */
			fscanf(fp,"%s",firstRead);
			fseek(fp, -strlen(firstRead), SEEK_CUR);
			equal = 0;
			/*printing to the file, with macros as should be */
			for ( i = 0 ; i <= mni ; i ++ )
			{
				if((strcmp(firstRead,macNames[i]) == 1) ||(strcmp(firstRead,macNames[i]) == -1))
				{
				equal = 0;
				}
				if((strcmp(firstRead,macNames[i]) == 0))
				{
				equal = 1;
				fputs(buffer,fpNew);
				fprintf(fpNew,"%s",macConts[i]);
				fgets(buffer,LINELEN, fp);
				break; /* we dont need anymore checks , and we want to keep equal value as 1 */
				}	
			}
			if (equal == 0)
			{
			fputs(buffer,fpNew);
			}		
		}			
	}
	remove(kelet);
	rename("temp.txt", kelet);
	free(*macNames);
	free(macNames);
	free(firstRead);
	free(*macConts);
	free(macConts);
	fclose(fp);
	fp = NULL;
	fclose(fpNew);
	fpNew = NULL;
	return 0;	
}	

			
		
			
			
			
			
			
			
			
			
			
			
			
			
			
			
