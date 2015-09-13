/* array of invalid chars under the windows filesystem */
const char* INVALCHR = "\\|:\"*?<>";
/* size of INVALCHR */
const int INVALCHRNUM = 8;

/* TODO: add -r (--recursive) option */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG

char** getFileList(char** argv, int* argc, const char* filename);
void verify(const char* s);
void help(char* bin);

int main(int argc, char** argv) 
{
	int count, fileno;
	char* filename = NULL;
	char** files = NULL;

	if(argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")))
	{
		help(argv[0]);
		return 0;
	}

	filename = malloc(200 * sizeof(char));
	if(argc == 1)
		filename = strcpy(filename, "."); 
	else if(argc == 2)
		filename = strcpy(filename, argv[1]);
	else
	{
		fprintf(stderr, "%s: improper number of arguments\n", argv[0]);
		help(argv[0]);
		free(filename);
		return 1;
	}

	fileno = 0;
	files = getFileList(files, &fileno, filename);

	for(count = 0; count < fileno; count++)
	{
		if(!strcmp(files[count], ".") || !strcmp(files[count], ".."))
		continue;

		verify(files[count]);
	}

	for(count = 0; count < fileno; count++)
		free(files[count]);
	free(files);

	free(filename);

	return 0;
}

char** getFileList(char** argv, int* argc, const char* filename)
{
	DIR *dir;
	struct dirent *ent;

	if((dir = opendir(filename)) != NULL) 
	{
		while((ent = readdir(dir)) != NULL) 
		{
			(*argc)++;
			argv = realloc(argv, (*argc) * sizeof (char*));
			argv[(*argc) - 1] = malloc(100 * sizeof(char));
			strcpy(argv[*argc - 1], ent->d_name);

#ifdef DEBUG
			printf("%s\n", argv[*argc - 1]);
#endif

		}
		closedir(dir);
	} 
	else
	{
		/* could not open directory */
		perror("");
		return NULL;
	}

	return argv;
}

void verify(const char* s)
{
	int count1, count2;
	for(count1 = 0; s[count1] != '\0'; count1++)
	{
		if(count1 == 0 && s[count1] == '.')
		printf("%s: entries cannot start with '.'\n", s);

		for(count2 = 0; count2 < INVALCHRNUM; count2++)
		{
			if(INVALCHR[count2] == s[count1])
			printf("%s: %c\n", s, s[count1]);
		}
	}

	if(count1 > 199)
		printf("%s: filename too long (%d)\n", s, count1);
}

void help(char* bin)
{
	printf("Usage: %s [DIR]\n" 
		   "Prints which filesystem entries in DIR, if any, are invalid in a Windows filesystem\n"
		   "If no DIR is specified, defaults to current directory\n", bin);
}
