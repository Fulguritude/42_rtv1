/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_key.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/14 20:49:10 by fulguritu         #+#    #+#             */
/*   Updated: 2018/07/14 20:49:13 by fulguritu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

#if 0
static int	handle_key_palette(int key, t_control *ctrl)
{
	int		status;

	status = -1;
	if (key == KEY_NUMPAD_0)
		status = 0;
	else if (key == KEY_NUMPAD_1)
		status = 1;
	else if (key == KEY_NUMPAD_2)
		status = 2;
	else if (key == KEY_NUMPAD_3)
		status = 3;
	else if (key == KEY_NUMPAD_4)
		status = 4;
	else if (key == KEY_NUMPAD_5)
		status = 5;
	else if (key == KEY_NUMPAD_6)
		status = 6;
	else if (key == KEY_NUMPAD_7)
		status = 7;
	else if (key == KEY_NUMPAD_8)
		status = 8;
	else if (key == KEY_NUMPAD_9)
		status = 9;
	ctrl->fractol.palette = status == -1 ? ctrl->fractol.palette : status;
	return (status != -1);
}

static int	handle_key_window(int key, t_control *ctrl)
{
	int					status;
	static t_float		zoom_trans_ratio = 0.04;

	status = 1;
	if (key == KEY_RIGHT)
		ctrl->fractol.anchor.re -= zoom_trans_ratio * ctrl->fractol.zoom;
	else if (key == KEY_LEFT)
		ctrl->fractol.anchor.re += zoom_trans_ratio * ctrl->fractol.zoom;
	else if (key == KEY_UP)
		ctrl->fractol.anchor.im += zoom_trans_ratio * ctrl->fractol.zoom;
	else if (key == KEY_DOWN)
		ctrl->fractol.anchor.im -= zoom_trans_ratio * ctrl->fractol.zoom;
	else if (key == KEY_PAGE_UP)
		ctrl->fractol.zoom *= 1.25;
	else if (key == KEY_PAGE_DN)
		ctrl->fractol.zoom *= 0.8;
	else if (key == KEY_LCTRL)
		ctrl->debug = !(ctrl->debug);
	else if (key == KEY_RCTRL)
		ctrl->render_mode = (ctrl->render_mode + 1) == 3 ?
									0 : ctrl->render_mode + 1;
	else
		status = 0;
	return (status);
}
#endif

int			handle_key(int key, void *param)
{
	t_control			*ctrl;

	ctrl = (t_control *)param;
	if (key == KEY_ESC)
		exit_error("Software closing.", 0);
	else if (key == KEY_LCTRL)
		ctrl->debug = !(ctrl->debug);
	else if (key == KEY_PAGE_DN)
		ctrl->cam.polar_pos[0] *= 1.1;
	else if (key == KEY_PAGE_UP)
		ctrl->cam.polar_pos[0] *= 0.9;
	else if (key == KEY_LEFT)
		ctrl->cam.polar_pos[1] -= 0.2;
	else if (key == KEY_RIGHT)
		ctrl->cam.polar_pos[1] += 0.2;
	else if (key == KEY_DOWN && ctrl->cam.polar_pos[2] - 0.1 > -0.00001)
		ctrl->cam.polar_pos[2] -= 0.1;
	else if (key == KEY_UP && ctrl->cam.polar_pos[2] + 0.1 < PI + 0.00001)
		ctrl->cam.polar_pos[2] += 0.1;
	else if (key == KEY_NUMPAD_0) //backward
		ctrl->cam.anchor[2] += 0.5;
	else if (key == KEY_NUMPAD_2) //forward
		ctrl->cam.anchor[2] -= 0.5;
	else if (key == KEY_NUMPAD_4) //left
		ctrl->cam.anchor[0] -= 0.5;
	else if (key == KEY_NUMPAD_6) //right
		ctrl->cam.anchor[0] += 0.5;
	else if (key == KEY_NUMPAD_5) //down
		ctrl->cam.anchor[1] -= 0.5;
	else if (key == KEY_NUMPAD_8) //up
		ctrl->cam.anchor[1] += 0.5;
	else
		return (1);
//	else if (key == KEY_SPACE)
//	else if (key == KEY_HOME)
//	else if (!has_changed)
//		return (1);
	render(ctrl);
	return (0);
}
