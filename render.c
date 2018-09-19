/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 00:08:48 by fulguritu         #+#    #+#             */
/*   Updated: 2018/09/17 00:09:00 by fulguritu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			handle_redraw(void *param)
{
	t_control	*ctrl;

	ctrl = (t_control *)param;
	mlx_put_image_to_window(ctrl->mlx_ptr, ctrl->win_ptr, ctrl->img_ptr, 0, 0);
	return (0);
}

static void		show_debug_info(t_control *ctrl)
{
	char	*str;

	str = NULL;
	ft_asprintf(&str, "polar: (nrm: %.4f, lon: %.4f, lat: %.4f)",
		ctrl->cam.polar_pos[0], ctrl->cam.polar_pos[1], ctrl->cam.polar_pos[2]);
	mlx_string_put(ctrl->mlx_ptr, ctrl->win_ptr, 20, 20, WHITE, str);
	ft_strdel(&str);
	ft_asprintf(&str, "world: (x  : %.4f, y  : %.4f, z  : %.4f)",
		ctrl->cam.world_pos[0], ctrl->cam.world_pos[1], ctrl->cam.world_pos[2]);
	mlx_string_put(ctrl->mlx_ptr, ctrl->win_ptr, 20, 40, WHITE, str);
	ft_strdel(&str);
//	ft_asprintf(&str, "", );
//	mlx_string_put(ctrl->mlx_ptr, ctrl->win_ptr, 20, 60, WHITE, str);
//	ft_strdel(&str);
//	ft_asprintf(&str, "h_ratio: %.4f", ctrl->fdf.h_scale);
//	mlx_string_put(ctrl->mlx_ptr, ctrl->win_ptr,
//		HALF_DRENWIN_WIDTH + 20, 80, WHITE, str);
//	ft_strdel(&str);
//	mlx_string_put(ctrl->mlx_ptr, ctrl->win_ptr, HALF_DRENWIN_WIDTH + 20,
//		100, GREEN, str);
}

void			render(t_control *ctrl)
{
//	t_mat_4b4	w_to_v;

//	mlximg_clear(ctrl);
	ctrl->cam = init_cam(ctrl->cam.polar_pos);
	mlximg_fill(ctrl, BG_COLOR);


	t_objlst	test;
	t_light_src	spot;

	test.len = 5;

	test.objs[0].type = sphere;
	vec3_set(test.objs[0].data.sphere.center, 0., 0., -10.);
	test.objs[0].data.sphere.radius = 3.;
	test.objs[0].data.sphere.color = RED / 4 * 3;
	vec3_set(test.objs[0].data.sphere.albedo, 1., 0., 0.);

	test.objs[1].type = sphere;
	vec3_set(test.objs[1].data.sphere.center, 5., 5., -9.);
	test.objs[1].data.sphere.radius = 2.;
	test.objs[1].data.sphere.color = GREEN / 4 * 3;
	vec3_set(test.objs[1].data.sphere.albedo, 0., 1., 0.);

	test.objs[2].type = sphere;
	vec3_set(test.objs[2].data.sphere.center, 1., -1., -7.75);
	test.objs[2].data.sphere.radius = 1.;
	test.objs[2].data.sphere.color = BLUE / 4 * 3;
	vec3_set(test.objs[2].data.sphere.albedo, 0., 0., 1.);

	test.objs[3].type = sphere;
	vec3_set(test.objs[3].data.sphere.center, 0., 0., 0.);
	test.objs[3].data.sphere.radius = 1.;
	test.objs[3].data.sphere.color = BLACK;// / 4 * 3;
	vec3_set(test.objs[3].data.sphere.albedo, 0.07, 0.07, 0.07);

	test.objs[4].type = sphere;
	vec3_set(test.objs[4].data.sphere.center, -10., -10., 10.);
	test.objs[4].data.sphere.radius = 1.;
	test.objs[4].data.sphere.color = DBG_COLOR;// / 4 * 3;
	vec3_set(test.objs[4].data.sphere.albedo, 1., 0., 1.);

	for (int i = 0; i < test.len; ++i)
		if (test.objs[i].type == sphere)
			sphere_wtov(&(test.objs[i].data.sphere), ctrl->cam);
		else
			exit_error("render: malformed object", 0);

	vec3_set(spot.origin, 1., 10., 0.);
	spot.intensity = 500000.;

	ctrl->spot = spot;
	

	cast_rays(ctrl, test);
	mlx_put_image_to_window(ctrl->mlx_ptr, ctrl->win_ptr, ctrl->img_ptr, 0, 0);

	if (ctrl->debug)
		show_debug_info(ctrl);
	
}
