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
** Principle: you cast a ray from the point of contact in question to the light
** source(s) and if it intersects with an object, and 
** 		dist(contact, light) > dist(contact, shadow_ray_contact)
** then the contact point is hidden from the light source
*/
static t_bool	cast_shadow_ray()
{

}
#endif

t_color			color_app_lum(t_vec_3d lum)//t_color clr, t_vec_3d lum)
{
	t_u8		red;
	t_u8		grn;
	t_u8		blu;

//printf("lum: % .3f\n", lum);
	red = ft_fmax(0., ft_fmin(255., lum[0]));//(clr >> 16) * lum[0];
	grn	= ft_fmax(0., ft_fmin(255., lum[1]));//(clr >> 8) * lum[1];
	blu = ft_fmax(0., ft_fmin(255., lum[2]));//(clr) * lum[2];
	return (red << 16 | grn << 8 | blu);
}

void			mat44_app_vec3(t_vec_3d result, t_mat_4b4 const mat, t_vec_3d const v)
{
	t_vec_4d	tmp;

	mat44_app_vec(tmp, mat, (t_vec_4d){v[0], v[1], v[2], 1.});
	vec3_cpy(result, tmp);
}

t_ray			ray_x_to_y(t_mat_4b4 const x_to_y,
						t_mat_4b4 unit_x_to_y, t_ray const ray)
{
	t_ray		result;

	mat44_app_vec3(result.origin, x_to_y, ray.origin);
	mat44_app_vec3(result.dir, unit_x_to_y, ray.dir);
	result.t = ray.t;
	return (result);
}

#if 0
-static t_bool  cast_ray_to_objs(t_control *ctrl, t_objlst const ol,
-                                                               t_ray ray, int pixel) //maybe make pixel an int* and rm ctrl for subfunction
-{
-       t_bool          has_inter;
-       t_vec_3d        contact_pt;
-       t_vec_3d        contact_nrml;
-       t_vec_3d        contact_dirlgt;
-       t_float         tmp;
-       t_vec_3d        lum;
-       int                     obj_k;
-
-       obj_k = 0;
-       has_inter = FALSE;
-       while (obj_k < ol.len)
-       {
-               if (ol.objs[obj_k].type == sphere
-               && (has_inter = intersect_ray_sphere(&ray, ol.objs[obj_k].data.sphere)))
-               {
-                       sphere_get_ctt_n_nrml(contact_pt, contact_nrml, ray, ol.objs[obj_k].data.sphere);
-                       vec3_sub(contact_dirlgt, ctrl->spot.origin, contact_pt);
-                       tmp = vec3_eucl_quadnorm(contact_dirlgt);
-//
-                       tmp = 1. / tmp;
-                       vec3_scale(contact_dirlgt, sqrt(tmp), contact_dirlgt); ////vec3_eucl_nrmlz(contact_dirlgt, contact_dirlgt); is the c
ostlier version
-                       
-                       vec3_scale(
-                               lum,
-                               INV_PI * ctrl->spot.intensity * ft_fmax(0., vec3_dot(contact_nrml, contact_dirlgt)) * tmp,
-                               ol.objs[obj_k].data.sphere.albedo);
-                       ((t_u32 *)ctrl->img_data)[pixel] = color_app_lum(lum);
-               }
-               else if (ol.objs[obj_k].type != sphere)
-                       exit_error("cast_ray_to_objs: malformed object.", 0);
-               ++obj_k;
-       }
-       return (has_inter);
-}
#endif

