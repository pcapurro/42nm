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
