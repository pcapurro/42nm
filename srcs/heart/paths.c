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
	{
		if (argv[i][0] == '-')
			continue ;
		len++;
	}

	infos->paths = malloc(sizeof(char*) * (len + 1));
	if (!infos->paths)
		memoryFailed(), exit(1);
	infos->paths[len] = NULL;

	if (len == 0)
		getPath(infos);

	for (int i = 0, j = 0; len > 0 && argv[i] != NULL; i++)
	{
		if (argv[i][0] == '\0' || argv[i][0] == '-')
			continue ;

		infos->paths[j] = getDup(argv[i]);
		if (!infos->paths[j])
		{
			memoryFailed();
			j--;
			while (j != -1)
				free(infos->paths[j]), j--;
			free(infos->paths);
			exit(1);
		}
		j++;
	}
}
