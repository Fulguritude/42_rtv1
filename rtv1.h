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
** reltv_pos		: cartesian coordinate of camera with anchor as center
** polar_pos		: zoom/radius, longitude, latitude relative to anchor
** anchor			: origin of polar_pos and reltv_pos
** axis_x			: right vector of cam
** axis_y			: up vector of cam
** axis_z			: "forward" vector of cam,  input eye vector
** hrz_fov			: field-of-view horizontal angle in radians
** w_to_v			: the camera's world-to-view matrix
*/
typedef struct	s_camera
{
	t_vec_3d		world_pos;
//	t_vec_3d		reltv_pos;
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
	t_vec_3d	origin;
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
//	plane,
	cylinder,
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
	t_mat_4b4	n_to_w; //normal_to_world = transpose(inverse(o_to_w))
	t_bool		(*intersect)(t_ray *objray);//, struct s_object obj);
	void		(*get_cnn)(t_vec_3d contact, t_vec_3d normal,
							t_ray const objray);//, struct s_object const obj);
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

/*
GEtting a ray's direction from the FOV angle:
x = j - W/2
y = i - H/2
z = - W/(2*tan(FOV/2))
=> FOV is the angle centered on the camera, and that spans 0 to width - 1
	horizontally.
=> this formula produces the vectors for a camera facing from +z to -z, positive
x on the right, and positive y towards the bottom


I) Lighting effects

Lambertian 'diffuse' lighting model:

a contact point on a sphere is lighted according to the formula (albedo * light_intensity * <N, L>)/ (pi * quaddist(contact, light_spot))

-where albedo is an coefficient between 0 and 1 defining how much light is reflected, and you generally (for physical realism) divide the albedo by PI, hence the pi in the formula
-where N is the unit normal on the object at the contact point
-where L is the unit vector from contact to light_spot)



Object shadows on other objects:
-send a ray from the current contact point being studied (we want its color) to
each light source. If the ray thus created is blocked [ie, d(contact, light) >
d(contact, new_contact)], then darken the point, as it is in shadow.
NB: little trick is to use contact + 0.01 * normal rather than contact, as this
might help avoid noise due to float imprecision


Mirror objects:
-send a light ray, cast only the reflected ray; obtained by the formula:
 v_reflected = v_incident - 2  vec3_scl(dot(v_incident, normal), normal)


Transparent objects:
- every transparent object should have a refraction index (air is 1.)
- every ray has a normal and a tangential componenent so that normal +
	tangential (scaled appropriately) = ray; this is fundamental when building
	a transparency ray 
- T = T_t + T_n, as explained
- T_t = n_i / n_t * (incident - vec3_scl(dot(incident, normal), normal) with n_i
	and n_t the refraction index of the incident milieu and the transparent
	milieu respectively
- T_n = vec3_scale(coef, normal) with coef = -sqrt(1 - (n_i/n_t)² * (1 -
		dot(incident, normal)²)). NB the number inside the square root can be
		negative, in which case the ray is not refracted but reflected


Shadows cast by transparent objects and mirror must be treated differently.


II) Intersection

Sphere/ray intersection
't' is the parameter that allows one to know where on the line generated by a ray they are (cur_point = origin + t * dir)
'center' is the sphere's center, 'radius' its radius

formula : t^2 + 2*t*<dir,origin - center> + quadnorm(origin - center) - radius^2 == 0

roots of the polynomial exist iff there is intersection (so use the discriminant of a degree 2 polynomial), and these roots (t1 and t2) are negative iff the object is behind the camera origin. If the roots are of different sign, then the camera origin is located inside the sphere

*/


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
*/
t_camera		init_cam(t_vec_3d polar_cam_pos, t_vec_3d anchor);
//static void			cam_to_mat(t_mat_4b4 result, t_camera const cam);

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
** intersect.c
*/
t_bool			intersect_ray_sphere(t_ray *ray);//, t_object const sphr);
void			get_cnn_sphere(t_vec_3d contact, t_vec_3d normal,
								t_ray const ray);//, t_object const sphr);

//void		sphere_wtov(t_sphere *sphr, t_camera const cam);

/*
** rays.c
*/
void			cast_rays(t_control *ctrl);


/*
** objects.c
**

*/
void				init_objects(t_control *ctrl);

#endif
