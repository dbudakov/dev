/*budakvoda, https://github.com/dbudakov */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define SYMBOL ' '
#define NEGATIVE 1

// #define FILE "./testfile"
// #define NUM_COLUMN 3

size_t get_len_file(int fd)
{
	size_t num;

	// printf("%d", fd);
	lseek(fd, 0, SEEK_END);
	num = lseek(fd,0, SEEK_CUR);
	lseek(fd, 0, SEEK_SET);
	return num;
}

char *file_to_str(int fd, size_t len)
{
	char	*str;

	// printf("%d ", len);
	str = (char *)malloc(len + 1);
	read(fd, str, len);
	str[len] = '\0';
	// printf("%s", str);
	return str;
}

int get_num_str(char *str_file)
{
	int	i;
	int	n;

	n = 0;
	i = 0;
	while (str_file[i] != '\0')
	{
		if (str_file[i] == '\n')
			n++;
		i++;
	}
	return n;
}

int get_max_word(char *str_file)
{
	int i;
	int n;
	int max_word;

	n = 0;
	i = 0;
	max_word = 0;
	while (str_file[i] != '\0')
	{
		while (str_file[i] != '\n')
		{
			if (str_file[i] == ' ')
				n++;
			i++;
		}
		if (str_file[i] == '\n')
			n++;
		if (max_word < n)
			max_word = n;
		n = 0;
		i++;
	}
	return max_word;
}

int **get_arr_pre(char *str_file, int num_str, int num_max_word)
{
	int	i, j, k;
	int	start;
	int	end;
	int	s;
	int **arr_pre;

	i = 0;
	arr_pre = (int**)malloc(num_str * sizeof(int *));
	while(i < num_str)
	{
		arr_pre[i] = (int *)malloc(num_max_word * sizeof(int));
		i++;
	}

	i = 0;
	j = 0;
	k = 0;
	while(str_file[i] != '\0')
	{
		if (str_file[i] == SYMBOL || str_file[i] == '\n')
		{
			arr_pre[j][k++] = s;
			s = 0;
			if (str_file[i] != '\n')
				i++;
		}
		if (str_file[i] == '\n')
		{
			k = 0;
			j++;
		}
		s++;
		i++;
	}
	return arr_pre;
}

int *get_max_val_arr(int **arr_pre, int num_str, int num_max_word)
{
	int i;
	int j;
	int max_val;
	int	*arr_res;

	i = 0;
	j = 0;
	max_val = 0;
	arr_res = (int*)malloc(num_max_word * sizeof(int));
	while(i < num_max_word)
	{
		while(j < num_str)
		{
			if (max_val < arr_pre[j][i])
				max_val = arr_pre[j][i];
			j++;
		}
		arr_res[i] = max_val;
		max_val = 0;
		j = 0;
		i++;
	}
	i = num_str - 1;
	while(i >= 0)
		free(arr_pre[i--]);
	free(arr_pre);
	return arr_res;
}

void ft_column_output(int *arr_res,int num_column, char *str_file, int num_max_word)
{
	int	i;
	int	j;
	int n;
	char *str;
	int max_value;

	i = 0;
	j = 0;
	n = 0;
	max_value = 0;
	while( i < num_max_word)
	{
		if (max_value < arr_res[i])
			max_value = arr_res[i];
		i++;
	}
	i = 0;
	str = (char *)malloc(max_value + 1);
	if(str == NULL)
		exit(1);
	while(str_file[i] != '\0')
	{
		while (str_file[i] != SYMBOL && str_file[i] != '\n' && str_file[i] != '\0')
			str[n++] = str_file[i++];
		str[n] = '\0';
		n = 0;
		if (j < num_column)
		{
			if (NEGATIVE)
				printf("%-*s ", arr_res[j], str);
			else
				printf("%*s ", arr_res[j], str);
		}
		else
			printf("%s ", str);

		if (j < num_max_word - 1)
			j++;
		else
		{
			printf("\n");
			j = 0;
		}
		i++;
	}
	free(arr_res);
	free(str);
}

int *get_max_val_field(char *str_file, int num_column)
{
	int i, j;
	int **arr_pre;
	int *arr_res;
	int	num_str;
	int	num_max_word;


	i = 0;
	num_str = get_num_str(str_file);
	num_max_word = get_max_word(str_file);
	arr_pre = get_arr_pre(str_file, num_str, num_max_word);
	arr_res = get_max_val_arr(arr_pre, num_str, num_max_word);
	ft_column_output(arr_res, num_column, str_file, num_max_word);
	return arr_res;
}

int main(int argc, char **argv)
{
	int		i;
	int		j;
	int		fd;
	int		*arr;
	size_t	len;
	char	*str_file;
	int		num_column;

	if (argc < 3)
	{
		printf("format: ./a.out [filename] -t [num_column]\n");
		exit(1);
	}

	i = 0;
	num_column = 0;
	while  (i < argc)
	{
		if (!strcmp(argv[i], "-t"))
		{
			num_column = atoi(argv[i+1]);
			break;
		}
		i++;
	}
	j = 0;
	while(j < argc)
	{
		if (j != 0 && j != i && j != i + 1)
			fd = open(argv[j], O_RDONLY);
		j++;
	}

	len = get_len_file(fd);
	str_file = file_to_str(fd, len);
	arr = get_max_val_field(str_file, num_column);
	free(str_file);
	close(fd);
}
