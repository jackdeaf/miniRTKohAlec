/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Koh <Koh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 01:57:51 by Koh               #+#    #+#             */
/*   Updated: 2022/09/15 12:58:08 by Koh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// left trim by c, return count of char trimmed
// char *s = "aabc"; trim_chr(&a, 'a'); // return 2, s becomes "bc"
static int	trim_chr(char **line, char c)
{
	const char	*start = *line;

	while (**line && **line == c)
		++*line;
	return (*line - start);
}

// left trim by isxxx(), return count of char trimmed
// char *s = "12bc"; trim_str(&a, isdigit); // return 2, s becomes "bc"
int	trim_str(char **line, int (*f)(int))
{
	const char	*start = *line;

	while (**line && f(**line))
		++*line;
	return (*line - start);
}

// trim /^-?[0-9]+.?[0-9]*/ into number, while moving str pointer
// return 1 if correct format and number within min and max (inclusive)
// return 0 otherwise
int	pull_nbr(char **line, double *d, double min, double max)
{
	const char	*begin = *line;

	if (trim_chr(line, '-') <= 1
		&& trim_str(line, ft_isdigit) >= 1
		&& trim_chr(line, '.') <= 1
		&& trim_str(line, ft_isdigit) >= 0
	)
	{
		*d = parse_double(begin);
		return (*d >= min && *d <= max);
	}
	return (0);
}

// trim <number>,<number>,<number> into t_vec3, while moving str pointer
// return 1 if correct format and number within min and max (inclusive)
// return 0 otherwise
int	pull_vec(char **line, t_vec3 *d, double min, double max)
{
	return (
		pull_nbr(line, &d->x, min, max) == 1
		&& trim_chr(line, ',') == 1
		&& pull_nbr(line, &d->y, min, max) == 1
		&& trim_chr(line, ',') == 1
		&& pull_nbr(line, &d->z, min, max) == 1
	);
}

// trim <number>,<number>,<number> into t_vec3, while moving str pointer
// number is converted into 0.0 to 1.0 instead of 0 and 255
// return 1 if correct format and number within 0 and 255 (inclusive)
// return 0 otherwise
int	pull_rgb(char **line, t_vec3 *d)
{
	if (
		pull_nbr(line, &d->x, 0, 255) == 1
		&& trim_chr(line, ',') == 1
		&& pull_nbr(line, &d->y, 0, 255) == 1
		&& trim_chr(line, ',') == 1
		&& pull_nbr(line, &d->z, 0, 255) == 1
	)
	{
		d->r /= 255.0;
		d->g /= 255.0;
		d->b /= 255.0;
		return (1);
	}
	return (0);
}
