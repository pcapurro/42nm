#include "../../../include/header.h"

void	getError(tInfos* infos, const char* message, const int i)
{
	const char*	error = NULL;
	char*		str = NULL;

	if (message == NULL)
		error = strerror(errno);
	else
		error = message;

	str = getJoin("ft_nm: '", infos->paths[i], "': ");
	if (!str)
	{
		memoryFailed();
		freeArray(infos->paths);
		freeArray(infos->binaries);
		exit(1);
	}
	infos->binaries[i] = getJoin(str, error, "\0");
	free(str);
}

char*	getName(tSymbols* symbol, tStrs* strs)
{
	char*	name = "NULL";

	for (int i = 0; strs[i].str != NULL; i++)
	{
		if (strs[i].id == (*symbol).link)
			name = strs[i].str + (*symbol).data->st_name;
	}

	return (name);
}

char*	getAddress(tSymbols* symbols, tStrs* strs)
{
	return ("000000000000290c ");
}

char*	getType(tSymbols* symbols, tStrs* strs)
{
	return ("T ");
}

// Header ELF (Elf64_Ehdr *)
/// e_shoff -> emplacement des sections dans le fichier
/// e_shentsize -> taille d'une section
/// e_shnum -> nb de sections

// Section (Elf64_Shdr *)
/// sh_type -> type de section
/// sh_offset -> ou commence la table
/// sh_entsize -> taille d'une entrée de la table

// Si le sh_type est égal à SH_SYMTAB : c'est une table de symboles (Elf64_Sym *)
/// st_name -> index dans la table des chaines de symboles (SHT_STRTAB) contenant le nom du symbole
/// st_info -> type de symbole (4) (fonction ou variable) + liaison du symbole (4) (global ou local)
/// st_value -> addresse du symbole
/// st_link -> index de la table des chaines correspondante

// Si le sh_type est égal à SHT_STRTAB : c'est une table de chaines (char* )
/// le sh_offset de la section qui a amené à cette table pointe vers un ensemble de chaines
/// ex : "getSymbols\0getError\0..."