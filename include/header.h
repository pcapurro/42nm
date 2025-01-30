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

# define MEMORY_ERROR "Error! Memory allocation failed.\n"
# define SYSTEM_FAILED "Error! System failed.\n"

typedef struct stat tStat;

struct sInfos
{
	bool		externOnly;
	bool		undefinedOnly;

	bool		noSort;
	bool		reverseSort;

	bool		options;
	char**		paths;
	void**		binaries;

	int			fd;
	int			binaryLen;
	char*		binary;

	int			returnValue;
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
	
	bool	end;
};

typedef struct sInfos tInfos;
typedef struct sSymbols tSymbols;
typedef struct sStrs tStrs;

int		getArrLen(void** array);
void	writeStr(const char* str, const int fd);
char*	getDup(const char* str);
char*	getJoin(const char* s1, const char* s2, const char* s3);
int		getStrLen(const char* str);

void	freeArray(char** array);
void	freeBinary(void* binary);

bool	isSame(const char* str1, const char* str2);

void	memoryFailed(void);
void	systemFailed(void);

void	getPath(tInfos* infos);
void	getPaths(tInfos* infos, const char** argv);

int		addUnixOption(tInfos* infos, const char* argv);
int		addGnuOption(tInfos* infos, const char* argv);
void	getOptions(tInfos* infos, const char** argv);

bool	isELF(const char* binary, const long int len);

int		getError(tInfos* infos, const char* message, const int i);

char*	getName(tSymbols* symbol, tStrs* strs, int* value, const int arch);
char*	getAddress(tSymbols* symbol, tStrs* strs, const int arch);
char*	getType(const char* binary, tSymbols* symbol, int* value, const int arch);

int		registerBinary32(const char* binary, tSymbols* symbols, tStrs* strs);
int		initializeBinary32(const char* binary, tSymbols** symbols, tStrs** strs);

int		registerBinary64(const char* binary, tSymbols* symbols, tStrs* strs);
int		initializeBinary64(const char* binary, tSymbols** symbols, tStrs** strs);

int		analyzeBinary(tInfos* infos, const int y, const int arch);
void	getSymbols(tInfos* infos);

void	*orderSymbols(void** symbols);
void	*reverseSymbols(void** symbols);
void	listSymbols(tInfos* infos);

void	setToDefault(tInfos* infos);
void	setToNull(tInfos* infos);

void	printHelp(void);
bool	isHelp(const char* str);

int		main(const int argc, const char** argv);

#endif