/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 23:00:36 by fulguritu         #+#    #+#             */
/*   Updated: 2018/09/16 23:00:57 by fulguritu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#if 0
/*
** has_inter refers to a single ray test with a single object here
** tmp is first quaddist from camera to contact; then intensity of the pixel
** (lum)
*/

static t_bool		ray_handle_sphere(t_control *ctrl, t_ray const ray
								t_sphere sphr, t_float *ray.t)
{
	t_vec_3d	contact_pt;
	t_vec_3d	contact_nrml;
	t_vec_3d	contact_dirlgt;
	t_bool		has_inter;
	t_float		tmp;

	if (!(has_inter = intersect_ray_sphere(contact_pt, contact_nrml, ray, sphr))
		return (FALSE);
	tmp = vec3_eucl_quaddist(contact_pt, ray.origin);
	if (has_inter && tmp < *ray.t) //one line to gain with double if assignment
	{
		*ray.t = tmp;
		vec3_sub(contact_dirlgt, ctrl->spot.origin, contact_pt);
		tmp = 1. / vec3_quadnorm(contact_dirlgt);
		vec3_nrmlz(contact_dirlgt, contact_dirlgt); //vec3_scale(sqrt(tmp), contact_dirlgt); should also work logically
		tmp = INV_PI * sphr.albedo * ctrl->spot.intensity
				* ft_max(0., vec3_dot(contact_nrml, contact_dirlgt));
	}

	((t_u32 *)ctrl->img_data)[pixel]
	return (has_inter);
}

/*
** has_inter refers to a single ray tested with the full list of objects here
*/

static t_bool	cast_ray_to_objs(t_control *ctrl, t_objlst const ol,
								t_ray const ray, int pixel)
{
	t_bool		has_inter;
	int			obj_k;
	t_float		ray.t;

	obj_k = 0;
	has_inter = FALSE;
	ray.t = 1. / 0.;
	while (obj_k < ol.len)
	{
		if (ol.objs[obj_k].type == sphere)
			ray_handle_sphere(ctrl, ray, ol.objs[obj_k].data.sphere, &ray.t);
		else if (ol.objs[obj_k].type != sphere)
			exit_error("cast_ray_to_objs: malformed object.", 0);
		++obj_k;
	}
	return (has_inter);
}
#endif

t_color			color_app_lum(t_vec_3d lum)//t_color clr, t_vec_3d lum)
{
	t_u8		red;
	t_u8		grn;
	t_u8		blu;

//printf("lum: %f\n", lum);
	red = ft_fmax(0., ft_fmin(255., lum[0]));//(clr >> 16) * lum[0];
	grn	= ft_fmax(0., ft_fmin(255., lum[1]));//(clr >> 8) * lum[1];
	blu = ft_fmax(0., ft_fmin(255., lum[2]));//(clr) * lum[2];
	return (red << 16 | grn << 8 | blu);
}



#if 0
/*
** Principle: you cast a ray from the point of contact in question to the light
** source(s) and if it intersects with an object, and 
** 		dist(contact, light) > dist(contact, shadow_ray_contact)
** then the contact point is hidden from the light source
*/
static t_bool	cast_shadow_ray()
{

}
#endif



static t_bool	cast_ray_to_objs(t_control *ctrl, t_objlst const ol,
								t_ray ray, int pixel) //maybe make pixel an int* and rm ctrl for subfunction
{
	t_bool		has_inter;
	t_vec_3d	contact_pt;
	t_vec_3d	contact_nrml;
	t_vec_3d	contact_dirlgt;
	t_float		tmp;
	t_vec_3d	lum;
	int			obj_k;

	obj_k = 0;
	has_inter = FALSE;
	while (obj_k < ol.len)
	{
		if (ol.objs[obj_k].type == sphere
		&& (has_inter = intersect_ray_sphere(&ray, ol.objs[obj_k].data.sphere)))
		{
			sphere_get_ctt_n_nrml(contact_pt, contact_nrml, ray, ol.objs[obj_k].data.sphere);
			vec3_sub(contact_dirlgt, ctrl->spot.origin, contact_pt);
			tmp = vec3_eucl_quadnorm(contact_dirlgt);
//
			tmp = 1. / tmp;
			vec3_scale(contact_dirlgt, sqrt(tmp), contact_dirlgt); ////vec3_eucl_nrmlz(contact_dirlgt, contact_dirlgt); is the costlier version
			
			vec3_scale(
				lum,
				INV_PI * ctrl->spot.intensity * ft_fmax(0., vec3_dot(contact_nrml, contact_dirlgt)) * tmp,
				ol.objs[obj_k].data.sphere.albedo);
			((t_u32 *)ctrl->img_data)[pixel] = color_app_lum(lum);
		}
		else if (ol.objs[obj_k].type != sphere)
			exit_error("cast_ray_to_objs: malformed object.", 0);
		++obj_k;
	}
	return (has_inter);
}

/*
** Rays are defined in Cam Space then sent to World Space.
**
** Intersections should be tested for in world space
*/

void			cast_rays(t_control *ctrl, t_objlst const objlst)
{
	int			i;
	int			j;
	t_ray		ray;
	t_vec_4d	tmp;
	t_float		fov_val;

	fov_val = -REN_W / (2 * tan(ctrl->cam.hrz_fov));
	i = 0;
	while (i < REN_H)
	{
		j = 0;
		while (j < REN_W)
		{
			vec3_cpy(ray.origin, (t_float *)ctrl->cam.c_to_w + 12);//ctrl->cam.world_pos);
			ray.t = 1. / 0.;
			vec3_set(tmp, j - REN_W / 2, i - REN_H / 2, fov_val);
			tmp[3] = 1.;
			mat44_app_vec(tmp, ctrl->cam.c_to_w, tmp);
			vec3_eucl_nrmlz(ray.dir, tmp);
			cast_ray_to_objs(ctrl, objlst, ray, i * REN_W + j);
			++j;
		}
		++i;
	}
}