#if 0
static t_bool	cast_ray_to_objs(t_control *ctrl, t_ray ray, int pixel)
{
	t_bool		has_inter;
	t_vec_3d	contact_pt;
	t_vec_3d	contact_nrml;
	t_vec_3d	contact_dirlgt;
	t_float		tmp;
	t_vec_3d	lum;
	t_ray		objray;
	t_light_src	objspot;
	int			k;
	t_object	cur_obj;

	k = 0;
	has_inter = FALSE;
	while (k < ctrl->objlst_len)
	{
		cur_obj = ctrl->objlst[k];
		objray = ray_x_to_y(cur_obj.w_to_o, ray);
//		mat44_app_vec3(objspot.origin, cur_obj.w_to_o, ctrl->spot.origin);
		if ((has_inter = cur_obj.intersect(&objray)))//, cur_obj)))
		{
//printf("k: %d | ray: (% .3f, % .3f, % .3f) + % .3f * (% .3f, % .3f, % .3f)\n", k, objray.origin[0], objray.origin[1], objray.origin[2], objray.t, objray.dir[0], objray.dir[1], objray.dir[2]);
			ray.t = objray.t;
			cur_obj.get_cnn(contact_pt, contact_nrml, objray);
//printf("1- contact_pt: ||(% .3f, % .3f, % .3f)|| = %f\tcontact_nrml: ||(% .3f, % .3f, % .3f)|| = %f\n", contact_pt[0], contact_pt[1], contact_pt[2], vec3_eucl_quadnorm(contact_pt), contact_nrml[0], contact_nrml[1], contact_nrml[2], vec3_eucl_quadnorm(contact_nrml));
		mat44_app_vec3(contact_pt, cur_obj.o_to_w, contact_pt);
		mat44_app_vec3(contact_nrml, cur_obj.o_to_w, contact_nrml);
	    vec3_add(contact_nrml, contact_nrml, (t_float *)cur_obj.o_to_w + 12);
		vec3_eucl_nrmlz(contact_nrml, contact_nrml);
//printf("2- contact_pt: ||(% .3f, % .3f, % .3f)|| = %f\tcontact_nrml: ||(% .3f, % .3f, % .3f)|| = %f\n\n", contact_pt[0], contact_pt[1], contact_pt[2], vec3_eucl_quadnorm(contact_pt), contact_nrml[0], contact_nrml[1], contact_nrml[2], vec3_eucl_quadnorm(contact_nrml));
		//objspot = ctrl->spot;
		//mat44_app_vec3(objspot.origin, cur_obj.w_to_o, objspot.origin);
			vec3_sub(contact_dirlgt, objspot.origin, contact_pt);
			tmp = vec3_eucl_quadnorm(contact_dirlgt);
			tmp = 1. / tmp;
			vec3_scale(contact_dirlgt, sqrt(tmp), contact_dirlgt); //less costly
			//vec3_eucl_nrmlz(contact_dirlgt, contact_dirlgt); //is the costlier version
			
			vec3_scale(
				lum,
				INV_PI * ctrl->spot.intensity * ft_fmax(0., vec3_dot(contact_nrml, contact_dirlgt)) * tmp,
				cur_obj.albedo);
			((t_u32 *)ctrl->img_data)[pixel] = color_app_lum(lum);
		}
		else if (cur_obj.type != sphere)
			exit_error("cast_ray_to_objs: malformed object.", 0);
		++k;
	}
	return (has_inter);
}
#endif


#if 0
t_light_src		light_x_to_y(t_mat_4b4 const x_to_y, t_light_src const spot,
				t_control *ctrl)
{
	t_light_src		result;

	mat44_app_vec3(result.origin, x_to_y, spot.origin);

	if (ctrl->debug)
		result.intensity = spot.intensity;
	else
		result.intensity = spot.intensity *
			vec3_eucl_norm(result.origin) / vec3_eucl_norm(spot.origin);
	return (result);
}

static t_bool	cast_ray_to_objs(t_control *ctrl, t_ray ray, int const pixel)
{
	t_bool		has_inter;
	t_vec_3d	contact_pt;
	t_vec_3d	contact_nrml;
	t_vec_3d	contact_dirlgt;
	t_float		tmp;
	t_vec_3d	lum;
	t_ray		objray;
	t_light_src	objspot;
	int			k;
	t_object	cur_obj;

	k = 0;
	has_inter = FALSE;
	while (k < ctrl->objlst_len)
	{
		cur_obj = ctrl->objlst[k];
		objray = ray_x_to_y(cur_obj.w_to_o, ray);
		if ((has_inter = cur_obj.intersect(&objray)))//, cur_obj)))
		{
			ray.t = objray.t;
			cur_obj.get_cnn(contact_pt, contact_nrml, objray);
			
		//mat44_app_vec3(contact_pt, cur_obj);
//objspot = ctrl->spot.origin;
objspot = light_x_to_y(cur_obj.w_to_o, ctrl->spot, ctrl);
			vec3_sub(contact_dirlgt, objspot.origin, contact_pt);
			tmp = vec3_eucl_quadnorm(contact_dirlgt);
			tmp = 1. / tmp;
			//vec3_scale(contact_dirlgt, sqrt(tmp), contact_dirlgt); //less costly
			vec3_eucl_nrmlz(contact_dirlgt, contact_dirlgt); //is the costlier version




			vec3_scale(
				lum,
				INV_PI * objspot.intensity * ft_fmax(0., vec3_dot(contact_nrml, contact_dirlgt)) * tmp,
				cur_obj.albedo);
			((t_u32 *)ctrl->img_data)[pixel] = color_app_lum(lum);
		}
		else if (cur_obj.type != sphere)
			exit_error("cast_ray_to_objs: malformed object.", 0);
		++k;
	}
	return (has_inter);
}
#endif

