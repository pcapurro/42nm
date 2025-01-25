#include "../../include/header.h"

void	readBinary(tInfos* infos)
{
	if (infos->errors[0] != NULL)
		writeStr(infos->errors[0], 2);
	else
	{
		for (int k = 0; ((tSymbols *)infos->binaries[0])[k].end != true; k++)
		{
			tSymbols*	symbol = (infos->binaries[0]);

			writeStr(symbol[k].address, 1);
			writeStr(symbol[k].type, 1);
			writeStr(symbol[k].name, 1);

			if (symbol[k + 1].end != true)
				writeStr("\n", 1);
		}
	}
	writeStr("\n", 1);
}

void	readBinaries(tInfos* infos)
{
	for (int i = 0; infos->paths[i] != NULL; i++)
	{
		writeStr(infos->paths[i], 1);
		writeStr(":\n", 1);

		if (infos->errors[i] != NULL)
			writeStr(infos->errors[i], 2);
		else
		{
			for (int k = 0; ((tSymbols *)infos->binaries[i])[k].end != true; k++)
			{
				tSymbols*	symbol = (infos->binaries[i]);

				writeStr(symbol[k].address, 1);
				writeStr(symbol[k].type, 1);
				writeStr(symbol[k].name, 1);

				if (symbol[k + 1].end != true)
					writeStr("\n", 1);
			}
		}
		writeStr("\n", 1);

		if (infos->paths[i + 1] != NULL)
			writeStr("\n", 1);
	}
}
