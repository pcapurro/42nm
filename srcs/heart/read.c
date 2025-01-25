#include "../../include/header.h"

void	readBinary(tInfos* infos)
{
	if (infos->binaries != NULL && infos->binaries[0] != NULL)
	{
		for (int k = 0; ((tSymbols *)infos->binaries[0])[k].name != NULL; k++)
		{
			writeStr(((tSymbols *)infos->binaries[0])[k].address, 1);
			writeStr(((tSymbols *)infos->binaries[0])[k].type, 1);
			writeStr(((tSymbols *)infos->binaries[0])[k].name, 1);

			if (((tSymbols *)infos->binaries[0])[k + 1].name != NULL)
				writeStr("\n", 1);
		}
	}
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
		{
			for (int k = 0; ((tSymbols *)infos->binaries[i])[k].name != NULL; k++)
			{
				writeStr(((tSymbols *)infos->binaries[i])[k].address, 1);
				writeStr(((tSymbols *)infos->binaries[0])[k].type, 1);
				writeStr(((tSymbols *)infos->binaries[i])[k].name, 1);

				if (((tSymbols *)infos->binaries[i])[k + 1].name != NULL)
					writeStr("\n", 1);
			}
		}
		else
			writeStr("'\\0'", 1);
		writeStr("\n", 1);

		if (infos->paths[i + 1] != NULL)
			writeStr("\n", 1);
	}
}
