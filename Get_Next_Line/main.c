#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

char	*get_next_line(int fd);

int main(void)
{
	int	fd1;
	char *linha1;
	char *linha2;
	int	fd2;

	fd1 = open("teste1.txt", O_RDONLY);
	
	fd2 = open("teste2.txt", O_RDONLY);
	//fd2 = open("teste3.txt", O_RDONLY);
	
	printf("%d\n%d\n", fd1, fd2);
	linha1 = get_next_line(fd1);
	printf("%s", linha1);
	free(linha1);
	linha2 = get_next_line(fd2);
	printf("%s", linha2);
	free(linha2);
	linha1 = get_next_line(fd1);
	printf("%s", linha1);
	free(linha1);
	linha2 = get_next_line(fd2);
	printf("%s", linha2);
	free(linha2);
	linha1 = get_next_line(fd1);
	printf("%s", linha1);
	free(linha1);
	linha2 = get_next_line(fd2);
	printf("%s", linha2);
	free(linha2);

	/*while ((linha1 = get_next_line(0)) != NULL){
		printf("%s", linha1);
		free(linha1);
	}*/

	return (0);
}
