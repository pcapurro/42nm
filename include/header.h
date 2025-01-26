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
	bool		debugOnly;
	bool		externOnly;
	bool		undefinedOnly;

	bool		noSort;
	bool		reverseSort;

	bool		options;
	char**		paths;
	void**		binaries;

	char**		errors;
};

struct sSymbols
{
	char*	name;
	char*	type;
	char*	address;

	void*	data;
	int		link;
	bool	end;
};

struct sStrs
{
	char*	str;
	int		id;
};

typedef struct sInfos tInfos;
typedef struct sSymbols tSymbols;
typedef struct sStrs tStrs;

int		getArrLen(char** str);
void	writeStr(const char* str, const int fd);
char*	getNumber(const int number);
char*	getDup(const char* str);
char*	getJoin(const char* s1, const char* s2, const char* s3);
int		getStrLen(const char* str);

void	freeArray(char** array);
void	freeBinaries(void** binaries);

bool	isSame(const char* str1, const char* str2);

void	memoryFailed(void);
void	systemFailed(void);

void	getPath(tInfos* infos);
void	getPaths(tInfos* infos, const char** argv);

int		addOption(tInfos* infos, const char* argv);
void	getOptions(tInfos* infos, const char** argv);

bool	isELF(const char* binary, const long int len);

void	getError(tInfos* infos, const char* message, const int i);

char*	getName(tSymbols* symbol, tStrs* strs, const int value);
char*	getAddress(tSymbols* symbol, tStrs* strs, const int value);
char*	getType(const char* binary, tSymbols* symbol, tStrs* strs, const int value);

void	registerBinary32(const char* binary, tSymbols* symbols, tStrs* strs);
void	initializeBinary32(const char* binary, tSymbols** symbols, tStrs** strs);
void	analyzeBinary32(tInfos* infos, const char* binary, const int y);

void	registerBinary64(const char* binary, tSymbols* symbols, tStrs* strs);
void	initializeBinary64(const char* binary, tSymbols** symbols, tStrs** strs);
void	analyzeBinary64(tInfos* infos, const char* binary, const int y);

void	getSymbols(tInfos* infos);

void	reOrderSymbols(tInfos* infos);
void	listSymbols(tInfos* infos);

void	setToDefault(tInfos* infos);

void	printHelp(void);
bool	isHelp(const char* str);

int		main(const int argc, const char** argv);

#endif