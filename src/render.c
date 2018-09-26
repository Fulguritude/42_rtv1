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

int				handle_redraw(void *param)
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
	ft_asprintf(&str, "anchr: (x  : %.4f, y  : %.4f, z  : %.4f)",
		ctrl->cam.anchor[0], ctrl->cam.anchor[1], ctrl->cam.anchor[2]);
	mlx_string_put(ctrl->mlx_ptr, ctrl->win_ptr, 20, 20, WHITE, str);
	ft_strdel(&str);
	ft_asprintf(&str, "polar: (nrm: %.4f, lon: %.4f, lat: %.4f)",
		ctrl->cam.polar_pos[0], ctrl->cam.polar_pos[1], ctrl->cam.polar_pos[2]);
	mlx_string_put(ctrl->mlx_ptr, ctrl->win_ptr, 20, 40, WHITE, str);
	ft_strdel(&str);
	ft_asprintf(&str, "world: (x  : %.4f, y  : %.4f, z  : %.4f)",
		ctrl->cam.world_pos[0], ctrl->cam.world_pos[1], ctrl->cam.world_pos[2]);
	mlx_string_put(ctrl->mlx_ptr, ctrl->win_ptr, 20, 60, WHITE, str);
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
	mlximg_fill(ctrl, BG_COLOR);
	ctrl->cam = init_cam(ctrl->cam.polar_pos, ctrl->cam.anchor);

	cast_rays(ctrl);
	mlx_put_image_to_window(ctrl->mlx_ptr, ctrl->win_ptr, ctrl->img_ptr, 0, 0);

	if (ctrl->debug)
		show_debug_info(ctrl);
	
}
