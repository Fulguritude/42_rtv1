/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 03:50:29 by fulguritu         #+#    #+#             */
/*   Updated: 2018/09/21 03:51:28 by fulguritu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static t_object		build_obj_matrices(t_vec_3d const world_pos,
								t_vec_3d const xyz_scaling,
								t_vec_3d const xyz_rot_rad,
								t_vec_3d const rgb_albedo)
{
	t_object	res;
	t_mat_3b3	rot;
	t_mat_3b3	tmp;

	res.type = null_obj;
	res.intersect = NULL;
	vec3_cpy(res.pos, world_pos);
	vec3_cpy(res.scl, xyz_scaling);
	vec3_cpy(res.rot, xyz_rot_rad);
	vec3_cpy(res.albedo, rgb_albedo);
	mat33_rot(rot, res.rot[0], 0);
	mat33_rot(tmp, res.rot[1], 1);
	mat33_mul(rot, tmp, rot);
	mat33_rot(tmp, res.rot[2], 2);
	mat33_mul(rot, tmp, rot);
	mat33_set_diagmat(tmp, res.scl);
	mat33_mul(rot, rot, tmp);
	mat33_inv(tmp, rot);
	mat44_set(res.unit_o_to_w, rot, (t_vec_3d){0, 0, 0}, (t_vec_4d){0, 0, 0, 1.});
	mat44_set(res.unit_w_to_o, tmp, (t_vec_3d){0, 0, 0}, (t_vec_4d){0, 0, 0, 1.});
	mat44_set(res.o_to_w, rot, (t_vec_3d){0, 0, 0},
					(t_vec_4d){res.pos[0], res.pos[1], res.pos[2], 1.});
	mat44_inv(res.w_to_o, res.o_to_w);
	mat44_transpose(res.n_to_w, res.w_to_o);
	return (res);
}

static t_inter_func		get_inter_func(t_objtype const type)
{
	t_inter_func	res;

	res = type == sphere ? &intersect_ray_sphere : NULL;
	res = type == plane ? &intersect_ray_plane : res;
	res = type == infcone ? &intersect_ray_infcone : res;
	res = type == infcylinder ? &intersect_ray_infcylinder : res;
	res = type == disk ? &intersect_ray_disk : res;
	res = type == square ? &intersect_ray_square : res;
//	res = type == cone ? &intersect_ray_cone : res;
	res = type == cylinder ? &intersect_ray_cylinder : res;
	return (res);
}

static t_hnn_func		get_hnn_func(t_objtype const type)
{
	t_hnn_func	res;

	res = type == sphere ? &get_hnn_sphere : NULL;
	res = type == infcone ? &get_hnn_infcone : res;
	res = type == infcylinder ? &get_hnn_infcylinder : res;
//	res = type == cone ? &get_hnn_cone : res;
	res = type == cylinder ? &get_hnn_cylinder : res;
	res = type == plane || type == disk || type == square ? get_hnn_plane : res;
	return (res);
}

void				print_object(t_object const obj)
{
	printf("pos: (%f, %f, %f)\n"
			"scl: (%f, %f, %f)\n"
			"rot: (%f, %f, %f)\n"
			"abd: (%f, %f, %f)\n",
			obj.pos[0], obj.pos[1], obj.pos[2],
			obj.scl[0], obj.scl[1], obj.scl[2],
			obj.rot[0], obj.rot[1], obj.rot[2],
			obj.albedo[0], obj.albedo[1], obj.albedo[2]);
}

void				init_object(t_control *ctrl,
							t_mat_3b3 const transforms,
							t_vec_3d const rgb_albedo,
							t_objtype const type)
{
	t_object	obj;
	t_vec_3d	world_pos;
	t_vec_3d	xyz_scaling;
	t_vec_3d	xyz_rot_rad;


	vec3_cpy(world_pos, (t_float *)transforms);
	vec3_cpy(xyz_scaling, (t_float *)transforms + 3);
	vec3_cpy(xyz_rot_rad, (t_float *)transforms + 6);
	obj = build_obj_matrices(world_pos, xyz_scaling, xyz_rot_rad, rgb_albedo);
	obj.type = type;
	obj.intersect = get_inter_func(type);
	obj.get_hnn = get_hnn_func(type);
	ctrl->objlst[(ctrl->objlst_len)++] = obj;
}

//the following function will probably need to be replace by a reader
void				init_objects(t_control *ctrl)
{
ft_printf("{red}init_objects\n");
	ctrl->spotlst_len = 0;

	vec3_set(ctrl->spotlst[ctrl->spotlst_len].pos, 1., 40., 15.);
	vec3_set(ctrl->spotlst[ctrl->spotlst_len].rgb, 1., 1., 1.);
	ctrl->spotlst[ctrl->spotlst_len].intensity = 5000000.;
	++(ctrl->spotlst_len);

	vec3_set(ctrl->spotlst[ctrl->spotlst_len].pos, -20., 20., 5.);
	vec3_set(ctrl->spotlst[ctrl->spotlst_len].rgb, 1., 0., 0.);
	ctrl->spotlst[ctrl->spotlst_len].intensity = 2000000.;
	++(ctrl->spotlst_len);


	ctrl->objlst_len = 0;

	ft_printf("{white}BLACK:\n");
	init_object(ctrl, (t_mat_3b3){0., 0., 0., 1., 2., 3., 0., HALF_PI, 0.}, (t_vec_3d){0.1, 0.1, 0.1}, sphere);

	ft_printf("{red}RED:\n");
	init_object(ctrl, (t_mat_3b3){0., 0., -10., 3., 3., 3., 0., 0., 0.}, (t_vec_3d){1., 0, 0}, sphere);

	ft_printf("{green}GREEN:\n");
	init_object(ctrl, (t_mat_3b3){5., 5., -9., 0.5, 1., 3., HALF_PI / 2., 0., 0.}, (t_vec_3d){0., 1., 0.}, infcylinder);

	ft_printf("{blue}BLUE:\n");
	init_object(ctrl, (t_mat_3b3){-5., 0., -9., 0.4, 1., 0.4, 0., 0., 0.}, (t_vec_3d){0., 0., 1.}, infcone);

	ft_printf("{white}GREY:\n");
	init_object(ctrl, (t_mat_3b3){-5., -20., -9., 1., 1., 1., 0., 0., 0.}, (t_vec_3d){0.5, 0.5, 0.5}, plane);

	ft_printf("{yellow}ORANGE:\n");
	init_object(ctrl, (t_mat_3b3){-10., 0., 1., 10., 1., 2., HALF_PI, HALF_PI / 2., 0.}, (t_vec_3d){1., 0.5, 0.}, disk);

	ft_printf("{magenta}PURPLE:\n");
	init_object(ctrl, (t_mat_3b3){10., 1., 1., 10., 10., 5., HALF_PI, -HALF_PI / 2., 0.}, (t_vec_3d){0.7, 0.1, 0.5}, square);

	ft_printf("{cyan}TURQUOISE:\n");
	init_object(ctrl, (t_mat_3b3){20., 0., 0., 3., 5., 1., HALF_PI / 2, -HALF_PI / 2., HALF_PI}, (t_vec_3d){0.1, 0.5, 0.8}, cylinder);

ft_printf("{red}end{eoc}\n");
}
