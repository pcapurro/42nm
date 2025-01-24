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

void	analyzeBinary(tInfos* infos, const char* binary, const long int len, const int i)
{
	// for (int i = 0; i != len; i++)
	// 	printf("'%c'", binary[i]);
	// printf("\n");
}

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

		analyzeBinary(infos, binary, fileInfos.st_size, i);

		munmap(binary, fileInfos.st_size);
		close(fd);
	}
}
