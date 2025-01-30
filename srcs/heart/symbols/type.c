#include "../../../include/header.h"

static bool isInvalid(const char* binary, tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if (data->st_value < 0)
		return (true);
	// invalid address

	if (ELF64_ST_TYPE(data->st_info) == STT_SECTION)
		return (true);
	// the symbol represents a section, not a real symbol

	if (data->st_value > 0 && data->st_shndx == SHN_UNDEF)
		return (true);
	// an absolute symbol can't have a valid address

	return (false);
}

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

	if (data->st_shndx == 0)
		return (false);

	for (int i = 0; i != header->e_shnum; i++)
	{
		const void*	addr = binary + header->e_shoff + (i * header->e_shentsize);
		Elf64_Shdr*	section = (Elf64_Shdr*)addr;

		if (i == data->st_shndx)
		{
			if (section->sh_type != SHT_NOBITS)
				return (false);

			return (true);
		}
	}

	return (false);
}

static bool	isCommon(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym	*data = symbol->data;

	if (ELF64_ST_TYPE(data->st_info) == STT_COMMON)
		return (true);

	return (false);
}

static bool	isInitialized(const char* binary, tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;
	Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;

	if (data->st_shndx == SHN_UNDEF)
		return (false);

	if (ELF64_ST_BIND(data->st_info) == STB_WEAK)
		return (false);

	for (int i = 0; i != header->e_shnum; i++)
	{
		const void*	addr = binary + header->e_shoff + (i * header->e_shentsize);
		Elf64_Shdr*	section = (Elf64_Shdr*)addr;

		if (i == data->st_shndx)
		{
			if (section->sh_type == SHT_NOBITS || section->sh_type == SHT_SYMTAB \
				|| section->sh_type == SHT_STRTAB || section->sh_type == SHT_RELA)
				return (false);

			if (section->sh_type == SHT_REL || section->sh_type == SHT_HASH \
				|| section->sh_type == SHT_NOTE)
				return (false);

			if (section->sh_flags & SHF_ALLOC != 0)
				return (true);

			return (false);
		}
	}

	return (false);
}

static bool	isIndirect(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym	*data = symbol->data;

	if (ELF64_ST_TYPE(data->st_info) == STT_GNU_IFUNC)
		return (true);

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
			if ((section->sh_flags & SHF_ALLOC) != 0 && (section->sh_flags & SHF_WRITE) == 0 \
				&& (section->sh_flags & SHF_EXECINSTR) == 0)
				return (true);

			return (false);
		}
	}

	return (false);
}

static bool	isText(const char* binary, tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;
	Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;

	if (ELF64_ST_TYPE(data->st_info) == STT_OBJECT)
		return (false);

	for (int i = 0; i != header->e_shnum; i++)
	{
		const void*	addr = binary + header->e_shoff + (i * header->e_shentsize);
		Elf64_Shdr*	section = (Elf64_Shdr*)addr;

		if (i == data->st_shndx)
		{
			if ((section->sh_flags & SHF_EXECINSTR) != 0 && (section->sh_flags & SHF_ALLOC) != 0 \
				&& section->sh_type == SHT_PROGBITS)
				return (true);

			return (false);
		}
	}

	return (false);
}

static bool	isUndefinedWeak(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if (ELF64_ST_BIND(data->st_info) != STB_GLOBAL)
		return (false);

	if (data->st_shndx == SHN_UNDEF && ELF64_ST_BIND(data->st_info) == STB_WEAK)
		return (true);

	return (false);
}

static bool	isUndefined(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if (data->st_shndx == SHN_UNDEF && ELF64_ST_BIND(data->st_info) == STB_GLOBAL)
		return (true);

	return (false);
}

static bool	isWeakNormal(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if (ELF64_ST_TYPE(data->st_info) == STT_OBJECT && ELF64_ST_BIND(data->st_info) == STB_WEAK)
		return (true);

	return (false);
}

static bool	isWeakUnknown(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if (ELF64_ST_TYPE(data->st_info) != STT_OBJECT && ELF64_ST_BIND(data->st_info) == STB_WEAK)
		return (true);

	return (false);
}

static bool isLocalOrGlobal(const char type)
{
	if (type == '?')
		return (false);

	if (type == 'U')
		return (false);

	if (type == 'i' || type == 'u' || type == 'n')
		return (false);

	return (true);
}

static bool	isGlobal(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if (ELF64_ST_BIND(data->st_info) == STB_GLOBAL \
		|| (ELF64_ST_BIND(data->st_info) == STB_WEAK && data->st_shndx != SHN_UNDEF))
		return (true);

	return (false);
}

char*	getType(const char* binary, tSymbols* symbol, tStrs* strs, const int value)
{
	char*	type = NULL;

	type = getDup("? ");
	if (!type)
		return (NULL);

	if (isInvalid(binary, symbol, strs, value) == true)
		type[0] = '!';

	else if (isAbsolute(symbol, strs, value) == true)
		type[0] = 'A';

	else if (isBSS(binary, symbol, strs, value) == true)
		type[0] = 'B';

	else if (isCommon(symbol, strs, value) == true)
		type[0] = 'C';

	else if (isInitialized(binary, symbol, strs, value) == true)
		type[0] = 'D';

	else if (isUndefined(symbol, strs, value) == true)
		type[0] = 'U';

	else if (isWeakNormal(symbol, strs, value) == true)
		type[0] = 'V';

	else if (isWeakUnknown(symbol, strs, value) == true)
		type[0] = 'W';

	else if (isUndefinedWeak(symbol, strs, value) == true)
		type[0] = 'u';

	else if (isReadMode(binary, symbol, strs, value) == true)
		type[0] = 'R';

	else if (isText(binary, symbol, strs, value) == true)
		type[0] = 'T';

	else if (isIndirect(symbol, strs, value) == true)
		type[0] = 'i';

	if (isLocalOrGlobal(type[0]) == true \
		&& isGlobal(symbol, strs, value) == false)
		type[0] += 32;

	return (type);
}
