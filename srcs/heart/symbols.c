#include "../../include/header.h"

void	getError(tInfos* infos, const char* message, const int i)
{
	const char*	error = NULL;
	char*		str = NULL;

	if (message == NULL)
		error = strerror(errno);
	else
		error = message;

	str = getJoin("ft_nm: '", infos->paths[i], "': ");
	if (!str)
	{
		memoryFailed();
		freeArray(infos->paths);
		freeArray(infos->binaries);
		exit(1);
	}
	infos->binaries[i] = getJoin(str, error, "\0");
	free(str);
}

void	analyze64Binary(tInfos* infos, const char* binary, const long int len, const int i)
{
	char**		strs = NULL;
	Elf64_Ehdr*	header = (Elf64_Ehdr*) binary;
	Elf64_Sym**	symbols = NULL;

	int	symLen = 0;
	int	strsLen = 0;

	for (int i = 0; i != header->e_shnum; i++)
	{
		const void*	addr = binary + header->e_shoff + (i * header->e_shentsize);
		Elf64_Shdr*	section = (Elf64_Shdr*)addr;

		if (section->sh_type == SHT_SYMTAB)
			symLen += section->sh_size / section->sh_entsize;
		if (section->sh_type == SHT_STRTAB)
			strsLen++;
	}

	symbols = malloc(sizeof(Elf64_Sym*) * (symLen + 1));
	for (int i = 0; i != symLen + 1; i++)
		symbols[i] = NULL;

	strs = malloc(sizeof(char*) * (strsLen + 1));
	for (int i = 0; i != strsLen + 1; i++)
		strs[i] = NULL;

	for (int i = 0, k = 0, j = 0; i != header->e_shnum; i++)
	{
		const void*	addr = binary + header->e_shoff + (i * header->e_shentsize);
		Elf64_Shdr*	section = (Elf64_Shdr*)addr;

		if (section->sh_type == SHT_SYMTAB)
		{
			int	size = section->sh_size / section->sh_entsize;
			for (int o = 0; o != size; o++, k++)
				symbols[k] = (Elf64_Sym *) section->sh_offset + (o * section->sh_entsize);
		}
		if (section->sh_type == SHT_STRTAB)
			strs[j] = (char*) binary + section->sh_offset, j++;
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

bool	isELF(const char* binary, const long int len)
{
	if (len < 4)
		return (false);

	if (binary[1] == 'E' && binary[2] == 'L' && binary[3] == 'F' \
		&& binary[0] == 0x7F)
		return (true);

	return (false);
}

void	getSymbols(tInfos* infos)
{
	int		len = getArrLen(infos->paths);
	char*	binary = NULL;
	tStat	fileInfos;

	infos->binaries = malloc(sizeof(char*) * (len + 1));
	if (!infos->binaries)
		{ memoryFailed(); freeArray(infos->paths); exit(1); }
	for (int i = 0; i != len + 1; i++)
		infos->binaries[i] = NULL;

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

		binary = mmap(NULL, fileInfos.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
		if (binary == MAP_FAILED)
			{ getError(infos, NULL, i); close(fd); continue ; }

		if (isELF(binary, fileInfos.st_size) == false)
			{ getError(infos, "file format not recognized", i); close(fd); continue ; }

		if (binary[4] == 2)
			analyze64Binary(infos, binary, fileInfos.st_size, i);

		munmap(binary, fileInfos.st_size);
		close(fd);
	}
}
