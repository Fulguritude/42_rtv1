/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/29 07:05:34 by fulguritu         #+#    #+#             */
/*   Updated: 2018/09/29 07:05:47 by fulguritu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

#if 0
static void		shader_diffuse(t_vector *result, t_shader *shader)
{
	vector_scale(&shader->light, 2);
	vector_invert(&shader->ray.dir);
	shader->ray.t = vector_scalar(&shader->ray.dir, &shader->hit_shdr.normal);
	if (shader->ray.t < 0)
		shader->ray.t = 0;
	vector_set(result,
		result->x + shader->light.x * shader->ray.t,
		result->y + shader->light.y * shader->ray.t,
		result->z + shader->light.z * shader->ray.t);
	vector_invert(&shader->ray.dir);
}

static void		shader_specular(t_vector *result, t_shader *shader, t_ray *ray)
{
	vector_shdr.normalize(&shader->light);
	vector_scale(&shader->light, 200);
	shader->ray.t = 2 * vector_scalar(&shader->ray.dir, &shader->hit_shdr.normal);
	shader->ray.dir.x -= shader->hit_shdr.normal.x * shader->ray.t;
	shader->ray.dir.y -= shader->hit_shdr.normal.y * shader->ray.t;
	shader->ray.dir.z -= shader->hit_shdr.normal.z * shader->ray.t;
	shader->ray.t = -vector_scalar(&shader->ray.dir, &ray->dir);
	if (shader->ray.t < 0)
		shader->ray.t = 0;
	vector_set(result,
		result->x + shader->light.x * shader->ray.t,
		result->y + shader->light.y * shader->ray.t,
		result->z + shader->light.z * shader->ray.t);
}

static t_u32	shader_getcolor(t_u32 color,
	t_vector *diffuse,
	t_vector *specular)
{
	int	r;
	int	g;
	int	b;

	r = (float)color_get_r(color) * diffuse->x + specular->x;
	g = (float)color_get_g(color) * diffuse->y + specular->y;
	b = (float)color_get_b(color) * diffuse->z + specular->z;
	if (r < 0)
		r = 0;
	else if (r >= 256)
		r = 255;
	if (g < 0)
		g = 0;
	else if (g >= 256)
		g = 255;
	if (b < 0)
		b = 0;
	else if (b >= 256)
		b = 255;
	return (color_new(0, r, g, b));
}

void			shader_init(t_shader *shader)
{
	ft_memcpy(&shader->ray.orig, &shader->hit_pos, sizeof(t_vector));
	shader->ray.orig.x += shader->hit_shdr.normal.x * LIGHT_BIAS;
	shader->ray.orig.y += shader->hit_shdr.normal.y * LIGHT_BIAS;
	shader->ray.orig.z += shader->hit_shdr.normal.z * LIGHT_BIAS;
}

void			shader_setupray(t_shader *shader, t_light *light)
{
	vector_set(&shader->ray.dir,
		shader->hit_pos.x - light->position.x,
		shader->hit_pos.y - light->position.y,
		shader->hit_pos.z - light->position.z);
	vector_set(&shader->light,
		(float)color_get_r(light->color) * light->strength,
		(float)color_get_g(light->color) * light->strength,
		(float)color_get_b(light->color) * light->strength);
	vector_scale(&shader->light, 1 / (4 * M_PI * vector_length(&shader->ray.dir)));
	vector_shdr.normalize(&shader->ray.dir);
}

t_u32			render_shade(t_rtv1 *rtv1, t_ray *ray, t_shader *shader)
{
	int			render;
	t_list		*lst;
	float		tmp;

	render = rtv1->camera->render;
	tmp = (render & RENDER_DIFFUSE) ? 0.2 : 1;
	vector_set(&shader->diffuse, tmp, tmp, tmp);
	vector_set(&shader->specular, 0, 0, 0);
	shader_init(shader);
	lst = rtv1->lights;
	while (lst)
	{
		shader_setupray(shader, (t_light *)lst->content);
		if (!(render & RENDER_SHADOWS) || !(render_trace(rtv1, &shader->ray)))
		{
			if (render & RENDER_DIFFUSE)
				shader_diffuse(&shader->diffuse, shader);
			if (render & RENDER_SPECULAR)
				shader_specular(&shader->specular, shader, ray);
		}
		lst = lst->next;
	}
	return (shader_getcolor(shader->object_color,
		&shader->diffuse, &shader->specular));
}
#endif


static t_color			color_app_lum(t_vec_3d lum)//t_color clr, t_vec_3d lum)
{
	t_u8		red;
	t_u8		grn;
	t_u8		blu;

//printf("lum: % .3f\n", lum);
	red = ft_fmax(0., ft_fmin(255., lum[0]));
	grn	= ft_fmax(0., ft_fmin(255., lum[1]));
	blu = ft_fmax(0., ft_fmin(255., lum[2]));
	return (red << 16 | grn << 8 | blu);
}

/*
** Returns the result of light and object albedo color construction
*/

