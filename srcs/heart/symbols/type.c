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

	if ((data->st_shndx == SHN_UNDEF) && (ELF64_ST_TYPE(data->st_info) == STT_OBJECT))
		return (true);

	return (false);
}

static bool	isInitialized(const char* binary, tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;
	Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;

	if (data->st_shndx == SHN_UNDEF)
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

			break ;
		}
	}

	return (false);
}

static bool	isDebug(const char* binary, tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;
	Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;

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

static bool	isWeakUnknown(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if (data->st_info >> 4 == STB_WEAK)
		return (true);

	return (false);
}

static bool isLocalOrGlobal(const char type)
{
	if (type == '?')
		return (false);

	if (type == 'A' || type == 'N' \
		|| type == 'U' || type == 'C')
		return (false);

	return (true);
}

static bool	isLocal(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if (data->st_info >> 4 == STB_LOCAL)
		return (true);

	if (data->st_info >> 4 == STB_WEAK && data->st_shndx == 0)
		return (true);

	return (false);
}

char*	getType(const char* binary, tSymbols* symbol, tStrs* strs, const int value)
{
	char*	type = NULL;

	type = getDup("? ");
	if (!type)
		return (NULL);

	if (isAbsolute(symbol, strs, value) == true)
		type[0] = 'A'; // v

	if (isBSS(binary, symbol, strs, value) == true)
		type[0] = 'B'; // x

	if (isCommon(symbol, strs, value) == true)
		type[0] = 'C'; // x

	if (isInitialized(binary, symbol, strs, value) == true)
		type[0] = 'D'; // v

	if (isDebug(binary, symbol, strs, value) == true)
		type[0] = 'N'; // x

	if (isReadMode(binary, symbol, strs, value) == true)
		type[0] = 'R'; // x

	if (isText(symbol, strs, value) == true)
		type[0] = 'T'; // x

	if (isUndefined(symbol, strs, value) == true)
		type[0] = 'U'; // v

	if (isWeakUnknown(symbol, strs, value) == true)
		type[0] = 'W'; // v

	if (isLocalOrGlobal(type[0]) == true && isLocal(symbol, strs, value) == true)
		type[0] += 32; // v

	return (type);
}

	// A
	// // "A" The symbol's value is absolute, and  will not be changed by further linking.

	// B
	// // The symbol is in the BSS data section. This section typically contains zero-initialized
	// // or uninitialized data, although the exact behavior is system dependent.

	// C
	// The symbol is common. Common symbols are uninitialized data.
	// When linking, multiple common symbols may appear with the same
	// name. If the symbol is defined anywhere, the common symbols
	// are treated as undefined references.

	// D
	// // The symbol is in the initialized data section.

	// N
	// The symbol is a debugging symbol.

	// R
	// The symbol is in a read only data section.

	// T
	// The symbol is in the text (code) section.

	// U
	// The symbol is undefined.

	// W
	// The symbol is a weak symbol that has not been specifically
	// tagged as a weak object symbol. When a weak defined symbol is
	// linked with a normal defined symbol, the normal defined symbol
	// is used with no error. When a weak undefined symbol is linked
	// and the symbol is not defined, the value of the symbol is
	// determined in a system-specific manner without error. On some
	// systems, uppercase indicates that a default value has been specified.

// jbarbate :
// char	findType64(Elf64_Sym sym, Elf64_Shdr *shdr)
// {
// 	char  c;
	
// 	if (ELF64_ST_TYPE(sym.st_info) == STT_OBJECT && ELF64_ST_BIND(sym.st_info) == STB_WEAK)
// 		c = 'V';

// 	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK)
// 		c = 'W';

// 	else if (ELF64_ST_TYPE(sym.st_info) == STT_GNU_IFUNC)
// 		c = 'i';

// 	else if (ELF64_ST_TYPE(sym.st_info) == STT_FILE || sym.st_shndx == SHN_ABS)
// 		c = 'A';

// 	else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS)
// 		c = 'B';

// 	else if (shdr[sym.st_shndx].sh_flags == (SHF_MERGE | SHF_STRINGS)
// 		|| (!shdr[sym.st_shndx].sh_flags && sym.st_shndx))
// 		c = 'N';

// 	else if (shdr[sym.st_shndx].sh_flags & SHF_EXECINSTR)
// 		c = 'T';

// 	else if (shdr[sym.st_shndx].sh_flags == (SHF_WRITE | SHF_ALLOC))
// 		c = 'D';

// 	else if (shdr[sym.st_shndx].sh_flags == SHF_ALLOC)
// 		c = 'R';

// 	else if (ELF64_ST_TYPE(sym.st_info) == STT_COMMON)
// 		c = 'C';

// 	else if (!sym.st_shndx)
// 		c = 'U';

// 	else
// 		c = '?';

// 	return c;
// }
