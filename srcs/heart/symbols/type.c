#include "../../../include/header.h"

static bool	isAbsolute(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if (data->st_shndx == SHN_ABS)
		return (true);

	return (false);
}

static bool	isBSS(const char* binary, tSymbols* symbol, tStrs* strs, const int value)
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

static bool	isCommon(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym	*data = symbol->data;

	if ((data->st_shndx == SHN_UNDEF) && ((data->st_info & 0xF) == STT_OBJECT))
		return (true);

	return (false);
}

static bool	isInitialized(const char* binary, tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	return (false);
}

static bool	isGlobal(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	return (false);
}

static bool	isIndirect(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if ((data->st_info & 0xF) == STT_NOTYPE && data->st_shndx == SHN_UNDEF)
		return (true);

	return (false);
}

static bool	isDebug(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	return (false);
}

static bool	isReadMode(const char* binary, tSymbols* symbol, tStrs* strs, const int value)
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

static bool	isSmall(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	return (false);
}

static bool	isText(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	return (false);
}

static bool	isUndefined(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if (data->st_shndx == SHN_UNDEF)
		return (true);

	return (false);
}

static bool	isWeak(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if (data->st_info >> 4 == STB_WEAK)
		return (true);

	return (false);
}

static bool	isStrong(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if (data->st_info >> 4 == STB_WEAK && data->st_shndx == SHN_UNDEF)
		return (true);

	return (false);
}

char*	getType(const char* binary, tSymbols* symbol, tStrs* strs, const int value)
{
	char*		type = NULL;

	type = getDup("- ");
	if (!type)
		memoryFailed(), exit(1);

	if (isAbsolute(symbol, strs, value) == true)
		type[0] = 'A';

	if (isBSS(binary, symbol, strs, value) == true)
		type[0] = 'B';

	if (isCommon(symbol, strs, value) == true)
		type[0] = 'C';

	if (isInitialized(binary, symbol, strs, value) == true)
		type[0] = 'D';

	if (isGlobal(symbol, strs, value) == true)
		type[0] = 'G';

	if (isIndirect(symbol, strs, value) == true)
		type[0] = 'I';

	if (isDebug(symbol, strs, value) == true)
		type[0] = 'N';

	if (isReadMode(binary, symbol, strs, value) == true)
		type[0] = 'R';

	if (isSmall(symbol, strs, value) == true)
		type[0] = 'S';

	if (isText(symbol, strs, value) == true)
		type[0] = 'T';

	if (isUndefined(symbol, strs, value) == true)
		type[0] = 'U';

	if (isStrong(symbol, strs, value) == true)
		type[0] = 'V';

	if (isWeak(symbol, strs, value) == true)
		type[0] = 'W'; //

	if (isSame(symbol->name, "test") == true)
	{
		if (((Elf64_Sym *)symbol->data)->st_info >> 4 == STB_WEAK)
			printf("weak\n");
		if (((Elf64_Sym *)symbol->data)->st_info >> 4 == STB_GLOBAL)
			printf("global\n");
		if (((Elf64_Sym *)symbol->data)->st_info >> 4 == STB_LOCAL)
			printf("local\n");

		if (((Elf64_Sym *)symbol->data)->st_shndx == 0)
			printf("undef\n");
	}

	if (((Elf64_Sym *)symbol->data)->st_info >> 4 == STB_WEAK \
		&& ((Elf64_Sym *)symbol->data)->st_shndx == 0 && type[0] != '-')
		type[0] = type[0] + 32;

	return (type);
}
