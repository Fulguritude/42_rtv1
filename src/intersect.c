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

#if 0
-/*
-** Should sphr.center in world_pos or view_pos ?
-**
-** Replace with a t_vec3 for coefs ?
-**
-** replace everything with ray as a pointer with t = inf at first, and return
-** false if the intersection is behind (o + t*dir)  ? 
-*/
-t_bool         intersect_ray_sphere(t_ray *ray, t_sphere const sphr)
-{
-       t_float         delta;
-       t_float         b;
-       t_float         c;
-       t_float         root1;
-       t_float         root2;
-       t_vec_3d        orig_to_sphr;
-
-       vec3_sub(orig_to_sphr, ray->origin, sphr.center);
-       b = 2 * vec3_dot(ray->dir, orig_to_sphr);
-       c = vec3_eucl_quadnorm(orig_to_sphr) - sphr.radius * sphr.radius;
-       delta = b * b - 4 * c;
-       if (delta < 0.)
-               return (FALSE);
-       delta = sqrt(delta);
-       root1 = (-b + delta) * 0.5;
-       root2 = (-b - delta) * 0.5;
-       if (root1 <= 0. || root2 <= 0. || (root1 > ray->t && root2 > ray->t))
-               return (FALSE);
-       ray->t = ft_fmin(root1, root2);
-       return (TRUE);
-}
-
-void           sphere_get_ctt_n_nrml(t_vec_3d contact, t_vec_3d normal,
-                                                                       t_ray const ray, t_sphere const sphr)
-{
-       vec3_scale(contact, ray.t, ray.dir);
-       vec3_add(contact, contact, ray.origin); //pos relative to ray->origin, which itself should be in world_pos
-       vec3_sub(normal, contact, sphr.center);
-       vec3_scale(normal, 1. / sphr.radius, normal);
-}
-
-void           sphere_wtoc(t_sphere *sphr, t_camera const cam)
-{
-       t_vec_4d        tmp;
-
-       vec3_cpy(tmp, sphr->center);
-       tmp[3] = 1.;
-       mat44_app_vec(tmp, cam.w_to_c, tmp);
-       vec3_cpy(sphr->center, tmp);
-}
#endif

//rename sphere_intersect_ray and make this sphere.c ?

/*
** This function should return TRUE iff objray.t has been updated.
** Rays should arrive already converted to object space coordinates.
**
** tmps = (b, c, root1, root2)
**
** Since 1. is the default radius, 1. * 1. = 1. is the quadratic norm
** Sphere is centered in (0., 0., 0.) by default
*/
t_bool		intersect_ray_sphere(t_ray *objray)//, t_object const sphr)
{
	t_float		delta;
//	t_vec_4d	tmps;
	t_float		a;
	t_float		b;
	t_float		c;
	t_float		root1;
	t_float		root2;
//	t_vec_3d	orig_to_sphr;


//printf("objray: origin-(%f, %f, %f); dir-(%f, %f, %f)\n", objray->origin[0], objray->origin[1], objray->origin[2], objray->dir[0], objray->dir[1], objray->dir[2]);
//	vec3_sub(orig_to_sphr, objray->origin, sphr.center);
	a = vec3_eucl_quadnorm(objray->dir);
	b = 2 * vec3_dot(objray->dir, objray->origin);
	c = vec3_eucl_quadnorm(objray->origin) - 1.;
	delta = b * b - 4 * a * c;
	if (delta < 0.)
		return (FALSE);
	a = 0.5 / a;
	delta = sqrt(delta);
	root1 = (-b + delta) * a;
	root2 = (-b - delta) * a;
//printf("delta: %6f\troot1: %6f\troot2: %6f\tobjray.t: %6f\n", delta, root1, root2, objray->t);
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

void		get_cnn_sphere(t_vec_3d contact, t_vec_3d normal,
									t_ray const objray)//, t_object const sphr)
{
	vec3_scale(contact, objray.t, objray.dir);
	vec3_add(contact, contact, objray.origin);
//printf("Should be 1.: %f\n", vec3_eucl_quadnorm(contact));
	vec3_cpy(normal, contact);
}
