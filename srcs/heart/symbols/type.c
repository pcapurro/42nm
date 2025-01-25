#include "../../../include/header.h"

bool	isAbsolute(tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym *data = symbol->data;

	if (data->st_shndx == SHN_ABS)
		return (true);

	return (false);
}

bool	isBSS(tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym *data = symbol->data;

	if ((data->st_info & 0xF) == STT_OBJECT)
	{
		return (true);
	}

	return (false);
}

char*	getType(tSymbols* symbol, tStrs* strs)
{
	char*		type = NULL;

	type = getDup("- ");
	if (!type)
		memoryFailed(), exit(1);

	if (isAbsolute(symbol, strs) == true)
		type[0] = 'A';

	if (((Elf64_Sym *)symbol->data)->st_info >> 4 != STB_GLOBAL
		&& type[0] != '-')
		type[0] = type[0] + 32;

	return (type);
}
