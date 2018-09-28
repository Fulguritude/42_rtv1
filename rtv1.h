/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/30 17:34:32 by fulguritu         #+#    #+#             */
/*   Updated: 2018/09/10 03:17:05 by fulguritu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

/*
** NB : convention used is +x for right axis, +y for up axis, +z for back axis
*/

# include "libft/hdr/libft.h"
# include "libft/hdr/libft_algebra.h"
# include "libft/hdr/ft_printf.h"
# include "mlx_event_lin.h"
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdio.h>
# include <mlx.h>
# include <errno.h>

# define REN_W		480
# define REN_H		360

# define BLACK			0x000000
# define RED			0xFF0000
# define GREEN			0x00FF00
# define BLUE			0x0000FF
# define WHITE			0xFFFFFF
# define DBG_COLOR		0x5500BB
# define BG_COLOR		0x00BB88
# define NO_INTER		0xFF000000

# define MAX_OBJ_NB		32

typedef struct	s_point
{
	int		x;
	int		y;
}				t_point;

typedef t_u32	t_color;

/*
** CAMERA
**
** world_pos		: cartesian coordinate of camera in world 
**						(world = anchor + relative)
** reltv_pos		: cartesian coordinate of camera with anchor as center
** polar_pos		: zoom/radius, longitude, latitude relative to anchor
** anchor			: origin of polar_pos and reltv_pos
** axis_x			: right vector of cam
** axis_y			: up vector of cam
** axis_z			: "forward" vector of cam,  input eye vector
** hrz_fov			: field-of-view horizontal angle in radians
** w_to_c			: the camera's world-to-view/cam matrix
** c_to_w			: the camera's view/cam-to-world matrix (useful to put
**						 rays in world space)
*/
typedef struct	s_camera
{
	t_vec_3d		world_pos;
	t_vec_3d		reltv_pos;
	t_vec_3d		polar_pos;
	t_vec_3d		anchor;
	t_vec_3d		axis_x;
	t_vec_3d		axis_y;
	t_vec_3d		axis_z;
	t_float			hrz_fov;
	t_mat_4b4		c_to_w;
	t_mat_4b4		w_to_c;
}				t_camera;

/*
** RAYS AND SHADING
**
** Rays can be interpreted in any space.
** origin + scale(t, dir) = end of the current ray
*/
typedef struct	s_ray
{
	t_vec_3d	pos;
	t_vec_3d	dir;
	t_float		t;
}				t_ray;

typedef struct	s_light_src
{
	t_vec_3d	origin;
	t_float		intensity;
}				t_light_src;

/*
** PRIMITIVES
**
** All primitives are considered to be centered near the origin with default
** unit dimensions.
*/
typedef	enum	e_objtype
{
	null_obj,
	sphere,
	plane,
	disk,
	square,
	infcylinder,
	cylinder,
	infcone,
	cone,
	cube
}				t_objtype;

