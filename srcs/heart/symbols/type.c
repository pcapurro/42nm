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
			if ((section->sh_flags & SHF_ALLOC) != 0 && (section->sh_flags & SHF_WRITE) == 0 \
				&& (section->sh_flags & SHF_EXECINSTR) == 0)
				return (true);

			return (false);
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

static bool	isWeakNormal(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	return (false);
}

static bool	isWeakUnknown(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if (ELF64_ST_BIND(data->st_info) == STB_WEAK)
		return (true);

	return (false);
}

static bool isLocalOrGlobal(const char type)
{
	if (type == '?')
		return (false);

	if (type == 'N' || type == 'U')
		return (false);

	return (true);
}

static bool	isLocal(tSymbols* symbol, tStrs* strs, const int value)
{
	Elf64_Sym*	data = symbol->data;

	if (ELF64_ST_BIND(data->st_info) == STB_LOCAL)
		return (true);

	if (ELF64_ST_BIND(data->st_info) == STB_WEAK && data->st_shndx == 0)
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
		type[0] = 'A'; // v

	else if (isBSS(binary, symbol, strs, value) == true)
		type[0] = 'B'; // v
	// the symbol is in the '.bss' data section (= a section of uninitialized data) (SHT_NOBITS type)
	// .bss data section:
	// = commonly used for static/global variables uninitialized or set to zero
	// > doesn't contain any data in the binary file, space being used during execution

	else if (isCommon(symbol, strs, value) == true)
		type[0] = 'C'; // v

	else if (isInitialized(binary, symbol, strs, value) == true)
		type[0] = 'D'; // v

	else if (isDebug(binary, symbol, strs, value) == true)
		type[0] = 'N'; // x

	else if (isReadMode(binary, symbol, strs, value) == true)
		type[0] = 'R'; // v
	// the symbol is in a read-only section
	// = any section with allocated without the flag SHF_WRITE
	// = and the section must have the flag SHF_EXECINSTR

	else if (isText(symbol, strs, value) == true)
		type[0] = 'T'; // x

	else if (isUndefined(symbol, strs, value) == true)
		type[0] = 'U'; // v

	else if (isWeakNormal(symbol, strs, value) == true)
		type[0] = 'V'; // x

	else if (isWeakUnknown(symbol, strs, value) == true)
		type[0] = 'W'; // v

	else if (isLocalOrGlobal(type[0]) == true && isLocal(symbol, strs, value) == true)
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

	// V
	// The symbol is a weak object. When a weak defined symbol is
	// linked with a normal defined symbol, the normal defined symbol
	// is used with no error. When a weak undefined symbol is linked
	// and the symbol is not defined, the value of the weak symbol
	// becomes zero with no error. On some systems, uppercase
	// indicates that a default value has been specified.

	// W
	// The symbol is a weak symbol that has not been specifically
	// tagged as a weak object symbol. When a weak defined symbol is
	// linked with a normal defined symbol, the normal defined symbol
	// is used with no error. When a weak undefined symbol is linked
	// and the symbol is not defined, the value of the symbol is
	// determined in a system-specific manner without error. On some
	// systems, uppercase indicates that a default value has been specified.
