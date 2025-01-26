#include "../../../include/header.h"

int		getTopOrdered(tSymbols* array, const int len)
{
	int	saved = '\0';
	int	count = 0;

	for (int value = 255, k = 0; count != 1; value = 255, k++)
	{
		count = 0;
		for (int i = 0; i != len; i++)
		{
			if (array[i].name != NULL && array[i].name[k] < value)
				value = array[i].name[k];
		}
		for (int i = 0; i != len; i++)
		{
			if (array[i].name != NULL && array[i].name[k] == value)
				count++, saved = i;

			if (array[i].name != NULL && array[i].name[k] != value)
				array[i].name = NULL;
		}

		if (value == '\0')
			break ;
	}

	return (saved);
}

void	orderSymbols(void** array)
{
	int			len = 0;
	tSymbols*	newArray = NULL;
	tSymbols*	copyArray = NULL;

	for (int i = 0; ((tSymbols *)(*array))[i].end != true; i++)
		len++;

	newArray = malloc(sizeof(tSymbols) * (len + 1));
	if (!newArray)
		memoryFailed(), exit(1);
	newArray[len].end = true;

	copyArray = malloc(sizeof(tSymbols) * (len + 1));
	if (!copyArray)
		memoryFailed(), exit(1);
	copyArray[len].end = true;

	int	value = 0;
	for (int i = 0, j = 0; ((tSymbols *)(*array))[i].end != true; i++, j++)
	{
		for (int k = 0; ((tSymbols *)(*array))[k].end != true; k++)
		{
			copyArray[k].address = ((tSymbols *)(*array))[k].address;
			copyArray[k].type = ((tSymbols *)(*array))[k].type;
			copyArray[k].name = ((tSymbols *)(*array))[k].name;
			copyArray[k].end = false;
		}
		value = getTopOrdered(copyArray, len);
		newArray[j].address = ((tSymbols *)(*array))[value].address;
		newArray[j].type = ((tSymbols *)(*array))[value].type;
		newArray[j].name = ((tSymbols *)(*array))[value].name;
		newArray[j].end = false;
		((tSymbols *)(*array))[value].name = NULL;
	}

	free(copyArray);
	free(*array);
	(*array) = newArray;
}

void	reverseSymbols(void** array)
{
	int			len = 0;
	tSymbols*	newArray = NULL;

	for (int i = 0; ((tSymbols *)(*array))[i].end != true; i++)
		len++;

	newArray = malloc(sizeof(tSymbols) * (len + 1));
	if (!newArray)
		memoryFailed(), exit(1);
	newArray[len].end = true;

	for (int i = 0, k = len - 1; ((tSymbols *)(*array))[i].end != true; i++, k--)
	{
		newArray[k].address = ((tSymbols *)(*array))[i].address;
		newArray[k].type = ((tSymbols *)(*array))[i].type;
		newArray[k].name = ((tSymbols *)(*array))[i].name;
	}

	free(*array);
	(*array) = newArray;
}
