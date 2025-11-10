#include "mini.h"

void	error_exit(const char *error)
{
	write(2, &error, ft_strlen(error));
	exit(EXIT_FAILURE);
}
