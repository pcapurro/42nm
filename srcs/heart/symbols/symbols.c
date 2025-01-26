#include "../../../include/header.h"

void	getError(tInfos* infos, const char* message, const int i)
{
	const char*	error = NULL;
	char*		str = NULL;
	char*		msg = NULL;

	if (message == NULL)
		error = strerror(errno);
	else
		error = message;

	str = getJoin("ft_nm: '", infos->paths[i], "': ");
	if (!str)
		memoryFailed(), exit(1);
	msg = getJoin(str, error, "\0");
	if (!msg)
		memoryFailed(), exit(1);
	free(str);

	infos->errors[i] = msg;
}

bool	isELF(const char* binary, const long int len)
{
	if (len < 21)
		return (false);

	if (binary[1] == 'E' && binary[2] == 'L' && binary[3] == 'F' \
		&& binary[0] == 0x7F)
		return (true);

	return (false);
}

void	initializeSymbols(tInfos* infos)
{
	int	len = getArrLen((void*)infos->paths);

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
		freeBinaries(infos->binaries);
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
			analyzeBinary64(infos, binary, i);
		else
			analyzeBinary32(infos, binary, i);

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