static void		shader_get_diffuse(t_vec_3d reslum, t_control *ctrl,
										t_shader shdr, t_light const spot)
{
	t_float		tmp;

	vec3_sub(shdr.dirlight.dir, spot.pos, shdr.dirlight.pos);
	tmp = vec3_eucl_quadnorm(shdr.dirlight.dir);
	shdr.dirlight.t = sqrt(tmp);
	vec3_scale(shdr.dirlight.dir, 1. / shdr.dirlight.t, shdr.dirlight.dir); //less costly
//	vec3_eucl_nrmlz(shdr.dirlight.dir, shdr.dirlight.dir); //is the costlier version
	if (trace_ray_to_objs(ctrl, shdr.dirlight, NULL, NULL))
	{
		vec3_set(reslum, 0., 0., 0.);
		return ;
	}
	vec3_scale(reslum,
		INV_PI * spot.intensity * ft_fmax(0., vec3_dot(shdr.normal, shdr.dirlight.dir)) / tmp,
		shdr.obj_albedo);
	vec3_set(reslum, reslum[0] * spot.rgb[0], reslum[1] * spot.rgb[1], reslum[2] * spot.rgb[2]);
}

/*
static void		shader_get_specular(t_vec_3d reslum, t_control *ctrl,
										t_shader shdr, t_light const spot)
{
	return ;
}*/

/*
** Principle: you cast a ray from the point of contact in question to the light
** source(s) and if it intersects with an object, and 
** 		dist(contact, light) > dist(contact, shadow_ray_contact)
** then the contact point is hidden from the light source
*/

t_color			get_color_from_fixed_objray(t_control *ctrl,
							t_object const obj, t_ray const objray)
{
	t_shader	shdr;
	t_vec_3d	lum;
	t_vec_3d	tmp;
	t_light		cur_spot;
	int			i;
	
	obj.get_hnn(shdr.dirlight.pos, shdr.normal, objray);
	vec3_scale(lum, APPROX, shdr.normal);
	vec3_add(shdr.dirlight.pos, shdr.dirlight.pos, lum);
	mat44_app_vec3(shdr.dirlight.pos, obj.o_to_w, shdr.dirlight.pos);
	mat44_app_vec3(shdr.normal, obj.n_to_w, shdr.normal);
	vec3_eucl_nrmlz(shdr.normal, shdr.normal); //necessary ?
	vec3_cpy(shdr.obj_albedo, obj.albedo);
	vec3_set(lum, 0., 0., 0.);
	i = -1;
	while (++i < ctrl->spotlst_len)
	{
		cur_spot = ctrl->spotlst[i];
		shader_get_diffuse(tmp, ctrl, shdr, cur_spot);
		vec3_add(lum, lum, tmp);
	}
	return (color_app_lum(lum));
}
