/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 16:54:51 by fulguritu         #+#    #+#             */
/*   Updated: 2018/10/01 17:19:25 by fulguritu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_float			ft_supm_norm(t_ved_3d const v)
{
	t_float		sup;
	t_float		tmp;

	sup = ft_fabs(v[0]);
	tmp = ft_fabs(v[1]);
	sup = ft_fmax(tmp, sup);
	tmp = ft_fabs(v[2]);
	return (ft_fmax(tmp, sup));
}

t_bool		intersect_ray_cube(t_ray *objray)
{
	t_ray	tmp_ray;
	t_float	tmp;

	if (ft_supm_norm(objray->pos) >= 0.5)
		return (FALSE);
	
}
