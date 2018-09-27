/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 22:59:05 by fulguritu         #+#    #+#             */
/*   Updated: 2018/09/16 23:00:18 by fulguritu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

/*
** This function should return TRUE iff objray.t has been updated.
** Rays should arrive already converted to object space coordinates.
**
** The sphere primitive is centered in (0., 0., 0.) by default.
** Since 1. is the default radius, 1. * 1. = 1. is the quadratic norm of our
**		sphere primitive.
*/

t_bool		intersect_ray_sphere(t_ray *objray)
{
	t_float		delta;
	t_vec_4d	quadpoly;
	t_float		root1;
	t_float		root2;

	quadpoly[0] = vec3_eucl_quadnorm(objray->dir);
	quadpoly[1] = 2 * vec3_dot(objray->dir, objray->pos);
	quadpoly[2] = vec3_eucl_quadnorm(objray->pos) - 1.;
	delta = quadpoly[1] * quadpoly[1] - 4 * quadpoly[0] * quadpoly[2];
	if (delta < 0.)
		return (FALSE);
	quadpoly[0] = 0.5 / quadpoly[0];
	delta = sqrt(delta);
	root1 = (-quadpoly[1] + delta) * quadpoly[0];
	root2 = (-quadpoly[1] - delta) * quadpoly[0];
	if (root1 <= 0. || root2 <= 0. || (root1 > objray->t && root2 > objray->t))
		return (FALSE);
	objray->t = ft_fmin(root1, root2);
	return (TRUE);
}

/*
** Rays should arrive already converted to object space.
**
** Returns both vectors in object space. Note that on the unit sphere of R3,
** every point on the sphere corresponds to its unit normal.
*/

void		get_hnn_sphere(t_vec_3d hitpos, t_vec_3d normal,
									t_ray const objray)
{
	vec3_scale(hitpos, objray.t, objray.dir);
	vec3_add(hitpos, hitpos, objray.pos);
	vec3_cpy(normal, hitpos);
}
