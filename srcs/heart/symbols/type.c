#include "../../../include/header.h"

static bool isInvalid(tSymbols* symbol, const int arch)
{
	Elf64_Sym*	data64 = symbol->data;
	Elf32_Sym*	data32 = symbol->data;

	if (arch == 64 && ELF64_ST_TYPE(data64->st_info) == STT_SECTION)
		return (true);

	if (arch == 32 && ELF32_ST_TYPE(data32->st_info) == STT_SECTION)
		return (true);

	return (false);
}

static bool	isAbsolute(tSymbols* symbol, const int arch)
{
	Elf64_Sym*	data64 = symbol->data;
	Elf32_Sym*	data32 = symbol->data;

	if (arch == 64 && data64->st_shndx == SHN_ABS)
		return (true);

	if (arch == 32 && data32->st_shndx == SHN_ABS)
		return (true);

	return (false);
}

static bool	isBSS(const char* binary, tSymbols* symbol, int* value, const int len, const int arch)
{
	if (arch == 64)
	{
		Elf64_Sym*	data = symbol->data;
		Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;

		if (data->st_shndx == 0)
			return (false);

		for (int i = 0, newValue = 0; i != header->e_shnum; i++)
		{
			newValue = header->e_shoff + (i * header->e_shentsize);
			if (newValue <= 0 || newValue >= len)
				{ *value = 2; return (true); }

			const void*	addr = binary + newValue;
			Elf64_Shdr*	section = (Elf64_Shdr*)addr;

			if (i == data->st_shndx)
			{
				if (section->sh_type != SHT_NOBITS)
					return (false);

				return (true);
			}
		}
	}

	if (arch == 32)
	{
		Elf32_Sym*	data = symbol->data;
		Elf32_Ehdr*	header = (Elf32_Ehdr*) binary;

		if (data->st_shndx == 0)
			return (false);

		for (int i = 0, newValue = 0; i != header->e_shnum; i++)
		{
			newValue = header->e_shoff + (i * header->e_shentsize);
			if (newValue <= 0 || newValue >= len)
				{ *value = 2; return (true); }

			const void*	addr = binary + newValue;
			Elf32_Shdr*	section = (Elf32_Shdr*)addr;

			if (i == data->st_shndx)
			{
				if (section->sh_type != SHT_NOBITS)
					return (false);

				return (true);
			}
		}
	}

	return (false);
}

static bool	isCommon(tSymbols* symbol, const int arch)
{
	Elf64_Sym*	data64 = symbol->data;
	Elf32_Sym*	data32 = symbol->data;

	if (arch == 64 && ELF64_ST_TYPE(data64->st_info) == STT_COMMON)
		return (true);

	if (arch == 32 && ELF32_ST_TYPE(data32->st_info) == STT_COMMON)
		return (true);

	return (false);
}

static bool	isInitialized(const char* binary, tSymbols* symbol, int* value, const int len, const int arch)
{
	if (arch == 64)
	{
		Elf64_Sym*	data = symbol->data;
		Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;

		if (data->st_shndx == SHN_UNDEF)
			return (false);

		if (ELF64_ST_BIND(data->st_info) == STB_WEAK)
			return (false);

		for (int i = 0, newValue = 0; i != header->e_shnum; i++)
		{
			newValue = header->e_shoff + (i * header->e_shentsize);
			if (newValue <= 0 || newValue >= len)
				{ *value = 2; return (true); }

			const void*	addr = binary + newValue;
			Elf64_Shdr*	section = (Elf64_Shdr*)addr;

			if (i == data->st_shndx)
			{
				if (section->sh_type == SHT_NOBITS || section->sh_type == SHT_SYMTAB \
					|| section->sh_type == SHT_STRTAB || section->sh_type == SHT_RELA)
					return (false);

				if (section->sh_type == SHT_REL || section->sh_type == SHT_HASH \
					|| section->sh_type == SHT_NOTE)
					return (false);

				if ((section->sh_flags & SHF_EXECINSTR) != 0)
					return (false);

				return (true);
			}
		}
	}

	if (arch == 32)
	{
		Elf32_Sym*	data = symbol->data;
		Elf32_Ehdr*	header = (Elf32_Ehdr*) binary;

		if (data->st_shndx == SHN_UNDEF)
			return (false);

		if (ELF32_ST_BIND(data->st_info) == STB_WEAK)
			return (false);

		for (int i = 0, newValue = 0; i != header->e_shnum; i++)
		{
			newValue = header->e_shoff + (i * header->e_shentsize);
			if (newValue <= 0 || newValue >= len)
				{ *value = 2; return (true); }

			const void*	addr = binary + newValue;
			Elf32_Shdr*	section = (Elf32_Shdr*)addr;

			if (i == data->st_shndx)
			{
				if (section->sh_type == SHT_NOBITS || section->sh_type == SHT_SYMTAB \
					|| section->sh_type == SHT_STRTAB || section->sh_type == SHT_RELA)
					return (false);

				if (section->sh_type == SHT_REL || section->sh_type == SHT_HASH \
					|| section->sh_type == SHT_NOTE)
					return (false);

				if ((section->sh_flags & SHF_EXECINSTR) != 0)
					return (false);

				return (true);
			}
		}
	}

	return (false);
}

