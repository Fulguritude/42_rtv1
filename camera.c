/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 00:31:44 by fulguritu         #+#    #+#             */
/*   Updated: 2018/07/19 15:23:48 by tduquesn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void		cam_to_mat(t_mat_4b4 result, t_camera const cam)
{
	t_mat_3b3	tmp;
	t_vec_3d	v;

	mat33_set(tmp, cam.axis_x, cam.axis_y, cam.axis_z);
	result[0] = tmp[0];
	result[1] = tmp[1];
	result[2] = tmp[2];
	result[3] = 0.;
	result[4] = tmp[3];
	result[5] = tmp[4];
	result[6] = tmp[5];
	result[7] = 0.;
	result[8] = tmp[6];
	result[9] = tmp[7];
	result[10] = tmp[8];
	result[11] = 0.;
	vec3_sub(v, cam.world_pos, cam.anchor); //TODO maybe needs fixing
	result[12] = v[0];
	result[13] = v[1];
	result[14] = v[2];
	result[15] = 1.;
	mat44_inv(result, result);
}

t_camera		init_cam(t_vec_3d polar_cam_pos)
{
	t_camera	result;

	vec3_set(result.anchor, 0., 0., 0.);
	vec3_cpy(result.polar_pos, polar_cam_pos);
	vec3_polar_to_cartesian(result.world_pos, polar_cam_pos);
//	vec3_add(ctrl->cam.reltv_pos, ctrl->cam.world_pos, ctrl->cam.anchor);
	vec3_sub(result.axis_z, result.world_pos, result.anchor);
	vec3_eucl_nrmlz(result.axis_z, result.axis_z);
	vec3_set(result.axis_y, 0., 1., 0.);
	vec3_cross(result.axis_x, result.axis_y, result.axis_z);
	vec3_eucl_nrmlz(result.axis_x, result.axis_x);
	vec3_cross(result.axis_y, result.axis_x, result.axis_z);
//	vec3_eucl_nrmlz(result.axis_x, result.axis_x);
//	vec3_scale(result.axis_x, 1., result.axis_x);

printf("cam:\n\tx: (%.5f, %.5f, %.5f)\n\ty: (%.5f, %.5f, %.5f)\n\tz: (%.5f, %.5f, %.5f)\n\n",
result.axis_x[0], result.axis_x[1], result.axis_x[2],
result.axis_y[0], result.axis_y[1], result.axis_y[2],
result.axis_z[0], result.axis_z[1], result.axis_z[2]);

	result.hrz_fov = 0.8;
	cam_to_mat(result.w_to_v, result);


printf("inv_cam:\n\tx: (%.5f, %.5f, %.5f, %.5f)\n\ty: (%.5f, %.5f, %.5f, %.5f)\n\tz: (%.5f, %.5f, %.5f, %.5f)\n\tt: (%.5f, %.5f, %.5f, %.5f)\n\n",
result.w_to_v[0], result.w_to_v[1], result.w_to_v[2], result.w_to_v[3],
result.w_to_v[4], result.w_to_v[5], result.w_to_v[6], result.w_to_v[7],
result.w_to_v[8], result.w_to_v[9], result.w_to_v[10], result.w_to_v[11],
result.w_to_v[12], result.w_to_v[13], result.w_to_v[14], result.w_to_v[15]);

	return (result);
}
