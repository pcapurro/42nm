#include "../../../include/header.h"

int	getError(tInfos* infos, const char* message, const int i)
{
	const char*	error = NULL;
	char*		str = NULL;
	char*		msg = NULL;

	if (message == NULL)
		error = strerror(errno);
	else
		error = message;

	if (isSame(error, "Success") == true)
	{
		msg = getDup("\0");
		if (!msg)
			return (1);
		infos->errors[i] = msg;

		return (0);
	}

	str = getJoin("ft_nm: '", infos->paths[i], "': ");
	if (!str)
		return (1);
	msg = getJoin(str, error, "\n");
	if (!msg)
		{ free(str); return (1); }
	free(str);

	infos->errors[i] = msg;

	return (0);
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
		memoryFailed(), setToNull(infos), exit(1);
	for (int i = 0; i != len + 1; i++)
		infos->binaries[i] = NULL;

	infos->errors = malloc(sizeof(char*) * (len + 1));
	if (!infos->errors)
		memoryFailed(), setToNull(infos), exit(1);

	for (int i = 0; i != len + 1; i++)
		infos->errors[i] = NULL;
}

void	getSymbols(tInfos* infos)
{
	tStat	fileInfos;

	initializeSymbols(infos);
	for (int i = 0, value = 0; infos->paths[i] != NULL; i++, infos->fd = -1, value = 0)
	{
		if (value != 0)
			memoryFailed(), setToNull(infos), exit(1);

		infos->fd = open(infos->paths[i], O_RDONLY);
		if (infos->fd == -1 || fstat(infos->fd, &fileInfos) < 0 || fileInfos.st_size == 0)
		{
			value = getError(infos, NULL, i);
			if (infos->fd != -1)
				close(infos->fd);
			continue ;
		}
		else
			infos->binaryLen = fileInfos.st_size;

		infos->binary = mmap(NULL, infos->binaryLen, PROT_READ, MAP_PRIVATE, infos->fd, 0);
		if (infos->binary == MAP_FAILED)
			{ value = getError(infos, NULL, i); close(infos->fd); continue ; }
		if (isELF(infos->binary, infos->binaryLen) == false)
			{ value = getError(infos, "file format not recognized", i); close(infos->fd); continue ; }

		if (infos->binary[4] == 2)
			analyzeBinary64(infos, i);
		else
			analyzeBinary32(infos, i);

		munmap(infos->binary, fileInfos.st_size);
		close(infos->fd);
	}
	infos->fd = -1;
	infos->binary = NULL;
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