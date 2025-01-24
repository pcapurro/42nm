#ifndef HEADER_H
# define HEADER_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <stdio.h>
# include <stdbool.h>
# include <errno.h>

# include <elf.h>

# include <sys/mman.h>
# include <sys/stat.h>

typedef struct stat tStat;

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
};

typedef struct sInfos tInfos;

int		getArrLen(char** str);
void	writeStr(const char* str, const int fd);
char*	getNumber(const int number);
char*	getDup(const char* str);
char*	getJoin(const char* s1, const char* s2, const char* s3);
int		getStrLen(const char* str);

void	freeArray(char** array);
bool	isSame(const char* str1, const char* str2);

void	memoryFailed(void);
void	systemFailed(void);

void	getPath(tInfos* infos);
void	getPaths(tInfos* infos, const char** argv);

int		addOption(tInfos* infos, const char* argv);
void	getOptions(tInfos* infos, const char** argv);

void	getError(tInfos* infos, const char* message, const int i);
void	analyze64Binary(tInfos* infos, const char* binary, const long int len, const int i);
void	getSymbols(tInfos* infos);

void	readBinaries(tInfos* infos);
void	readBinary(tInfos* infos);

void	setToDefault(tInfos* infos);

void	printHelp(void);
bool	isHelp(const char* str);

int		main(const int argc, const char** argv);

#endif