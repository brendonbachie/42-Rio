#include "get_next_line.h"
#include <fcntl.h>

int main(void)
{
	int     fd;
	char    *linha;

	fd = open("teste1.txt", O_RDONLY);
	if (fd < 0)
		return (1);

	while ((linha = get_next_line(fd)) != NULL)
	{
		printf("%s", linha);
		free(linha);
	}

	close(fd);
	return (0);
}
