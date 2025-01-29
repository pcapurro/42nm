#include "../../include/header.h"

void	memoryFailed(void)
{
	writeStr(MEMORY_ERROR, 2);
}

void	systemFailed(void)
{
	writeStr(SYSTEM_FAILED, 2);
}
