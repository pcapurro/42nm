#include "../../include/header.h"

int		getStrLen(const char* str)
{
	int len = 0;
	for (int i = 0; str != NULL && str[i] != '\0'; i++)
		len++;

	return (len);
}

int		getArrLen(void** array)
{
	int len = 0;
	for (int i = 0; array != NULL && array[i] != NULL; i++)
		len++;

	return (len);
}

char*	getJoin(const char* s1, const char* s2, const char* s3)
{
	int		i = 0;
	int		len = 0;
	char	*str = NULL;

	if (!s1 || !s2 || !s3)
		return (NULL);

	len =  getStrLen(s1) + getStrLen(s2) + getStrLen(s3);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);

	for (int k = 0; s1[k] != '\0'; k++, i++)
		str[i] = s1[k];
	for (int k = 0; s2[k] != '\0'; k++, i++)
		str[i] = s2[k];
	for (int k = 0; s3[k] != '\0'; k++, i++)
		str[i] = s3[k];
	str[len] = '\0';

	return (str);
}

char*	getDup(const char* str)
{
	int		len = 0;
	char*	string;

	for (int i = 0; str[i] != '\0'; i++)
		len++;
	string = malloc(sizeof(char) * (len + 1));
	if (!string)
		return (NULL);

	for (int i = 0; str[i] != '\0'; i++)
		string[i] = str[i];
	string[len] = '\0';

	return (string);
}

void	writeStr(const char* str, const int fd)
{
	for (int i = 0; str[i] != '\0'; i++)
		write(fd, &str[i], 1);
}
