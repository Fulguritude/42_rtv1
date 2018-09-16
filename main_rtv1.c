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

t_bool		intersect_ray_sphere(//t_control *ctrl,
							t_vec_3d contact,
							t_ray const ray,
							t_sphere const sphr)
{
	t_float		delta;
	t_float		b;
	t_float		c;
	t_float		root1;
	t_float		root2;
	t_vec_3d	orig_to_sphr;

	vec3_sub(orig_to_sphr, ray.origin, sphr.center);
	b = 2 * vec3_dot(ray.dir, orig_to_sphr);
	c = vec3_eucl_quadnorm(orig_to_sphr) - sphr.radius * sphr.radius;
	delta = b * b - 4 * c;
	if (delta < 0)
		return (FALSE);
	delta = sqrt(delta);
	root1 = (-b + delta) * 0.5;
	root2 = (-b - delta) * 0.5;
	if (root1 <= 0 || root2 <= 0)
		return (FALSE);
	if (root1 < root2)
		 vec3_scale(contact, root1, ray.dir);
	else
		 vec3_scale(contact, root2, ray.dir);
	vec3_add(contact, contact, ray.origin);
	return (TRUE);
}

void			cast_rays(t_control *ctrl, t_vec_3d origin,
							t_sphere *sphr_lst, int len)
{
	int			i;
	int			j;
	int			obj_k;
	t_ray		ray;
	t_bool		has_inter;
	t_vec_3d	contact_pt;
	t_float		prox_to_cam;

//printf("len: %d\n", len);
	i = 0;
	while (i < REN_H)
	{
		j = 0;
		while (j < REN_W)
		{
			vec3_cpy(ray.origin, origin);
			vec3_set(ray.dir, j - REN_W / 2, i - REN_H / 2, 
							-REN_W / (2 * tan(ctrl->cam.hrz_fov)));
			vec3_eucl_nrmlz(ray.dir, ray.dir);
			obj_k = 0;
			has_inter = FALSE;
			prox_to_cam = 1. / 0.;
			while (obj_k < len)
			{
				if ((has_inter = intersect_ray_sphere(contact_pt, ray, sphr_lst[obj_k])))
				{
					if (vec3_eucl_quaddist(contact_pt, ray.origin) < prox_to_cam)
					{
						prox_to_cam = vec3_eucl_quaddist(contact_pt, ray.origin); //TODO opti
						((t_u32 *)ctrl->img_data)[i * REN_W + j] = sphr_lst[obj_k].color;
					}
				}
				++obj_k;
			}
			++j;
		}
		++i;
	}
}

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
	ctrl.cam = init_cam((t_vec_3d){0., 0., 0.});


	mlx_key_hook(ctrl.win_ptr, handle_key, &ctrl);
	render(&ctrl);


	mlx_loop(ctrl.mlx_ptr);


	return (0);
}
