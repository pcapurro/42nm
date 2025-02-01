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

	str = getJoin("42nm: '", infos->paths[i], "': ");
	if (!str)
		return (1);
	msg = getJoin(str, error, "\n");
	if (!msg)
		{ free(str); return (1); }
	free(str);

	infos->returnValue++;
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
	
	if (binary[4] != 0x01 && binary[4] != 0x02)
		return (false);

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
			value = getError(infos, "file format not recognized", i);

		if (value == 0)
		{
			if (infos->binary[4] == 0x02)
				value = analyzeBinary(infos, i, 64);
			if (infos->binary[4] == 0x01)
				value = analyzeBinary(infos, i, 32);

			if (value == 1)
				value = getError(infos, "no symbols", i);
			if (value == 2)
				value = getError(infos, "corrupted or invalid symbols", i);
		}

		munmap(infos->binary, fileInfos.st_size);
		close(infos->fd);
	}
	infos->fd = -1;
	infos->binary = NULL;
}

// ELF binary structure:

// Header ELF (Elf64/32_Ehdr *)
/// e_shoff -> offset of sections
/// e_shentsize -> size of a section
/// e_shnum -> number of sections

// Section (Elf64/32_Shdr *)
/// sh_type -> type section
/// sh_offset -> offset table
/// sh_entsize -> size of the table

// Table with a sh_type of SH_SYMTAB > table of symbols (Elf64/32_Sym *)
/// st_name -> index in the string table of symbols (SHT_STRTAB) containing the name of the symbol
/// st_info -> symbol type (function or variable) + symbol bind (global or local)
/// st_value -> symbol addr
/// st_link -> index in the string table of symbols

// If the sh_type is SHT_STRTAB : table of arrays (char*)
/// the st_name leads in a specific string
/// the sh_offset of a section leads somewhere in the specific string