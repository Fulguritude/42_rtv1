/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_mouse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/18 03:33:15 by fulguritu         #+#    #+#             */
/*   Updated: 2018/08/05 22:12:41 by tduquesn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

/*
** "Key hook"
*/

#if 0
static void		handle_resize_key(int key, t_control *ctrl)
{
	if (key == KEY_PAGE_DN)
		ctrl->cam.polar_pos[0] *= 1.1;
	else if (key == KEY_PAGE_UP)
		ctrl->cam.polar_pos[0] *= 0.9;
	else if (key == KEY_NUMPAD_ADD)
		ctrl->fdf.h_scale += 0.5;
	else if (key == KEY_NUMPAD_SUB)
		ctrl->fdf.h_scale -= 0.5;
}

int				handle_key(int key, void *param)
{
	t_control	*ctrl;

	ctrl = (t_control *)param;
	if (key == KEY_SPACE)
		toggle_proj(ctrl);
	else if (key == KEY_LCTRL)
		toggle_debug(ctrl);
	else if (key == KEY_LEFT)
		ctrl->cam.polar_pos[1] -= 0.2;
	else if (key == KEY_RIGHT)
		ctrl->cam.polar_pos[1] += 0.2;
	else if (key == KEY_DOWN && ctrl->cam.polar_pos[2] - 0.1 > 0)
		ctrl->cam.polar_pos[2] -= 0.1;
	else if (key == KEY_UP && ctrl->cam.polar_pos[2] + 0.1 < PI)
		ctrl->cam.polar_pos[2] += 0.1;
	else if (key == KEY_ESC)
		exit_error("Software closing.", 0);
	else
		handle_resize_key(key, ctrl);
	vec3_polar_to_cartesian(ctrl->cam.world_pos, ctrl->cam.polar_pos);
	vec3_add(ctrl->cam.world_pos, ctrl->cam.world_pos, ctrl->cam.anchor);
	ctrl->cam = init_cam(ctrl->cam.polar_pos);
	handle_redraw(param);
	return (0);
}

/*
** "Mouse hook"
*/

int				handle_mouse(int button, int x, int y, void *param)
{
	t_control			*ctrl;
	static int			click_nb = -1;
	static t_gridpoint	last_click_coord;
	t_gridpoint			tmp;

	++click_nb;
	ctrl = (t_control*)param;
	if (button == L_CLICK)
		mlx_pixel_put(ctrl->mlx_ptr, ctrl->win_ptr, x, y, BLUE);
	else if (button == R_CLICK)
		mlx_pixel_put(ctrl->mlx_ptr, ctrl->win_ptr, x, y, GREEN);
	if (click_nb == 1)
	{
		tmp.x = x + HALF_DRENWIN_WIDTH;
		tmp.y = y + HALF_DRENWIN_HEIGHT;
		bresenham(ctrl, last_click_coord, tmp);
		click_nb = -1;
	}
	last_click_coord.x = x + HALF_DRENWIN_WIDTH;
	last_click_coord.y = y + HALF_DRENWIN_WIDTH;
	mlx_put_image_to_window(ctrl->mlx_ptr, ctrl->win_ptr, ctrl->img_ptr,
							-HALF_DRENWIN_WIDTH, -HALF_DRENWIN_HEIGHT);
	return (0);
}

/*
** "Expose hook"
*/

static void		handle_redraw_helper(t_control *ctrl)
{
	t_gridpoint		vtx1_pixmap_pos;
	t_gridpoint		vtx2_pixmap_pos;
	int				i;

	i = -1;
	while (++i < ctrl->fdf.edge_lst_len)
	{
		vtx1_pixmap_pos = ctrl->fdf.edge_lst[i].vtx_from->proj_pos;
		vtx2_pixmap_pos = ctrl->fdf.edge_lst[i].vtx_to->proj_pos;
		vtx1_pixmap_pos.x += REN_WIDTH / 2;
		vtx1_pixmap_pos.y += REN_HEIGHT / 2;
		vtx2_pixmap_pos.x += REN_WIDTH / 2;
		vtx2_pixmap_pos.y += REN_HEIGHT / 2;
		if (point_in_bounds(vtx1_pixmap_pos.x, vtx1_pixmap_pos.y)
				&& point_in_bounds(vtx2_pixmap_pos.x, vtx2_pixmap_pos.y))
			bresenham(ctrl, vtx1_pixmap_pos, vtx2_pixmap_pos);
	}
	mlx_put_image_to_window(ctrl->mlx_ptr, ctrl->win_ptr, ctrl->img_ptr,
							-HALF_DRENWIN_WIDTH, -HALF_DRENWIN_HEIGHT);
	if (ctrl->debug)
		show_debug_info(ctrl);
}

