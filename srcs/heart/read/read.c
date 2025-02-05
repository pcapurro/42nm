#include "../../../include/header.h"

static void	writeLine(const char* address, const char* type, const char* name)
{
	writeStr(address, 1);
	writeStr(type, 1);
	writeStr(name, 1);

	writeStr("\n", 1);
}

static bool	isValid(const char type)
{
	if (type == '!')
		return (false);

	if (type == 'a' || type == 'A')
		return (false);

	return (true);
}

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
			if (infos->noSort == false && orderSymbols(&infos->binaries[i]) == NULL)
				memoryFailed(), setToNull(infos), exit(1);
			if (infos->reverseSort == true && infos->noSort == false \
				&& reverseSymbols(&infos->binaries[i]) == NULL)
				memoryFailed(), setToNull(infos), exit(1);

			for (int k = 0; ((tSymbols *)infos->binaries[i])[k].end != true; k++)
			{
				tSymbols*	symbol = (infos->binaries[i]);

				if (isValid(symbol[k].type[0]) == true)
					writeLine(symbol[k].address, symbol[k].type, symbol[k].name);
			}
		}
		if (infos->paths[i + 1] != NULL)
			writeStr("\n", 1);
	}
}
