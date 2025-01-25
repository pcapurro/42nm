#include "../../../include/header.h"

void	initializeBinary64(const char* binary, tSymbols** symbols, tStrs** strs)
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

	(*symbols) = malloc(sizeof(tSymbols) * (symLen + 1));
	if (!(*symbols))
		memoryFailed(), exit(1);
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

	(*strs) = malloc(sizeof(tStrs) * strsLen);
	if (!(*strs))
		memoryFailed(), exit(1);
	for (int i = 0; i != strsLen; i++)
	{
		(*strs)[i].str = NULL;
		(*strs)[i].id = 0;
	}
}

void	registerBinary64(const char* binary, tSymbols* symbols, tStrs* strs)
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
}

void	analyzeBinary64(tInfos* infos, const char* binary, const int y)
{
	tStrs*		strs = NULL;
	tSymbols*	symbols = NULL;

	initializeBinary64(binary, &symbols, &strs);
	registerBinary64(binary, symbols, strs);

	for (int i = 0; symbols[i].data != NULL; i++)
	{
		symbols[i].name = getName(&symbols[i], strs);
		symbols[i].type = getType(binary, &symbols[i], strs);
		symbols[i].address = getAddress(&symbols[i], strs, 64);
	}
	infos->binaries[y] = symbols;
}
