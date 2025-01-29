#include "../../../include/header.h"

void	registerBinary32(const char* binary, tSymbols* symbols, tStrs* strs)
{
	;
}

void	*initializeBinary32(const char* binary, tSymbols** symbols, tStrs** strs)
{
	Elf32_Ehdr*	header = (Elf32_Ehdr*) binary;

	return (header);
}

void	analyzeBinary32(tInfos* infos, const int y)
{
	;
}

