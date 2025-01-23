#include "../../include/header.h"

void	getPath(tInfos* infos)
{
	infos->paths[1] = NULL;

	infos->paths[0] = getDup("a.out");
	if (!infos->paths[0])
		free(infos->paths), exit(1);
}

void	getPaths(tInfos* infos, const char** argv)
{
	int len = 0;

	for (int i = 0; argv[i] != NULL; i++)
		len++;

	infos->paths = malloc(sizeof(char*) * len + 1);
	if (!infos->paths)
		memoryFailed(), exit(1);
	infos->paths[len] = NULL;

	if (len == 0)
		getPath(infos);

	for (int i = 0; len > 0 && argv[i] != NULL; i++)
	{
		infos->paths[i] = getDup(argv[i]);
		if (!infos->paths[i])
		{
			memoryFailed();
			i--;
			while (i != -1)
				free(infos->paths[i]), i--;
			free(infos->paths);
			exit(1);
		}
	}
}
