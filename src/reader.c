/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 06:00:02 by fulguritu         #+#    #+#             */
/*   Updated: 2018/10/01 13:49:04 by fulguritu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void			r_rt_f_read_vec3_line(t_vec_3d res, int fd)
{
	char		**strls;
	int			status;
	char		*line;

	line = NULL;
	if ((status = get_next_line(fd, &line)) == ERR_RD)
		exit_error("r_rt_f_read_vec3_line: gnl: error reading line", errno);
	else if (status == EOF_RD)
		exit_error("r_rt_f_read_vec3_line: EOF reached prematurely", 0);
	strls = ft_split(line, " \t");
	if (ft_ptrarrlen(strls) < 3)
		exit_error("r_rt_f_read_vec3_line: line must have 3 params", 0);
	res[0] = ft_atolf(strls[0]);
	res[1] = ft_atolf(strls[1]);
	res[2] = ft_atolf(strls[2]);
	ft_strlsdel(&strls);
	ft_strdel(&line);
	if (res[0] != res[0] || res[1] != res[1] || res[2] != res[2])
		exit_error("r_rt_f_read_vec3_line: param in vec3 line is NaN", 0);
}

t_bool		ft_float_in_interval(t_float x, t_float start, t_float end)
{
	return (start <= x && x <= end);
}

static void			r_rt_f_setup_light(t_control *ctrl, int fd)
{
	char	*line;
	t_float	tmp;
	int		status;

	line = NULL;
//printf("\tlight1\n");
	r_rt_f_read_vec3_line(ctrl->spotlst[ctrl->spotlst_len].pos, fd);
//printf("\tlight2\n");
	r_rt_f_read_vec3_line(ctrl->spotlst[ctrl->spotlst_len].rgb, fd);
	if (!ft_float_in_interval(ctrl->spotlst[ctrl->spotlst_len].rgb[0], 0., 1.)
	|| !ft_float_in_interval(ctrl->spotlst[ctrl->spotlst_len].rgb[1], 0., 1.)
	|| !ft_float_in_interval(ctrl->spotlst[ctrl->spotlst_len].rgb[2], 0., 1.))
		exit_error("r_rt_f_setup_light: light rgb should be in [0., 1.]^3", 0);
	if ((status = get_next_line(fd, &line)) == ERR_RD)
		exit_error("r_rt_f_setup_light: gnl: error reading line", errno);
	else if (status == EOF_RD)
		exit_error("r_rt_f_setup_light: EOF reached prematurely", 0);
	tmp = ft_atolf(line);
	if (tmp != tmp)
		exit_error("r_rt_f_setup_light: light intensity is NaN", 0);
	ft_strdel(&line);
	ctrl->spotlst[ctrl->spotlst_len].intensity = tmp;
	++(ctrl->spotlst_len);
	if (ctrl->spotlst_len > MAX_LGT_NB)
		exit_error("r_rt_f_setup_light: MAX_LGT_NB exceeded", 0);
}

static void			r_rt_f_set_obj(t_control *ctrl, int fd, t_objtype type)
{
	char	*line;

	line = NULL;
//printf("\tobj1\n");
	r_rt_f_read_vec3_line(ctrl->objlst[ctrl->objlst_len].pos, fd);
//printf("\tobj2\n");	
	r_rt_f_read_vec3_line(ctrl->objlst[ctrl->objlst_len].scl, fd);
//printf("\tobj3\n");	
	r_rt_f_read_vec3_line(ctrl->objlst[ctrl->objlst_len].rot, fd);
//printf("\tobj4\n");	
	r_rt_f_read_vec3_line(ctrl->objlst[ctrl->objlst_len].albedo, fd);
//printf("\tobj5\n");	
	r_rt_f_read_vec3_line(ctrl->objlst[ctrl->objlst_len].specul, fd);
	if (!ft_float_in_interval(ctrl->objlst[ctrl->objlst_len].albedo[0], 0., 1.)
	|| !ft_float_in_interval(ctrl->objlst[ctrl->objlst_len].albedo[1], 0., 1.)
	|| !ft_float_in_interval(ctrl->objlst[ctrl->objlst_len].albedo[2], 0., 1.))
		exit_error("r_rt_f_set_obj: obj albedo should be in [0., 1.] ^ 3", 0);
	build_obj(&(ctrl->objlst[ctrl->objlst_len]), type);
print_object(ctrl->objlst[ctrl->objlst_len]);
	++(ctrl->objlst_len);
	ft_strdel(&line);
	if (ctrl->objlst_len > MAX_OBJ_NB)
		exit_error("r_rt_f_set_obj: MAX_OBJ_NB exceeded", 0);
}

static void			read_rt_file_context_line(t_control *ctrl,
												char const *str, int fd)
{
	if (ft_strnequ(str, "LIGHT", 5))
		r_rt_f_setup_light(ctrl, fd);
	else if (ft_strnequ(str, "PLANE", 5))
		r_rt_f_set_obj(ctrl, fd, plane);
	else if (ft_strnequ(str, "SPHERE", 6))
		r_rt_f_set_obj(ctrl, fd, sphere);
	else if (ft_strnequ(str, "INFCONE", 7))
		r_rt_f_set_obj(ctrl, fd, infcone);
	else if (ft_strnequ(str, "INFCYLINDER", 11))
		r_rt_f_set_obj(ctrl, fd, infcylinder);
	else if (ft_strnequ(str, "CYLINDER", 8))
		r_rt_f_set_obj(ctrl, fd, cylinder);
	else if (ft_strnequ(str, "DISK", 4))
		r_rt_f_set_obj(ctrl, fd, disk);
	else if (ft_strnequ(str, "SQUARE", 6))
		r_rt_f_set_obj(ctrl, fd, square);
	else if (ft_strnequ(str, "CAMERA", 6))
	{
		r_rt_f_read_vec3_line(ctrl->cam.anchor, fd);
		r_rt_f_read_vec3_line(ctrl->cam.polar_pos, fd);		
	}
	else
		exit_error("read_rt_file_context_line: invalid context str error", 0);
}

void				read_rt_file(t_control *ctrl, char const *fpath)
{
	char		*str;
	int			fd;
	int			i;
	int			status;

	if ((fd = open(fpath, O_RDONLY)) == -1)
		exit_error("open: file couldn't be opened", errno);
	i = -1;
	while (++i < MAX_FILE_LN_NB)
	{
ft_printf("entity %d\n", i);
		str = NULL;
		if ((status = get_next_line(fd, &str)) == ERR_RD)
			exit_error("read_rt_file: gnl: error while reading line", errno);
		else if (status == EOF_RD || !str)
			break ;
		read_rt_file_context_line(ctrl, str, fd);
		if ((status = get_next_line(fd, &str)) == EOF_RD)
			break ;
		if (status == ERR_RD || !ft_strequ(str, ""))
			exit_error("r_rt_f_setup_light: missing newline after obj"
						" or light block", errno);
		free(str);
	}
	if (ctrl->objlst_len == 0 || ctrl->objlst_len == 0)
		exit_error("read_rt_file: missing object or light in file", 0);
	if (close(fd) == -1)
		exit_error("close: file closed improperly", errno);
}
