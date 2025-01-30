#include "../../include/header.h"

void	freeArray(char** array)
{
	if (array != NULL)
	{
		for (int i = 0; array[i] != NULL; i++)
			free(array[i]);
		free(array);
	}
}

void	freeBinary(void* binary)
{
	tSymbols*	symbol;

	for (int k = 0; ((tSymbols*)binary)[k].end != true; k++)
	{
		symbol = binary;

		if (symbol[k].address != NULL)
			free(symbol[k].address);
		if (symbol[k].type != NULL)
			free(symbol[k].type);
		if (symbol[k].name != NULL)
			free(symbol[k].name);
	}
	free(binary);
}

bool	isSame(const char* str1, const char* str2)
{
	if (getStrLen(str1) != getStrLen(str2))
		return (false);

	for (int i = 0; str1[i] != '\0'; i++)
	{
		if (str1[i] != str2[i])
			return (false);
	}

	return (true);
}