static t_bool	cast_ray_to_objs(t_control *ctrl, t_ray ray, int const pixel)
{
	t_bool		has_inter;
	t_vec_3d	contact_pt;
	t_vec_3d	contact_nrml;
	t_vec_3d	contact_dirlgt;
	t_float		tmp;
	t_vec_3d	lum;
	t_ray		objray;
	int			k;
	t_object	cur_obj;

	k = -1;
	has_inter = FALSE;
	while (++k < ctrl->objlst_len)
	{
		cur_obj = ctrl->objlst[k];
//print_mat44("w_to_o:", cur_obj.w_to_o);
//print_vec4("ray.dir:", (t_vec_4d){ray.dir[0], ray.dir[1], ray.dir[2], 1.});
		objray = ray_x_to_y(cur_obj.w_to_o, cur_obj.unit_w_to_o, ray);
		if ((has_inter = cur_obj.intersect(&objray)))//, cur_obj)))
		{
			ray.t = objray.t;
			cur_obj.get_cnn(contact_pt, contact_nrml, objray);
			
			mat44_app_vec3(contact_pt, cur_obj.o_to_w, contact_pt);
			mat44_app_vec3(contact_nrml, cur_obj.n_to_w, contact_nrml);
			vec3_eucl_nrmlz(contact_nrml, contact_nrml);

//if (ctrl->debug)
//printf("should be 1: %f\n", vec3_eucl_quadnorm(contact_nrml));

			vec3_sub(contact_dirlgt, ctrl->spot.origin, contact_pt);
			tmp = vec3_eucl_quadnorm(contact_dirlgt);
			tmp = 1. / tmp;
			//vec3_scale(contact_dirlgt, sqrt(tmp), contact_dirlgt); //less costly
			vec3_eucl_nrmlz(contact_dirlgt, contact_dirlgt); //is the costlier version




			vec3_scale(
				lum,
				INV_PI * ctrl->spot.intensity * ft_fmax(0., vec3_dot(contact_nrml, contact_dirlgt)) * tmp,
				cur_obj.albedo);
			((t_u32 *)ctrl->img_data)[pixel] = color_app_lum(lum);
		}
		else if (cur_obj.type != sphere)
			exit_error("cast_ray_to_objs: malformed object.", 0);
	}
	return (has_inter);
}

/*
** Rays are defined in Cam Space then sent to World Space.
**
** Intersections should be tested for in world space
*/

void			cast_rays(t_control *ctrl)
{
	int			i;
	int			j;
	t_ray		ray;
	t_vec_3d	tmp;
	t_float		fov_val;

//print_mat44("c_to_w", ctrl->cam.c_to_w);
//print_mat44("w_to_c", ctrl->cam.w_to_c);
	fov_val = -REN_W / (2 * tan(ctrl->cam.hrz_fov));
	i = 0;
	while (i < REN_H)
	{
		j = 0;
		while (j < REN_W)
		{
//TODO cleanup with x_to_y
			vec3_cpy(ray.origin, (t_float *)(ctrl->cam.c_to_w) + 12);//ctrl->cam.world_pos);
			ray.t = 1. / 0.;
			vec3_set(tmp, j - REN_W / 2, i - REN_H / 2, fov_val);
//t_vec_3d trans;
//vec3_cpy(trans, (t_float *)(ctrl->cam.c_to_w) + 12);
//vec3_cpy((t_float *)(ctrl->cam.c_to_w) + 12, (t_vec_3d){0., 0., 0.});
			mat44_app_vec3(tmp, ctrl->cam.c_to_w, tmp);
//vec3_cpy((t_float *)(ctrl->cam.c_to_w) + 12, trans);
			vec3_eucl_nrmlz(ray.dir, tmp);
			cast_ray_to_objs(ctrl, ray, i * REN_W + j);
			++j;
		}
		++i;
	}
}
