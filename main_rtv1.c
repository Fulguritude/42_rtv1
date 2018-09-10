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

t_float		ft_eucl_quaddist(t_vec_3d const v1, t_vec3d const v2)
{
	t_vec3d		tmp;
	t_float		result;

	tmp = vec3d_sub(v1, v2);
	return (vec3d_quadnorm(tmp));
}

int		intersect_ray_sphere(//t_control *ctrl,
							t_vec_3d contact,
							t_ray const ray,
							t_sphere const sphr)
{
	t_float		delta;
	t_float		b;
	t_float		c;
	t_float		root1;
	t_float		root2;
	t_vec_3d		orig_to_sphr;

	vec3d_sub(orig_to_sphr, ray.origin, sphr.center);
	b = (2 * vec3d_dot(ray.dir, orig_to_sphr);
	c = vec3d_quadnorm(orig_to_sphr) - sphr.radius * sphr.radius;
	delta = b * b - 4 * c;
	if (delta < 0)
		return (FALSE);
	delta = sqrt(delta);
	root1 = (-b + delta) / 2.;
	root2 = (-b - delta) / 2.;
	if (root1 <= 0 || root2 <= 0)
		return (FALSE);
	if (root1 < root2)
		 vec3_scale(contact, root1, ray.dir);
	else
		 vec3_scale(contact, root2, ray.dir);
	vec3_add(contact, contact, ray.origin);
	return (TRUE);
}

t_bool	cast_rays(t_control *ctrl, t_vec_3d origin, t_sphere *sphr_lst, int len)
{
	int			i;
	int			j;
	int			obj_k;
	t_ray		ray;
	t_bool		has_inter;
	t_vec_3d	contact_pt;

	i = 0;
	while (i < REN_H)
	{
		j = 0;
		while (j < REN_W)
		{
			while (k < len)
			{
				vec3_set(ray.origin, 0., 0., 0.);
				vec3_set(ray.dir, j - REN_W/2, i - REN_H/2, -W/(2*tan(ctrl->hrz_fov)));
				if (intersect_ray_sphere(contact_pt, ray, sphr_lst[k]))
					ctrl->img_data
				else

					
				++k;
			}
			++j;
		}
		++i:
	}
	
}



int		main(void)
{
	t_control	ctrl;
	t_sphere	test;
	t_light_src	spot;

	ctrl.cam_pos = (t_vec_3d){0., 0., 0.};
	ctrl.hrz_fov = 1.;

	test.center = (t_vec_3d){0., 0., -10.};
	test.radius = 2.;
	test.color = 0x00FF0000;
	test.albedo = 2.; //?

	spot.origin = (t_vec_3d){1., 10., 0.};
	spot.intensity = 10000.;

	

}
