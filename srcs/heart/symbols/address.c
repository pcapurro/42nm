#include "../../../include/header.h"

char*	getAddress(tSymbols* symbol, tStrs* strs, const int value)
{
	int		len = 0;
	int		number = 0;
	char*	str = NULL;

	if (value == 32)
		number = ((Elf32_Sym *)(*symbol).data)->st_value, len = 8;
	if (value == 64)
		number = ((Elf64_Sym *)(*symbol).data)->st_value, len = 16;

	str = malloc(sizeof(char) * (len + 2));
	if (!str)
		return (NULL);

	for (int i = 0; i != len; i++)
		str[i] = '0';
	str[len] = ' ';
	str[len + 1] = '\0';

	if (number <= 0)
	{
		if (((Elf64_Sym *)(*symbol).data)->st_shndx == SHN_UNDEF \
			|| ((Elf64_Sym *)(*symbol).data)->st_info >> 4 == STB_WEAK)
		{
			for (int i = 0; i != len; i++)
				str[i] = ' ';
		}

		return (str);
	}

	int k = len - 1;
	while (k != -1 && number > 9)
	{
		if (number % 16 > 9)
			str[k] = "0123456789abcdef"[number % 16];
		else
			str[k] = (number % 16) + 48;
		number = number / 16;
		k--;
	}
	str[k] = (number % 16) + 48;

	return (str);
}
