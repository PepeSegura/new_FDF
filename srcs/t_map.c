/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_map.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psegura- <psegura-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 12:26:02 by psegura-          #+#    #+#             */
/*   Updated: 2024/08/05 12:04:57 by psegura-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FDF.h"

void	print_map(t_map *map)
{
	int	i;
	int	j;

	i = 0;
	printf("\nSTART\n");
	while (i < map->actual_size)
	{
		j = 0;
		while (j < map->min_wide)
		{
			printf("(%.2d, %.2d, %.2d) ", map->points[i][j].x,
				map->points[i][j].y, map->points[i][j].z);
			j++;
		}
		printf("\n");
		i++;
	}
	printf("\nEND\n");
}

void	init_map(t_map *map)
{
	map->max_size = 128;
	map->actual_size = 0;
	map->min_wide = INT32_MAX;
	map->points = ft_calloc(map->max_size, sizeof(t_point *));
	if (map->points == NULL)
		ft_error("Malloc failed");
}

void	resize_map(t_map *map)
{
	t_point	**new_points;
	int		i;

	map->max_size = map->max_size << 1;
	new_points = ft_calloc(map->max_size, sizeof(t_point *));
	if (new_points == NULL)
		ft_error("Malloc failed");
	i = 0;
	while (i < map->actual_size)
	{
		new_points[i] = map->points[i];
		i++;
	}
	free(map->points);
	map->points = new_points;
}

#define VALID_CHARS "+-0123456789,xabcdef \f\n\r\t\v"
#define DIGITS_SYMBOLS "0123456789+-"

int	count_points(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] && str[i] != '\n')
	{
		if (ft_strchr(VALID_CHARS, str[i]) == NULL)
			ft_error("Invalid CHAR");
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (str[i] && ft_strchr(DIGITS_SYMBOLS, str[i]))
		{
			count++;
			while (str[i] && !ft_isspace(str[i]))
				i++;
		}
		else if (str[i] == '\0' || str[i] == '\n')
			break ;
		else
			ft_error("Invalid CHAR");
	}
	return (count);
}

void	skip_digits_and_set_color(t_point *point, char *line, int *i)
{
	while (line[*i] && ft_strchr(DIGITS_SYMBOLS, line[*i]))
		(*i)++;
	while (line[*i] && ft_isspace(line[*i]))
		(*i)++;
	if (line[*i] == ',')
	{
		point->color = ft_atol_16(&line[*i + 3]); // (ft_atoi(&line[i]) << 8) | 0xFF;
		while (line[*i] && line[*i] != ' ')
			(*i)++;
	}
}

void	init_point(t_point *point, int x, int y)
{
	point->x = x;
	point->y = y;
	point->color = DEFAULT_COLOR;
}

void	init_points_line(char *line, t_point *points_line, int map_actual_size)
{
	int	i;
	int	point;

	i = 0;
	point = 0;
	while (line[i] && line[i] != '\n')
	{
		while (line[i] && ft_isspace(line[i]))
			i++;
		if (line[i] && ft_strchr(DIGITS_SYMBOLS, line[i]))
		{
			init_point(&points_line[point], point, map_actual_size);
			points_line[point].z = ft_atoi(&line[i]);
			skip_digits_and_set_color(&points_line[point], line, &i);
			point++;
		}
		else
		{
			while (line[i] && !ft_isspace(line[i]))
				i++;
		}
	}
}

void	add_line(t_map *map, char *line)
{
	int		size_line;
	t_point	*points_line;

	if (map->actual_size == map->max_size)
		resize_map(map);
	size_line = count_points(line);
	if (size_line == 0)
		ft_error("There is an empty line");
	if (size_line < map->min_wide)
		map->min_wide = size_line;
	points_line = ft_calloc(size_line, sizeof(t_point));
	if (points_line == NULL)
		ft_error("Malloc failed");
	init_points_line(line, points_line, map->actual_size);
	map->points[map->actual_size] = points_line;
	map->actual_size++;
}
