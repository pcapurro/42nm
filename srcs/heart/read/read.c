#include "../../../include/header.h"

void	listSymbols(tInfos* infos)
{
	for (int i = 0; infos->paths[i] != NULL; i++)
	{
		if (getArrLen((void*)infos->paths) > 1 \
			&& (infos->errors[i] == NULL || infos->errors[i][0] != '\0'))
			writeStr(infos->paths[i], 1), writeStr(":\n", 1);

		if (infos->errors[i] != NULL)
			writeStr(infos->errors[i], 2);
		else
		{
			if (infos->noSort == false)
				orderSymbols(&infos->binaries[i]);
			if (infos->reverseSort == true && infos->noSort == false)
				reverseSymbols(&infos->binaries[i]);

			for (int k = 0; ((tSymbols *)infos->binaries[i])[k].end != true; k++)
			{
				tSymbols*	symbol = (infos->binaries[i]);

				if (infos->undefinedOnly == true && symbol[k].type[0] != 'w' \
					&& symbol[k].type[0] != 'U' && symbol[k].type[0] != 'u')
					continue ;
				if (infos->externOnly == true && symbol[k].type[0] > 96)
					continue ;

				writeStr(symbol[k].address, 1);
				writeStr(symbol[k].type, 1);
				writeStr(symbol[k].name, 1);
				writeStr("\n", 1);
			}
		}
		if (infos->paths[i + 1] != NULL)
			writeStr("\n", 1);
	}
}
