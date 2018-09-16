/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat33.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fulguritude <marvin@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/14 02:12:30 by fulguritu         #+#    #+#             */
/*   Updated: 2018/06/14 02:12:33 by fulguritu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_algebra.h"

void			mat33_set(t_mat_3b3 result, t_vec_3d const v0,
							t_vec_3d const v1, t_vec_3d const v2)
{
	result[0] = v0[0];
	result[1] = v0[1];
	result[2] = v0[2];
	result[3] = v1[0];
	result[4] = v1[1];
	result[5] = v1[2];
	result[6] = v2[0];
	result[7] = v2[1];
	result[8] = v2[2];
}

/*
**	[ 0  3  6 ]
**  [ 1  4  7 ]
**  [ 2  5  8 ]
*/

t_float			mat33_det(t_mat_3b3 const m)
{
	return (m[0] * m[4] * m[8] + m[1] * m[5] * m[6] + m[2] * m[3] * m[7]
			- m[2] * m[4] * m[6] - m[1] * m[3] * m[8] - m[0] * m[5] * m[7]);
}

/*
** M^-1 = 1/det * adj(m) and adj(m) = comat(m^transpose)
** TODO debug
*/

void			mat33_inv(t_mat_3b3 result, t_mat_3b3 const m)
{
	t_float		det;
	t_mat_3b3	tmp;

	if ((det = mat33_det(m)) == 0.)
	{
		write(2, "non invertible matrix error\n", 28);
		ft_bzero(result, T_MAT33_SIZE);
		return ;
	}
	tmp[0] = m[4] * m[8] - m[5] * m[7];
	tmp[3] = m[5] * m[6] - m[3] * m[8];
	tmp[6] = m[3] * m[7] - m[4] * m[6];
	tmp[1] = m[2] * m[7] - m[1] * m[8];
	tmp[4] = m[0] * m[8] - m[2] * m[6];
	tmp[7] = m[1] * m[6] - m[0] * m[7];
	tmp[2] = m[1] * m[5] - m[2] * m[4];
	tmp[5] = m[2] * m[3] - m[0] * m[5];
	tmp[8] = m[0] * m[4] - m[1] * m[3];
	ft_memcpy(result, tmp, sizeof(tmp));
	mat33_scale(result, 1. / det, result);
}

void			mat33_transpose(t_mat_3b3 result, t_mat_3b3 const src)
{
	t_float		tmp;

	tmp = src[1];
	result[1] = src[3];
	result[3] = tmp;
	tmp = src[2];
	result[2] = src[6];
	result[6] = tmp;
	tmp = src[7];
	result[7] = src[5];
	result[5] = tmp;
	result[0] = src[0];
	result[4] = src[4];
	result[8] = src[8];
}
