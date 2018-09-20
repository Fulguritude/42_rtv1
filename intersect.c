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

//rename sphere_intersect_ray and make this sphere.c ?

/*
** Should sphr.center in world_pos or view_pos ?
**
** Replace with a t_vec3 for coefs ?
**
** replace everything with ray as a pointer with t = inf at first, and return
** false if the intersection is behind (o + t*dir)  ? 
*/
t_bool		intersect_ray_sphere(t_ray *ray, t_sphere const sphr)
{
	t_float		delta;
	t_float		b;
	t_float		c;
	t_float		root1;
	t_float		root2;
	t_vec_3d	orig_to_sphr;

	vec3_sub(orig_to_sphr, ray->origin, sphr.center);
	b = 2 * vec3_dot(ray->dir, orig_to_sphr);
	c = vec3_eucl_quadnorm(orig_to_sphr) - sphr.radius * sphr.radius;
	delta = b * b - 4 * c;
	if (delta < 0.)
		return (FALSE);
	delta = sqrt(delta);
	root1 = (-b + delta) * 0.5;
	root2 = (-b - delta) * 0.5;
	if (root1 <= 0. || root2 <= 0. || (root1 > ray->t && root2 > ray->t))
		return (FALSE);
	ray->t = ft_fmin(root1, root2);
	return (TRUE);
}

void		sphere_get_ctt_n_nrml(t_vec_3d contact, t_vec_3d normal,
									t_ray const ray, t_sphere const sphr)
{
	vec3_scale(contact, ray.t, ray.dir);
	vec3_add(contact, contact, ray.origin); //pos relative to ray->origin, which itself should be in world_pos
	vec3_sub(normal, contact, sphr.center);
	vec3_scale(normal, 1. / sphr.radius, normal);
}

void		sphere_wtoc(t_sphere *sphr, t_camera const cam)
{
	t_vec_4d	tmp;

	vec3_cpy(tmp, sphr->center);
	tmp[3] = 1.;
	mat44_app_vec(tmp, cam.w_to_c, tmp);
	vec3_cpy(sphr->center, tmp);
}


//parrallelepiped, cone, cylinder