static bool	isIndirect(tSymbols* symbol, const int arch)
{
	Elf64_Sym*	data64 = symbol->data;
	Elf32_Sym*	data32 = symbol->data;

	if (arch == 64 && ELF64_ST_TYPE(data64->st_info) == STT_GNU_IFUNC)
		return (true);

	if (arch == 32 && ELF32_ST_TYPE(data32->st_info) == STT_GNU_IFUNC)
		return (true);

	return (false);
}

static bool	isReadMode(const char* binary, tSymbols* symbol, int* value, const int len, const int arch)
{
	if (arch == 64)
	{
		Elf64_Sym*	data = symbol->data;
		Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;

		for (int i = 0, newValue = 0; i != header->e_shnum; i++)
		{
			newValue = header->e_shoff + (i * header->e_shentsize);
			if (newValue <= 0 || newValue >= len)
				{ *value = 2; return (true); }

			const void*	addr = binary + newValue;
			Elf64_Shdr*	section = (Elf64_Shdr*)addr;

			if (i == data->st_shndx)
			{
				if ((section->sh_flags & SHF_ALLOC) != 0 && (section->sh_flags & SHF_WRITE) == 0 \
					&& (section->sh_flags & SHF_EXECINSTR) == 0)
					return (true);

				return (false);
			}
		}
	}

	if (arch == 32)
	{
		Elf32_Sym*	data = symbol->data;
		Elf32_Ehdr*	header = (Elf32_Ehdr*) binary;

		for (int i = 0, newValue = 0; i != header->e_shnum; i++)
		{
			newValue = header->e_shoff + (i * header->e_shentsize);
			if (newValue <= 0 || newValue >= len)
				{ *value = 2; return (true); }

			const void*	addr = binary + newValue;
			Elf32_Shdr*	section = (Elf32_Shdr*)addr;

			if (i == data->st_shndx)
			{
				if ((section->sh_flags & SHF_ALLOC) != 0 && (section->sh_flags & SHF_WRITE) == 0 \
					&& (section->sh_flags & SHF_EXECINSTR) == 0)
					return (true);

				return (false);
			}
		}
	}

	return (false);
}

static bool	isText(const char* binary, tSymbols* symbol, int* value, const int len, const int arch)
{
	if (arch == 64)
	{
		Elf64_Sym*	data = symbol->data;
		Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;

		if (ELF64_ST_TYPE(data->st_info) == STT_OBJECT)
			return (false);

		for (int i = 0, newValue = 0; i != header->e_shnum; i++)
		{
			newValue = header->e_shoff + (i * header->e_shentsize);
			if (newValue <= 0 || newValue >= len)
				{ *value = 2; return (true); }

			const void*	addr = binary + newValue;
			Elf64_Shdr*	section = (Elf64_Shdr*)addr;

			if (i == data->st_shndx)
			{
				if ((section->sh_flags & SHF_EXECINSTR) != 0 && (section->sh_flags & SHF_ALLOC) != 0 \
					&& section->sh_type == SHT_PROGBITS)
					return (true);

				return (false);
			}
		}
	}

	if (arch == 32)
	{
		Elf32_Sym*	data = symbol->data;
		Elf32_Ehdr*	header = (Elf32_Ehdr*) binary;

		if (ELF32_ST_TYPE(data->st_info) == STT_OBJECT)
			return (false);

		for (int i = 0, newValue = 0; i != header->e_shnum; i++)
		{
			newValue = header->e_shoff + (i * header->e_shentsize);
			if (newValue <= 0 || newValue >= len)
				{ *value = 2; return (true); }

			const void*	addr = binary + newValue;
			Elf32_Shdr*	section = (Elf32_Shdr*)addr;

			if (i == data->st_shndx)
			{
				if ((section->sh_flags & SHF_EXECINSTR) != 0 && (section->sh_flags & SHF_ALLOC) != 0 \
					&& section->sh_type == SHT_PROGBITS)
					return (true);

				return (false);
			}
		}
	}

	return (false);
}

