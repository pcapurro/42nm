#include "../../../include/header.h"

bool	isELF(const char* binary, const long int len)
{
	if (len < 21)
		return (false);

	if (binary[1] == 'E' && binary[2] == 'L' && binary[3] == 'F' \
		&& binary[0] == 0x7F)
		return (true);

	return (false);
}
