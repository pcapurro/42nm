#include "../../include/header.h"

void	getError(tInfos* infos, const int i)
{
	char*	error = NULL;
	char*	str = NULL;

	error = strerror(errno);
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

void	analyzeBinary(tInfos* infos, const char* binary, const int i)
{
	;
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
			getError(infos, i);
			if (fd != -1)
				close(fd);
			continue ;
		}

		binary = mmap(NULL, fileInfos.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
		if (binary == MAP_FAILED)
			{ getError(infos, i); close(fd); continue ; }

		analyzeBinary(infos, binary, i);

		munmap(binary, fileInfos.st_size);
		close(fd);
	}
}