int				handle_redraw(void *param)
{
	t_control	*ctrl;
	t_mat_4b4	w_to_v;
	t_vec_4d	tmp;
	int			i;

	ctrl = (t_control *)param;
	mlximg_clear(ctrl);
	i = -1;
	cam_to_mat(w_to_v, ctrl->cam);
	while (++i < ctrl->fdf.vtx_lst_len)
	{
		vec3_cpy((t_float *)tmp, ctrl->fdf.vtx_lst[i].world_pos);
		tmp[2] *= ctrl->fdf.h_scale;
		tmp[3] = 1.;
		mat44_app_vec(tmp, w_to_v, tmp);
		vec3_sub(ctrl->fdf.vtx_lst[i].view_pos, tmp, ((t_float *)w_to_v) + 12);
		vec3_scale(ctrl->fdf.vtx_lst[i].view_pos, 10. * ctrl->cam.polar_pos[0],
					ctrl->fdf.vtx_lst[i].view_pos);
		ctrl->fdf.vtx_lst[i].proj_pos =
					ctrl->proj((t_float *)(ctrl->fdf.vtx_lst[i].view_pos));
	}
	handle_redraw_helper(ctrl);
	return (0);
}
#endif

/*
** It is with the mlx_hook function that one creates custom hooks. funct can
** take any number/types of arguments.
**
** int	mlx_hook(t_win_list *win, int x_event, int x_mask, 
**		 int (*funct)(),void *param)
** {
**  win->hooks[x_event].hook = funct;
**  win->hooks[x_event].param = param;
**  win->hooks[x_event].mask = x_mask;
** }
*/

int		handle_mouse_press(int button, int x, int y, void *param)
{
	t_control		*ctrl;
	static int		no_render = 1;
	static t_point	prev_press = {-1, -1};

	ctrl = (t_control *)param;
	if (button == SCROLL_UP || button == SCROLL_DOWN)
	{
		if (prev_press.x != x || prev_press.y != y)
		{
			ctrl->fractol.anchor =
					get_complex_from_point(&(ctrl->fractol), x, y);
			prev_press.x = x;
			prev_press.y = y;
		}
		ctrl->fractol.zoom *= (button == SCROLL_UP) ? 0.9 : 1.1;
		if (no_render >= 3)
		{
			no_render = 0;
			render(ctrl);
		}
		else
			++no_render;
	}
	return (0);
}

int		handle_mouse_release(int button, int x, int y, void *param)
{
	t_control		*ctrl;

	ctrl = (t_control *)param;
	if (button == L_CLICK)
	{
		ctrl->fractol.anchor = get_complex_from_point(&(ctrl->fractol), x, y);
		render(ctrl);
	}
	return (OK);
}

int		handle_mouse_move(int x, int y, void *param)
{
	t_control		*ctrl;
	static t_u32	no_render = 1;
	t_u32			mouse_speed;

	ctrl = (t_control *)param;
	if (!(ctrl->fractol.is_static))
	{
		if (ctrl->fractol.type == newton)
			ctrl->fractol.param =
					get_complex_from_point(&(ctrl->fractol), x, y);
		else
			ctrl->fractol.iter_cpoly.coefs[ctrl->fractol.cur_coef] =
					get_complex_from_point(&(ctrl->fractol), x, y);
		mouse_speed = (ctrl->mouse.x - x) * (ctrl->mouse.x - x)
					+ (ctrl->mouse.y - y) * (ctrl->mouse.y - y);
		if (no_render >= 5 * mouse_speed || (mouse_speed < 4 && no_render >= 5))
		{
			no_render = 0;
			render(ctrl);
		}
		else
			++no_render;
	}
	ctrl->mouse = (t_point){x, y};
	return (OK);
}
