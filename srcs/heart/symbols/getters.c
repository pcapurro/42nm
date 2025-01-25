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
	str = getJoin(str, error, "\0");

	infos->errors[i] = str;
}

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

	str[len + 1] = '\0';

	if (number <= 0)
	{
		for (int i = 0; i != len + 1; i++)
			str[i] = ' ';
		return (str);
	}

	for (int i = 0; i != len; i++)
		str[i] = '0';
	str[len] = ' ';

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

char*	getType(tSymbols* symbol, tStrs* strs)
{
	return ("C ");
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