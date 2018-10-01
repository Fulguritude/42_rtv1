/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polynom_rdr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/02 21:30:46 by fulguritu         #+#    #+#             */
/*   Updated: 2018/07/02 21:30:54 by fulguritu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/*
** t_poly		get_poly_from_filepath(char const *fpath)
** {
**
** }
*/

t_cpoly		get_cpoly_from_filepath(char const *fpath)
{
	t_cpoly		res;
	char		*str;
	int			fd;

	if ((fd = open(fpath, O_RDONLY)) == -1)
		exit_error("open: file not found", errno);
	str = NULL;
	get_next_line(fd, &str);
	if (!str || !ft_str_containsonly(str, "0123456789.+-abcdefpxABCDEFPX \t"))
		exit_error("str in file is invalid float format", 0);
	res = polyobj_to_cpoly(str);
	free(str);
	if (!(get_next_line(fd, &str) == EOF_RD))
		exit_error("cpoly file format invalid", 0);
	if (close(fd) == -1)
		exit_error("close: file closed improperly", errno);
	return (res);
}
