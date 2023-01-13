#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

/*
clear && cc get_next_line.c get_next_line_utils.c maintest.c -D BUFFER_SIZE=11 && ./a.out -1

bonus:
clear && cc get_next_line_bonus.c get_next_line_utils_bonus.c maintest.c -D BUFFER_SIZE=11 && ./a.out 2
*/


int main (int ac,char **av)
{

	char *file1 = "text1.txt";//"apodat.txt";
	char *file2 = "text1.txt";

	if (ac!= 2)
	{
		printf("Nombre d'arguments de ./a.out incorrects\n");
		return (0);
	}
	int choix;

	choix = atoi(av[1]); 	//-1 pour index incorrect, 
				//0 pour stdinput; 
				//1 pour un unique fichier (4 lignes),
				//2 pour un unique fichier entier,
				//3 pour deux fichiers 3 lignes intercallees;

	if (choix == -1)
	{
		//printf("\e[0;103mCas avec lecture de fichier incorrect : \n\e[0m");
		int fd;
		char *str;

		fd = -1;
		str = get_next_line(fd);
		printf("\e[1m{%s}\e[0m", str);
		free(str);
		return (0);
	}
	else if (choix == 0)
	{
		int fd;
		char *str;

		fd = 0;
		printf("\e[0;103mCas avec lecture de stdin : \n\e[0m");
		printf("Entrer ci-dessous la ligne a recopier :\n");
		str = get_next_line(fd);
		printf("\e[1m{%s}\e[0m", str);
		free(str);
		return (0);
	}
	else if (choix == 1 || choix == 2)
	{
		int	fd;;
		char *str;

		fd = open(file1, O_RDONLY);//"/dev/random"
		if (fd < 0)
		{
			printf("erreur fd\n");
			return (0);
		}
		if (choix ==1)//trois premieres lignes
		{
			printf("\e[0;103mCas avec lecture des trois premieres lignes du fiher : \n\e[0m");
			printf("\nLigne 1 :\n");
			str = get_next_line(fd);
			printf("\e[1m{%s}\e[0m", str);
			free(str);

			printf("\n\n**************************\nLigne 2 :\n");
			str = get_next_line(fd);
			printf("\e[1m{%s}\e[0m", str);
			free (str);

			printf("\n\n**************************\nLigne 3 :\n");
			str = get_next_line(fd);
			printf("\e[1m{%s}\e[0m", str);
			free(str);

			printf("\n\n**************************\nLigne 4 :\n");
			str = get_next_line(fd);
			printf("\e[1m{%s}\e[0m", str);
			free(str);
		}
		else //tout le fichier
		{
			printf("\e[0;103mCas avec lecture du fichier entier : \n\e[0m{");
			str = get_next_line(fd);
			while (str)
			{
				printf("%s", str);
				free(str);
				str = get_next_line(fd);
			}
			printf("}\n\n\e[0;103m/!\\Dans ce cas il est probable que le debut n'apparaisse par (affichage terminal), le visaliser dans fichier txt.\e[0m\n \n");
		}
		if (fd)
			close(fd);
	}
	else if (choix == 3) //deux fichiers
	{
		int	fd1;
		int fd2;
		char *str;

		printf("\e[0;101mBONUS : compiler avec \nclear && cc get_next_line_bonus.c get_next_line_utils_bonus.c maintest.c -D BUFFER_SIZE=11 && ./a.out 2\e[0m\n\n");
		printf("\e[0;103mCas avec lecture des lectures alternees des 3 premieres lignes de 2 fichiers : \n\e[0m");
		fd1 = open(file1, O_RDONLY);
		fd2 = open(file2, O_RDONLY);
		if (fd1 < 0 || fd2 < 0)
		{
			printf("Erreur dans un des deux fd\n");
			return (0);
		}
		printf("\nLigne 1 :\n\n");
		str = get_next_line(fd1);
		printf("\e[0;3m{%s}\e[0m\n", str);
		free(str);
		str = get_next_line(fd2);
		printf("\e[0;2m{%s}\e[0m\n", str);
		free(str);

		printf("\n\n**************************\nLigne 2 :\n\n");
		str = get_next_line(fd1);
		printf("\e[0;3m{%s}\e[0m\n", str);
		free(str);
		str = get_next_line(fd2);
		printf("\e[0;2m{%s}\e[0m\n", str);
		free(str);

		printf("\n\n**************************\nLigne 3 :\n\n");
		str = get_next_line(fd1);
		printf("\e[0;3m{%s}\e[0m\n", str);
		free(str);
		str = get_next_line(fd2);
		printf("\e[0;2m{%s}\e[0m\n", str);
		free(str);

		printf("\n\n**************************\nLigne 4 :\n\n");
		str = get_next_line(fd1);
		printf("\e[0;3m{%s}\e[0m\n", str);
		free(str);
		str = get_next_line(fd2);
		printf("\e[0;2m{%s}\e[0m\n", str);
		free(str);

		close(fd1);
		close(fd2);
	}
	return 0;
}
