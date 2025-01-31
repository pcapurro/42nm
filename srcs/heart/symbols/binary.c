#include "../../../include/header.h"

int	initializeBinary(const char* binary, tSymbols** symbols, tStrs** strs, const int arch)
{
	int	symLen = 0;
	int	strsLen = 0;

	Elf64_Ehdr*	header64 = (Elf64_Ehdr*) binary;
	Elf32_Ehdr*	header32 = (Elf32_Ehdr*) binary;

	if (arch == 64)
	{
		for (int i = 0; i != header64->e_shnum; i++)
		{
			const void*	addr = binary + header64->e_shoff + (i * header64->e_shentsize);
			Elf64_Shdr*	section = (Elf64_Shdr*)addr;

			if (section->sh_type == SHT_SYMTAB)
				symLen += (section->sh_size - 1) / section->sh_entsize;
			if (section->sh_type == SHT_STRTAB)
				strsLen++;
		}
	}

	if (arch == 32)
	{
		for (int i = 0; i != header32->e_shnum; i++)
		{
			const void*	addr = binary + header32->e_shoff + (i * header32->e_shentsize);
			Elf64_Shdr*	section = (Elf64_Shdr*)addr;

			if (section->sh_type == SHT_SYMTAB)
				symLen += (section->sh_size - 1) / section->sh_entsize;
			if (section->sh_type == SHT_STRTAB)
				strsLen++;
		}
	}


	if (symLen == 0)
		return (1);

	(*symbols) = malloc(sizeof(tSymbols) * (symLen + 1));
	if (!(*symbols))
		return (-1);

	for (int i = 0; i != symLen + 1; i++)
	{
		(*symbols)[i].data = NULL;
		(*symbols)[i].link = 0;

		(*symbols)[i].address = NULL;
		(*symbols)[i].type = NULL;
		(*symbols)[i].name = NULL;
		(*symbols)[i].end = false;
	}
	(*symbols)[symLen].end = true;

	(*strs) = malloc(sizeof(tStrs) * (strsLen + 1));
	if (!(*strs))
		{ free(*symbols); return (-1); }

	for (int i = 0; i != strsLen + 1; i++)
	{
		(*strs)[i].str = NULL;
		(*strs)[i].id = 0;
		(*strs)[i].end = false;
	}
	(*strs)[strsLen].end = true;

	return (0);
}

int		registerBinary(const char* binary, tSymbols* symbols, tStrs* strs, const int arch)
{
	Elf64_Ehdr*	header64 = (Elf64_Ehdr*) binary;
	Elf32_Ehdr*	header32 = (Elf32_Ehdr*) binary;

	if (arch == 64)
	{
		for (int i = 0, j = 0, k = 0; i != header64->e_shnum; i++)
		{
			const void*	addr = binary + header64->e_shoff + (i * header64->e_shentsize);
			Elf64_Shdr*	section = (Elf64_Shdr*)addr;

			if (section->sh_type == SHT_SYMTAB)
			{
				for (int l = 1; l != section->sh_size / section->sh_entsize; l++)
				{
					symbols[k].data = (Elf64_Sym *) (binary + section->sh_offset + l * section->sh_entsize);
					symbols[k].link = section->sh_link;
					k++;
				}
			}
			if (section->sh_type == SHT_STRTAB)
			{
				strs[j].id = i;
				strs[j].str = ((char*) binary) + section->sh_offset;
				j++;
			}
		}
	}

	if (arch == 32)
	{
		for (int i = 0, j = 0, k = 0; i != header32->e_shnum; i++)
		{
			const void*	addr = binary + header32->e_shoff + (i * header32->e_shentsize);
			Elf32_Shdr*	section = (Elf32_Shdr*)addr;

			if (section->sh_type == SHT_SYMTAB)
			{
				for (int l = 1; l != section->sh_size / section->sh_entsize; l++)
				{
					symbols[k].data = (Elf32_Sym *) (binary + section->sh_offset + l * section->sh_entsize);
					symbols[k].link = section->sh_link;
					k++;
				}
			}
			if (section->sh_type == SHT_STRTAB)
			{
				strs[j].id = i;
				strs[j].str = ((char*) binary) + section->sh_offset;
				j++;
			}
		}
	}

	return (0);
}

int	analyzeBinary(tInfos* infos, const int y, const int arch)
{
	int			value = 0;
	tStrs*		strs = NULL;
	tSymbols*	symbols = NULL;

	value = initializeBinary(infos->binary, &symbols, &strs, arch);

	if (value == -1)
		memoryFailed(), setToNull(infos), exit(1);
	if (value == 1 || value == 2)
		{ free(strs); return (value); }

	value = registerBinary(infos->binary, symbols, strs, arch);
	infos->binaries[y] = symbols;

	if (value == 2)
		{ free(strs); return (value); }

	for (int i = 0; symbols[i].end != true; i++)
	{
		symbols[i].name = getName(&symbols[i], strs, &value, arch);
		symbols[i].address = getAddress(&symbols[i], strs, arch);
		symbols[i].type = getType(infos->binary, &symbols[i], &value, arch);

		if (value != 0)
			{ free(strs); return (2); }

		if (!symbols[i].name || !symbols[i].type || !symbols[i].address)
		{
			memoryFailed();
			free(strs);
			setToNull(infos);
			exit(1);
		}
	}
	free(strs);

	return (0);
}
