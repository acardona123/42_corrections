/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_wrong_maps.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acardona <acardona@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 19:43:26 by acardona          #+#    #+#             */
/*   Updated: 2024/01/23 16:47:27 by acardona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
cc test_wrong_maps.c -o test_wrong_maps.exe
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

static bool		read_map_dir(char *exec_path, char *maps_dir, char **env);
static int		_child_exec_cub(char *exec_path, char *maps_dir,
					char *map_name, char **env);
static size_t	_ft_strlen(const char *str);
static char		*_ft_strjoin3(const char *s1, const char *s2, const char *s3);
static void		_test_maps_file_without_rights(char *exec_path, char **env);
static void		_test_repo_without_rights(char *exec_path, char **env);

int	main(int ac, char **av, char **env)
{
	DIR				*dir;
	int				fd_no_access_file;

	if (ac != 3 || !strstr(av[1], "cub3D")
		|| strlen(strstr(av[1], "cub3D")) != strlen("cub3D"))
		return (printf("Wrong number of arguments, expected format:\n\
./test_wrong_maps {path to the cub3D executable} \
{path to the invalid map directory}\n"), 1);
	if (strlen(av[2]) > 1 && av[2][strlen(av[2]) - 1] == '/')
		av[2][strlen(av[2]) - 1] = 0;
	read_map_dir(av[1], av[2], env);
	_test_maps_file_without_rights(av[1], env);
	_test_repo_without_rights(av[1], env);
	return (0);
}

static bool	read_map_dir(char *exec_path, char *maps_dir, char **env)
{
	DIR				*dir;
	DIR				*subdir;
	struct dirent	*elem;
	pid_t			pid;
	int				child_rtn;
	char			*elem_name;
	bool			rtn;

	printf("\n\e[105m==== dir: %s ====\e[0m\n", maps_dir);
	dir = opendir(maps_dir);
	if (!dir)
		return (fprintf(stderr, "Tester error: Wrong invalid map directory \
path\n"), true);
	elem = readdir(dir);
	rtn = false;
	while (elem)
	{
		if (elem->d_type == DT_DIR)
		{
			if (!strcmp(elem->d_name, ".") || !strcmp(elem->d_name, ".."))
			{
				elem = readdir(dir);
				continue ;
			}
			elem_name = _ft_strjoin3(maps_dir, "/", elem->d_name);
			if (!elem_name)
			{
				fprintf(stderr, "Tester error:\nMemory allocation failure\n");
				rtn = true;
				break ;
			}
			rtn = read_map_dir(exec_path, elem_name, env);
			free(elem_name);
			if (rtn)
				break ;
		}
		else if (elem->d_type == DT_REG)
		{
			pid = fork();
			if (pid == -1)
			{
				fprintf(stderr, "Tester error: fork error\n");
				rtn = false;
				break ;
			}
			if (!pid)
			{
				elem_name = strdup(elem->d_name);
				closedir(dir);
				if (!elem_name)
				{
					fprintf(stderr, "Tester error: Memory allocation \
failure\n");
					return (false);
				}
				return (_child_exec_cub(exec_path, maps_dir, elem_name, env));
			}
			else
			{
				waitpid(pid, &child_rtn, 0);
				if (WIFEXITED(child_rtn) && !WEXITSTATUS(child_rtn))
					printf("%s: \e[31mERROR NOT DETECTED\e[0m\n",
						elem->d_name);
				else
					printf("\e[30;1m==>%s: \e[33mok\e[0m\n", elem->d_name);
			}
		}
		elem = readdir(dir);
	}
	printf("\n\e[105m---- End: %s ----\e[0m\n\n", maps_dir);
	closedir(dir);
	return (rtn);
}

static int	_child_exec_cub(char *exec_path, char *maps_dir,
	char *map_name, char **env)
{
	char	*av[3];

	av[1] = _ft_strjoin3(maps_dir, "/", map_name);
	if (!av[1])
		return (fprintf(stderr, "Tester error:\nMemory allocation failure\n"),
			1);
	av[0] = "cub3D";
	av[2] = NULL;
	printf("\e[103m%s :\e[0m\n\e[37m", av[1]);
	execve(exec_path, av, env);
	free(av[1]);
	fprintf(stderr, "Tester error:\nexecve failure\n");
	exit (0);
}

static void	_test_maps_file_without_rights(char *exec_path, char **env)
{
	int		fd_unaccessible_file;
	char	*av[3];
	int		child_pid;
	int		child_rtn;

	printf("\e[103mfile_without_read_rights:\e[0m\n");
	av[0] = exec_path;
	av[1] = "no_rights.cub";
	av[2] = NULL;
	fd_unaccessible_file = open(av[1], O_CREAT | O_WRONLY, 0);
	close(fd_unaccessible_file);
	child_pid = fork();
	if (child_pid == -1)
	{
		fprintf(stderr, "Tester error:\n fork failure\n");
		remove(av[1]);
	}
	else if (!child_pid)
	{
		execve(exec_path, av, env);
		perror("execve");
		remove(av[1]);
		fprintf(stderr, "Tester error:\n execve failure\n");
		exit(0);
	}
	else
	{
		waitpid(child_pid, &child_rtn, 0);
		if (WIFEXITED(child_rtn) && !WEXITSTATUS(child_rtn))
			printf("%s: \e[31mERROR NOT DETECTED\e[0m\n", av[1]);
		else
			printf("\e[30;1m==>%s: \e[33mok\e[0m\n", av[1]);
		remove(av[1]);
	}
}

static void	_test_repo_without_rights(char *exec_path, char **env)
{
	DIR		*unaccessible_dir;
	char	*av[3];
	int		child_pid;
	int		child_rtn;
	int		fd_map_unreachable_texture;

	printf("\e[103mtexture_repo_without_read_rights:\e[0m\n");
	av[0] = exec_path;
	av[1] = "unreachable_text_repo.cub";
	av[2] = NULL;
	mkdir("no_rights_dir", 0);
	fd_map_unreachable_texture = open(av[1], O_CREAT | O_RDWR, 0766);
	if (fd_map_unreachable_texture == -1)
	{
		printf("\e[31mTester: open fail\e[0m\n");
		perror(NULL);
		return ;
	}
	write(fd_map_unreachable_texture, "NO ./no_rights_dir\nSO ./textures/\
wall_2\nWE ./textures/wall_3\nEA \
./textures/wall_4\nF 100,60,60\n\
C 60,160,255\n111\n1N1\n111", 155);
	close(fd_map_unreachable_texture);
	child_pid = fork();
	if (child_pid == -1)
	{
		fprintf(stderr, "Tester error:\n fork failure\n");
		remove("no_rights_dir");
		remove(av[1]);
	}
	else if (!child_pid)
	{
		execve(exec_path, av, env);
		fprintf(stderr, "Tester error:\n execve failure\n");
		exit(0);
	}
	else
	{
		waitpid(child_pid, &child_rtn, 0);
		if (WIFEXITED(child_rtn) && !WEXITSTATUS(child_rtn))
			printf("%s: \e[31mERROR NOT DETECTED\e[0m\n", "no_rights_dir");
		else
			printf("\e[30;1m==>%s: \e[33mok\e[0m\n", "no_rights_dir");
		remove(av[1]);
		remove("no_rights_dir");
	}
}

static size_t	_ft_strlen(const char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

static char	*_ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	int		fullsize;
	int		curpos;
	int		i;
	char	*full;

	if (s1 == NULL || s2 == NULL || s3 == NULL)
		return (NULL);
	fullsize = _ft_strlen(s1) + _ft_strlen(s2) + _ft_strlen(s3) + 1;
	full = calloc(fullsize, sizeof(char));
	if (!full)
		return (NULL);
	i = 0;
	curpos = 0;
	while (s1[i])
		full[curpos++] = s1[i++];
	i = 0;
	while (s2[i])
		full[curpos++] = s2[i++];
	i = 0;
	while (s3[i])
		full[curpos++] = s3[i++];
	return (full);
}
