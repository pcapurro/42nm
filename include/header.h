#ifndef HEADER_H
# define HEADER_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <stdio.h>
# include <stdbool.h>

# include <sys/mman.h>
# include <sys/stat.h>

struct sInfos
{
	bool	debugOnly;
	bool	externOnly;
	bool	undefinedOnly;

	bool	noSort;
	bool	reverseSort;

	bool	options;
	char**	paths;
	char**	binaries;

	int		error;
};

typedef struct sInfos tInfos;

int		getArrLen(char** str);
void	writeStr(const char* str, const int fd);
char*	getNumber(const int number);
char*	getDup(const char* str);
char*	getJoin(const char* s1, const char* s2, const char* s3);
int		getStrLen(const char* str);

void	freeArray(char** array);

void	memoryFailed(void);
void	systemFailed(void);

void	getPaths(tInfos* infos, const char** argv);
void	getSymbols(tInfos* infos);
void	readBinaries(tInfos* infos);
void	readBinary(tInfos* infos);

void	setToDefault(tInfos* infos);

void	printHelp(void);
bool	isHelp(const char* str);

int		main(const int argc, const char** argv);

#endif