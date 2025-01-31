#include "../../../include/header.h"

char*	getName(tSymbols* symbol, tStrs* strs, int* value, const int len, const int arch)
{
	char*	name = "NULL";

	for (int i = 0, newValue = 0; strs[i].end != true; i++)
	{
		if (strs[i].id == (*symbol).link)
		{
			if (arch == 64)
			{
				newValue = ((Elf64_Sym *)(*symbol).data)->st_name;
				if (newValue < 0 || newValue >= len)
					{ *value = 2; return (NULL); }
				name = getDup(strs[i].str + newValue);
			}
			if (arch == 32)
			{
				newValue = ((Elf32_Sym *)(*symbol).data)->st_name;
				if (newValue < 0 || newValue >= len)
					{ *value = 2; return (NULL); }
				name = getDup(strs[i].str + newValue);
			}

			break ;
		}
	}

	if (!name)
		return (NULL);

	return (name);
}
