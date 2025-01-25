#include "../../../include/header.h"

bool	isAbsolute(tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym*	data = symbol->data;

	if (data->st_shndx == SHN_ABS)
		return (true);

	return (false);
}

bool	isBSS(const char* binary, tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym*	data = symbol->data;
	Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;

	for (int i = 0; i != header->e_shnum; i++)
	{
		const void*	addr = binary + header->e_shoff + (i * header->e_shentsize);
		Elf64_Shdr*	section = (Elf64_Shdr*)addr;

		if (section->sh_type != SHT_NOBITS)
			continue ;
		else if (i == data->st_shndx)
			return (true);
	}

	return (false);
}

bool	isCommon(tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym	*data = symbol->data;

	if ((data->st_shndx == SHN_UNDEF) && ((data->st_info & 0xF) == STT_OBJECT))
		return (true);

	return (false);
}

bool	isInitialized(const char* binary, tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym*	data = symbol->data;

	return (false);
}

bool	isGlobal(tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym*	data = symbol->data;

	return (false);
}

bool	isIndirect(tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym*	data = symbol->data;

	if ((data->st_info & 0xF) == STT_NOTYPE && data->st_shndx == SHN_UNDEF)
		return (true);

	return (false);
}

bool	isDebug(tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym*	data = symbol->data;

	return (false);
}

bool	isReadMode(const char* binary, tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym*	data = symbol->data;
	Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;

	for (int i = 0; i != header->e_shnum; i++)
	{
		const void*	addr = binary + header->e_shoff + (i * header->e_shentsize);
		Elf64_Shdr*	section = (Elf64_Shdr*)addr;

		if (i == data->st_shndx)
		{
			// si alloué en mémoire + pas modifiable == lecture seule
			if ((section->sh_flags & SHF_ALLOC != 0) && (section->sh_flags & SHF_WRITE == 0))
				return (true);
			break ;
		}
	}

	return (false);
}

bool	isSmall(tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym*	data = symbol->data;

	return (false);
}

bool	isText(tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym*	data = symbol->data;

	return (false);
}

bool	isUndefined(tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym*	data = symbol->data;

	if (data->st_shndx == SHN_UNDEF)
		return (true);

	return (false);
}

bool	isWeakFound(tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym*	data = symbol->data;

	if (data->st_info >> 4 == STB_WEAK && data->st_shndx != SHN_UNDEF)
		return (true);

	return (false);
}

bool	isWeakNotFound(tSymbols* symbol, tStrs* strs)
{
	Elf64_Sym*	data = symbol->data;

	if (data->st_info >> 4 == STB_WEAK && data->st_shndx == SHN_UNDEF)
		return (true);

	return (false);
}

char*	getType(const char* binary, tSymbols* symbol, tStrs* strs)
{
	char*		type = NULL;

	type = getDup("- ");
	if (!type)
		memoryFailed(), exit(1);

	if (isAbsolute(symbol, strs) == true)
		type[0] = 'A';

	if (isBSS(binary, symbol, strs) == true)
		type[0] = 'B';

	if (isCommon(symbol, strs) == true)
		type[0] = 'C';

	if (isInitialized(binary, symbol, strs) == true)
		type[0] = 'D';

	if (isGlobal(symbol, strs) == true)
		type[0] = 'G';

	if (isIndirect(symbol, strs) == true)
		type[0] = 'I';

	if (isDebug(symbol, strs) == true)
		type[0] = 'N';

	if (isReadMode(binary, symbol, strs) == true)
		type[0] = 'R';

	if (isSmall(symbol, strs) == true)
		type[0] = 'S';

	if (isText(symbol, strs) == true)
		type[0] = 'T';

	if (isUndefined(symbol, strs) == true)
		type[0] = 'U';

	if (isWeakFound(symbol, strs) == true)
		type[0] = 'V';

	if (isWeakNotFound(symbol, strs) == true)
		type[0] = 'W';

	if (((Elf64_Sym *)symbol->data)->st_info >> 4 == STB_LOCAL
		&& type[0] != '-')
		type[0] = type[0] + 32;

	return (type);
}
