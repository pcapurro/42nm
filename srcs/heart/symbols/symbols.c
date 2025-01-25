#include "../../../include/header.h"

void	initializeBinaryData(const char* binary, tSymbols** symbols, tStrs** strs)
{
	int	symLen = 0;
	int	strsLen = 0;

	Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;

	for (int i = 0; i != header->e_shnum; i++)
	{
		const void*	addr = binary + header->e_shoff + (i * header->e_shentsize);
		Elf64_Shdr*	section = (Elf64_Shdr*)addr;

		if (section->sh_type == SHT_SYMTAB)
			symLen += section->sh_size / section->sh_entsize;
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

void	registerBinaryData(const char* binary, tSymbols* symbols, tStrs* strs)
{
	Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;

	for (int i = 0, j = 0, k = 0; i != header->e_shnum; i++)
	{
		const void*	addr = binary + header->e_shoff + (i * header->e_shentsize);
		Elf64_Shdr*	section = (Elf64_Shdr*)addr;

		if (section->sh_type == SHT_SYMTAB)
		{
			for (int l = 0; l != section->sh_size / section->sh_entsize; l++)
			{
				symbols[k].data = (Elf64_Sym *) (binary + (section->sh_offset + (l * section->sh_entsize)));
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

void	analyze64Binary(tInfos* infos, const char* binary, const int y)
{
	tStrs*		strs = NULL;
	tSymbols*	symbols = NULL;

	initializeBinaryData(binary, &symbols, &strs);
	registerBinaryData(binary, symbols, strs);

	for (int i = 0; symbols[i].data != NULL; i++)
	{
		symbols[i].name = getName(&symbols[i], strs);
		symbols[i].type = getType(&symbols[i], strs);
		symbols[i].address = getAddress(&symbols[i], strs, 64);
	}
	infos->binaries[y] = symbols;
}

void	initializeSymbols(tInfos* infos)
{
	int	len = getArrLen(infos->paths);

	infos->binaries = malloc(sizeof(void*) * (len + 1));
	if (!infos->binaries)
		{ memoryFailed(); freeArray(infos->paths); exit(1); }
	for (int i = 0; i != len + 1; i++)
		infos->binaries[i] = NULL;

	infos->errors = malloc(sizeof(char*) * (len + 1));
	if (!infos->errors)
	{
		memoryFailed();
		freeArray(infos->paths);
		// freeArray(infos->binaries);
		exit(1);
	}
	for (int i = 0; i != len + 1; i++)
		infos->errors[i] = NULL;
}

void	getSymbols(tInfos* infos)
{
	tStat	fileInfos;

	initializeSymbols(infos);
	for (int i = 0, fd = 0; infos->paths[i] != NULL; i++)
	{
		fd = open(infos->paths[i], O_RDONLY);
		if (fd == -1 || fstat(fd, &fileInfos) < 0)
		{
			getError(infos, NULL, i);
			if (fd != -1)
				close(fd);
			continue ;
		}

		char*	binary = mmap(NULL, fileInfos.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
		if (binary == MAP_FAILED)
			{ getError(infos, NULL, i); close(fd); continue ; }
		if (isELF(binary, fileInfos.st_size) == false)
			{ getError(infos, "file format not recognized", i); close(fd); continue ; }

		if (binary[4] == 2)
			analyze64Binary(infos, binary, i);

		munmap(binary, fileInfos.st_size);
		close(fd);
	}
}

// Header ELF (Elf64_Ehdr *)
/// e_shoff -> emplacement des sections dans le fichier
/// e_shentsize -> taille d'une section
/// e_shnum -> nb de sections

// Section (Elf64_Shdr *)
/// sh_type -> type de section
/// sh_offset -> ou commence la table
/// sh_entsize -> taille d'une entrée de la table

// Si le sh_type est égal à SH_SYMTAB : c'est une table de symboles (Elf64_Sym *)
/// st_name -> index dans la table des chaines de symboles (SHT_STRTAB) contenant le nom du symbole
/// st_info -> type de symbole (4) (fonction ou variable) + liaison du symbole (4) (global ou local)
/// st_value -> addresse du symbole
/// st_link -> index de la table des chaines correspondante

// Si le sh_type est égal à SHT_STRTAB : c'est une table de chaines (char* )
/// le sh_offset de la section qui a amené à cette table pointe vers un ensemble de chaines
/// ex : "getSymbols\0getError\0..."