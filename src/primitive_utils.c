/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   primitive_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 01:05:01 by fulguritu         #+#    #+#             */
/*   Updated: 2018/09/28 01:05:03 by fulguritu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_bool		get_realroots_quadpoly(t_float *root1, t_float *root2,
									t_vec_3d const quadpoly)
{
	t_float		delta;
	t_float		one_over_two_a;

	delta = quadpoly[1] * quadpoly[1] - 4 * quadpoly[0] * quadpoly[2];
	if (delta < 0.)
		return (FALSE);
	one_over_two_a = 0.5 / quadpoly[0];
	delta = sqrt(delta);
	*root1 = (-quadpoly[1] + delta) * one_over_two_a;
	*root2 = (-quadpoly[1] - delta) * one_over_two_a;
	return (TRUE);
}

/*
** Returns the point on the object hit by a ray in object space coordinates.
*/

inline void		get_ray_hitpos(t_vec_3d hitpos, t_ray const objray)
{
	vec3_scale(hitpos, objray.t, objray.dir);
	vec3_add(hitpos, hitpos, objray.pos);
}
