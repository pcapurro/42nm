#include "../../../include/header.h"

char*	getName(tSymbols* symbol, tStrs* strs, int* value, const int arch)
{
	char*	name = "NULL";

	for (int i = 0; strs[i].end != true; i++)
	{
		if (strs[i].id == (*symbol).link)
		{
			if (arch == 64)
				name = getDup(strs[i].str + ((Elf64_Sym *)(*symbol).data)->st_name);
			if (arch == 32)
				name = getDup(strs[i].str + ((Elf32_Sym *)(*symbol).data)->st_name);

			break ;
		}
	}

	if (!name)
		return (NULL);

	return (name);
}
