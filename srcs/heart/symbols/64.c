#include "../../../include/header.h"

int	initializeBinary64(const char* binary, tSymbols** symbols, tStrs** strs)
{
	int	symLen = 0;
	int	strsLen = 0;

	Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;

	for (int i = 0; i != header->e_shnum; i++)
	{
		const void*	addr = binary + header->e_shoff + (i * header->e_shentsize);
		Elf64_Shdr*	section = (Elf64_Shdr*)addr;

		if (section->sh_type == SHT_SYMTAB)
			symLen += (section->sh_size - 1) / section->sh_entsize;
		if (section->sh_type == SHT_STRTAB)
			strsLen++;
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

int		registerBinary64(const char* binary, tSymbols* symbols, tStrs* strs)
{
	Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;

	for (int i = 0, j = 0, k = 0; i != header->e_shnum; i++)
	{
		const void*	addr = binary + header->e_shoff + (i * header->e_shentsize);
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

	return (0);
}
