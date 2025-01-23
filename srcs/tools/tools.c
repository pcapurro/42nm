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
