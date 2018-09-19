/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_rtv1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/30 20:27:46 by fulguritu         #+#    #+#             */
/*   Updated: 2018/09/10 03:17:01 by fulguritu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void			exit_error(char *e_msg, int e_no)
{
	int		i;

	if (e_no == 0)
	{
		i = write(2, e_msg, ft_strlen(e_msg));
		i += write(2, "\n", 1);
		exit(i);
	}
	else
		perror(e_msg);
	exit(e_no);
}

/*
** Bits per pixel (32, each color is an int) are immediately converted to
** bytes per pixel.
**
** Anchor is which point of the complex plane is the center of the window.
*/

static void		init_mlx(t_control *ctrl)
{
	if (!(ctrl->mlx_ptr = mlx_init()))
		exit_error("could not init mlx_ptr", 0);
	if (!(ctrl->win_ptr = mlx_new_window(ctrl->mlx_ptr,
				REN_W, REN_H, "RTv1")))
		exit_error("could not init win_ptr", 0);
	if (!(ctrl->img_ptr = mlx_new_image(ctrl->mlx_ptr, REN_W, REN_H)))
		exit_error("could not init img_ptr", 0);
	if (!(ctrl->img_data = mlx_get_data_addr(ctrl->img_ptr, &(ctrl->img_bpp),
										&(ctrl->img_bpl), &(ctrl->endian))))
		exit_error("could not retrieve img data ptr and other info", 0);
	ctrl->img_bpp = ctrl->img_bpp / 8;
	ctrl->img_pixel_nb = REN_W * REN_H;
	ctrl->img_bytelen = ctrl->img_bpp * ctrl->img_pixel_nb;
	ctrl->debug = 1;
}

/*
** MASK_POINTERMOTION returns true when the mouse moves, when no button is
**		clicked;
*/
/*
static void		init_mouse_events(t_control *ctrl)
{
	int		event;
	int		mask;

	mlx_key_hook(ctrl->win_ptr, &handle_key, ctrl);
	mask = MASK_POINTERMOTION | MASK_BUTTON1MOTION |
				MASK_BUTTON2MOTION | MASK_BUTTON3MOTION;
	event = EVENT_MOTIONNOTIFY;
	mlx_hook(ctrl->win_ptr, event, mask, handle_mouse_move, ctrl);
	mask = MASK_BUTTONPRESS;
	event = EVENT_BUTTONPRESS;
	mlx_hook(ctrl->win_ptr, event, mask, handle_mouse_press, ctrl);
	mask = MASK_BUTTONRELEASE;
	event = EVENT_BUTTONRELEASE;
	mlx_hook(ctrl->win_ptr, event, mask, handle_mouse_release, ctrl);
}
*/

int		main(void)
{
	t_control	ctrl;

	init_mlx(&ctrl);
	vec3_set(ctrl.cam.polar_pos, 10., 0., 0.); // 2., 0., HALF_PI)


	mlx_key_hook(ctrl.win_ptr, handle_key, &ctrl);
	mlx_expose_hook(ctrl.win_ptr, handle_redraw, &ctrl);

	render(&ctrl);


	mlx_loop(ctrl.mlx_ptr);


	return (0);
}
