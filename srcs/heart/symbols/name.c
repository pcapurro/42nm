#include "../../../include/header.h"

char*	getName(tSymbols* symbol, tStrs* strs)
{
	char*	name = "NULL";

	for (int i = 0; strs[i].str != NULL; i++)
	{
		if (strs[i].id == (*symbol).link)
			name = getDup(strs[i].str + ((Elf64_Sym *)(*symbol).data)->st_name);
	}

	return (name);
}
