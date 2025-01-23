#include "../../include/header.h"

void	readBinary(tInfos* infos)
{
	if (infos->binaries != NULL && infos->binaries[0] != NULL)
		writeStr(infos->binaries[0], 1);
	else
		writeStr("'\\0'", 1);
	writeStr("\n", 1);
}

void	readBinaries(tInfos* infos)
{
	for (int i = 0; infos->paths[i] != NULL; i++)
	{
		writeStr(infos->paths[i], 1);
		writeStr(":\n", 1);

		if (infos->binaries != NULL && infos->binaries[i] != NULL)
			writeStr(infos->binaries[i], 1);
		else
			writeStr("'\\0'", 1);
		writeStr("\n", 1);

		if (infos->paths[i + 1] != NULL)
			writeStr("\n", 1);
	}
}