static bool	isUniqueGlobal(tSymbols* symbol, const int arch)
{
	Elf64_Sym*	data64 = symbol->data;
	Elf32_Sym*	data32 = symbol->data;

	if (arch == 64 && ELF64_ST_BIND(data64->st_info) == STB_GNU_UNIQUE)
		return (true);

	if (arch == 32 && ELF32_ST_BIND(data32->st_info) == STB_GNU_UNIQUE)
		return (true);

	return (false);
}

static bool	isUndefined(tSymbols* symbol, const int arch)
{
	Elf64_Sym*	data64 = symbol->data;
	Elf32_Sym*	data32 = symbol->data;

	if (arch == 64 && data64->st_shndx == SHN_UNDEF \
		&& ELF64_ST_BIND(data64->st_info) == STB_GLOBAL)
		return (true);

	if (arch == 32 && data32->st_shndx == SHN_UNDEF \
		&& ELF32_ST_BIND(data32->st_info) == STB_GLOBAL)
		return (true);

	return (false);
}

static bool	isWeakNormal(tSymbols* symbol, const int arch)
{
	Elf64_Sym*	data64 = symbol->data;
	Elf32_Sym*	data32 = symbol->data;

	if (arch == 64 && ELF64_ST_TYPE(data64->st_info) == STT_OBJECT \
		&& ELF64_ST_BIND(data64->st_info) == STB_WEAK)
		return (true);

	if (arch == 32 && ELF32_ST_TYPE(data32->st_info) == STT_OBJECT \
		&& ELF32_ST_BIND(data32->st_info) == STB_WEAK)
		return (true);

	return (false);
}

static bool	isWeakUnknown(tSymbols* symbol, const int arch)
{
	Elf64_Sym*	data64 = symbol->data;
	Elf32_Sym*	data32 = symbol->data;

	if (arch == 64 && ELF64_ST_TYPE(data64->st_info) != STT_OBJECT \
		&& ELF64_ST_BIND(data64->st_info) == STB_WEAK)
		return (true);

	if (arch == 32 && ELF32_ST_TYPE(data32->st_info) != STT_OBJECT \
		&& ELF32_ST_BIND(data32->st_info) == STB_WEAK)
		return (true);

	return (false);
}

static bool	isGlobal(tSymbols* symbol, const int arch)
{
	Elf64_Sym*	data64 = symbol->data;
	Elf32_Sym*	data32 = symbol->data;

	if (arch == 64)
	{
		if (ELF64_ST_BIND(data64->st_info) == STB_GLOBAL \
			|| (ELF64_ST_BIND(data64->st_info) == STB_WEAK && data64->st_shndx != SHN_UNDEF))
			return (true);
	}

	if (arch == 32)
	{
		if (ELF32_ST_BIND(data32->st_info) == STB_GLOBAL \
			|| (ELF32_ST_BIND(data32->st_info) == STB_WEAK && data32->st_shndx != SHN_UNDEF))
			return (true);
	}

	return (false);
}

static bool isLocalOrGlobal(const char type)
{
	if (type == '?' || type == '!')
		return (false);

	if (type == 'U')
		return (false);

	if (type == 'i' || type == 'u')
		return (false);

	return (true);
}

char*	getType(const char* binary, tSymbols* symbol, int* value, const int len, const int arch)
{
	char*	type = NULL;

	type = getDup("? ");
	if (!type)
		return (NULL);

	if (isInvalid(symbol, arch) == true)
		type[0] = '!';

	else if (isAbsolute(symbol, arch) == true)
		type[0] = 'A';

	else if (isBSS(binary, symbol, value, len, arch) == true)
		type[0] = 'B';

	else if (isCommon(symbol, arch) == true)
		type[0] = 'C';

	else if (isWeakNormal(symbol, arch) == true)
		type[0] = 'V';

	else if (isReadMode(binary, symbol, value, len, arch) == true)
		type[0] = 'R';

	else if (isWeakUnknown(symbol, arch) == true)
		type[0] = 'W';

	else if (isUndefined(symbol, arch) == true)
		type[0] = 'U';

	else if (isUniqueGlobal(symbol, arch) == true)
		type[0] = 'u';

	else if (isInitialized(binary, symbol, value, len, arch) == true)
		type[0] = 'D';

	else if (isText(binary, symbol, value, len, arch) == true)
		type[0] = 'T';

	else if (isIndirect(symbol, arch) == true)
		type[0] = 'i';

	if (isGlobal(symbol, arch) == false \
		&& isLocalOrGlobal(type[0]) == true)
		type[0] += 32;

	return (type);
}
