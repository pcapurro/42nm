#include "../../../include/header.h"

void	orderSymbols(void** array)
{
	int		len = getArrLen(*array);
	void*	newArray = NULL;
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
		((tSymbols *)newArray)[k].address = ((tSymbols *)(*array))[i].address;
		((tSymbols *)newArray)[k].type = ((tSymbols *)(*array))[i].type;
		((tSymbols *)newArray)[k].name = ((tSymbols *)(*array))[i].name;
	}

	free(*array);
	(*array) = newArray;
}