/*
** This struct is used to translate, rotate and scale our object into position
**	in the world space.
**
** So that normals be calculated easily, rays are converted into object space
**	first, then compared with the basic primitive. The object_to_world matrix
**	is the succession of: scaling, then rotating, then translating, applied
**  to the basic primitive.
**
** type			: an enum to sort each geometric primitive, see above
** pos			: position in world space
** scl			: (x, y, z) scaling factors as a vector (tmp diagonal matrix)
** rot			: model angles of rotation (in radians) around (resp.)
**					the x-, y-, and z-axes
** albedo		: a vector of float from 0. to 1. representing how much each
**					object reflects each primary color of light,
**					respectively (r, g, b);
** unit_w_to_o	: takes a unit vector from world space (that shouldn't be
**					translated) to a non-unit vector in object space
** unit_o_to_w	: takes a non-unit vector from object space (that shouldn't be
**					translated) to corresponding unit vector in world space
** o_to_w		: takes a vector that should be translated from object space
**					to world space
** w_to_o		: takes a vector that should be translated from world space
**					to object space
** n_to_w		: takes a vector that should be considered a normal of our
**					object from object space to world space, in practice,
**					normal_to_world = transpose(inverse(o_to_w))
** intersect	: the intersection function, set when type is set
** get_hnn		: the function that returns the appropriate hitpos and normal
**					of the ray on the object; note that the appropriate t
**					parameter should already be set before this function is
**					called
*/
typedef struct	s_object
{
	t_objtype	type;
	t_vec_3d	pos;
	t_vec_3d	scl;
	t_vec_3d	rot;
	t_vec_3d	albedo;
	t_mat_4b4	unit_o_to_w;
	t_mat_4b4	unit_w_to_o;
	t_mat_4b4	o_to_w;
	t_mat_4b4	w_to_o;
	t_mat_4b4	n_to_w;
	t_bool		(*intersect)(t_ray *objray);
	void		(*get_hnn)(t_vec_3d contact, t_vec_3d normal,
							t_ray const objray);
}				t_object;

/*
** SOFTWARE CONTROL TABLE
*/
typedef struct	s_control
{
	void			*mlx_ptr;
	void			*win_ptr;
	void			*img_ptr;
	int				img_bpp;
	int				img_bpl;
	int				img_bytelen;
	int				img_pixel_nb;
	int				endian;
	char			*img_data;
	t_camera		cam;
	int				debug;
	t_point			mouse;
	t_object		objlst[MAX_OBJ_NB];
	int				objlst_len;
	t_light_src		spot;
}				t_control;

void			exit_error(char *e_msg, int e_no);

/*
** image_utils.c
*/
int				point_in_bounds(int x, int y);
void			mlximg_setpixel(t_control *ctrl, int color, int x, int y);
void			mlximg_fill(t_control *ctrl, t_u32 val);
void			mlximg_clear(t_control *ctrl);

/*
** camera.c
**
** static void		build_cam_matrices(t_mat_4b4 result, t_camera const cam);
*/
t_camera		init_cam(t_vec_3d polar_cam_pos, t_vec_3d anchor);

/*
** render.c
*/
int				handle_redraw(void *param);
void			render(t_control *ctrl);

/*
** coordinates.c
*/
void			vec3_polar_to_cartesian(t_vec_3d result, t_vec_3d const src);
void			vec3_cartesian_to_polar(t_vec_3d result, t_vec_3d const src);

/*
** event_key.c
*/
int				handle_key(int key, void *param);

/*
** rays.c
*/
void			cast_rays(t_control *ctrl);

/*
** objects.c
*/
void			init_objects(t_control *ctrl);

/*
** primitive_utils.c
*/
t_bool			get_realroots_quadpoly(t_float *root1, t_float *root2,
									t_vec_3d const quadpoly);
void			get_ray_hitpos(t_vec_3d hitpos, t_ray const objray);

/*
** primitives_2d.c
*/
t_bool			intersect_ray_plane(t_ray *objray);
t_bool			intersect_ray_disk(t_ray *objray);
t_bool			intersect_ray_square(t_ray *objray);
void			get_hnn_plane(t_vec_3d hitpos, t_vec_3d normal,
							t_ray const objray);

/*
** sphere.c
*/
t_bool			intersect_ray_sphere(t_ray *ray);
void			get_hnn_sphere(t_vec_3d hitpos, t_vec_3d normal,
								t_ray const ray);

/*
** cone.c
*/
t_bool			intersect_ray_infcone(t_ray *ray);
void			get_hnn_infcone(t_vec_3d hitpos, t_vec_3d normal,
								t_ray const ray);
/*
** cylinder.c
*/
t_bool			intersect_ray_infcylinder(t_ray *ray);
void			get_hnn_infcylinder(t_vec_3d hitpos, t_vec_3d normal,
								t_ray const ray);

#endif
