/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 14:06:02 by ebalana-          #+#    #+#             */
/*   Updated: 2025/07/03 14:06:53 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHPERE_H
# define SHPERE_H

# include "miniRT.h"
# define SPHERES_COUNT 3

typedef struct s_sphere
{
	t_vec3	center;
	double	radius;
}	t_sphere;

#endif