/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebalana- <ebalana-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 12:14:45 by dcampas-          #+#    #+#             */
/*   Updated: 2025/09/23 16:37:09 by ebalana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	parse_integer_part(const char *str, int *i)
{
	double	result;

	result = 0.0;
	while (str[*i] >= '0' && str[*i] <= '9')
		result = result * 10.0 + (str[(*i)++] - '0');
	return (result);
}

double	parse_decimal_part(const char *str, int *i)
{
	double	result;
	double	decimal;

	result = 0.0;
	decimal = 0.1;
	while (str[*i] >= '0' && str[*i] <= '9')
	{
		result += (str[(*i)++] - '0') * decimal;
		decimal *= 0.1;
	}
	return (result);
}

double	ft_atof(const char *str)
{
	double	result;
	double	sign;
	int		i;

	result = 0.0;
	sign = 1.0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1.0;
	result += parse_integer_part(str, &i);
	if (str[i] == '.')
	{
		i++;
		result += parse_decimal_part(str, &i);
	}
	return (result * sign);
}
