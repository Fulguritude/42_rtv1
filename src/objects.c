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

static t_object		init_object(t_vec_3d const world_pos,
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

void				init_objects(t_control *ctrl)
{
//	t_list		*objlst;
//	t_list		*next_obj;
	t_object	tmp;
	t_vec_3d	world_pos;
	t_vec_3d	xyz_scaling;
	t_vec_3d	xyz_rot_rad;
	t_vec_3d	rgb_albedo;

ft_printf("{red}init_objects\n");
	vec3_set(ctrl->spot.origin, 1., 10., 15.);
	ctrl->spot.intensity = 500000.;
	ctrl->objlst_len = 0;

//TMP

	vec3_cpy(world_pos, (t_vec_3d){0., 0., 0.});
	vec3_cpy(xyz_scaling, (t_vec_3d){2., 1., 1.});
	vec3_cpy(xyz_rot_rad, (t_vec_3d){0., 0., 0.});
	vec3_cpy(rgb_albedo, (t_vec_3d){0.7, 0., 0.7});
	tmp = init_object(world_pos, xyz_scaling, xyz_rot_rad, rgb_albedo);
	tmp.type = sphere;
	tmp.intersect = &intersect_ray_sphere;
	tmp.get_hnn = &get_hnn_sphere;
	ctrl->objlst[(ctrl->objlst_len)++] = tmp;


//BLACK
	ft_printf("{white}BLACK:\n");
	vec3_cpy(world_pos, (t_vec_3d){0., 0., 0.});
	vec3_cpy(xyz_scaling, (t_vec_3d){1., 2., 3.});
	vec3_cpy(xyz_rot_rad, (t_vec_3d){0., 0., 0.});
	vec3_cpy(rgb_albedo, (t_vec_3d){0.1, 0.1, 0.1});
	tmp = init_object(world_pos, xyz_scaling, xyz_rot_rad, rgb_albedo);
	tmp.type = sphere;
	tmp.intersect = &intersect_ray_sphere;
	tmp.get_hnn = &get_hnn_sphere;
	ctrl->objlst[(ctrl->objlst_len)++] = tmp;

//RED
	ft_printf("{red}RED:\n");
	vec3_cpy(world_pos, (t_vec_3d){0., 0., -10.});
	vec3_cpy(xyz_scaling, (t_vec_3d){3., 3., 3.});
	vec3_cpy(xyz_rot_rad, (t_vec_3d){0., 0., 0.});
	vec3_cpy(rgb_albedo, (t_vec_3d){1., 0., 0.});
	tmp = init_object(world_pos, xyz_scaling, xyz_rot_rad, rgb_albedo);
	tmp.type = sphere;
	tmp.intersect = &intersect_ray_sphere;
	tmp.get_hnn = &get_hnn_sphere;
	ctrl->objlst[(ctrl->objlst_len)++] = tmp;

//GREEN
	ft_printf("{green}GREEN:\n");
	vec3_cpy(world_pos, (t_vec_3d){5., 5., -9.});
	vec3_cpy(xyz_scaling, (t_vec_3d){0.5, 1., 2.});
	vec3_cpy(xyz_rot_rad, (t_vec_3d){0., 0., 0.});
	vec3_cpy(rgb_albedo, (t_vec_3d){0., 1., 0.});
	tmp = init_object(world_pos, xyz_scaling, xyz_rot_rad, rgb_albedo);
	tmp.type = sphere;
	tmp.intersect = &intersect_ray_sphere;
	tmp.get_hnn = &get_hnn_sphere;
	ctrl->objlst[(ctrl->objlst_len)++] = tmp;

//BLUE
	ft_printf("{blue}BLUE:\n");
	vec3_cpy(world_pos, (t_vec_3d){1., -1., -7.75});
	vec3_cpy(xyz_scaling, (t_vec_3d){2., 1, 1.});
	vec3_cpy(xyz_rot_rad, (t_vec_3d){0., 0., 0.});
	vec3_cpy(rgb_albedo, (t_vec_3d){0., 0., 1.});
	tmp = init_object(world_pos, xyz_scaling, xyz_rot_rad, rgb_albedo);
	tmp.type = sphere;
	tmp.intersect = &intersect_ray_sphere;
	tmp.get_hnn = &get_hnn_sphere;
	ctrl->objlst[(ctrl->objlst_len)++] = tmp;

//PURPLE
	ft_printf("{magenta}PURPLE:\n");
	vec3_cpy(world_pos, (t_vec_3d){0.5, 0.5, -0.5});
	vec3_cpy(xyz_scaling, (t_vec_3d){1., 1., 1.});
	vec3_cpy(xyz_rot_rad, (t_vec_3d){0., 0., 0.});
	vec3_cpy(rgb_albedo, (t_vec_3d){1., 0., 1.});
	tmp = init_object(world_pos, xyz_scaling, xyz_rot_rad, rgb_albedo);
	tmp.type = sphere;
	tmp.intersect = &intersect_ray_sphere;
	tmp.get_hnn = &get_hnn_sphere;
	ctrl->objlst[(ctrl->objlst_len)++] = tmp;

ft_printf("{red}end{eoc}\n");

}
