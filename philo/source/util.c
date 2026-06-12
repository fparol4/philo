/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 19:53:02 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/12 19:53:02 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philo.h"

int	util_digit(char *v)
{
	int	i;

	if (!v[0])
		return (0);
	i = 0;
	while (v[i])
	{
		if (!(v[i] >= '0' && v[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int	util_atoi(char *v)
{
	int		i;
	long	result;

	if (!util_digit(v))
		return (-1);
	i = 0;
	result = 0;
	while (v[i] >= '0' && v[i] <= '9')
	{
		result = result * 10 + (v[i] - '0');
		if (result > INT_MAX)
			return (-1);
		i++;
	}
	return ((int)result);
}

int	util_check_props(t_context *context)
{
	if (context->n_philos < 1)
		return (-1);
	if (context->t_die < 1)
		return (-1);
	if (context->t_eat < 1)
		return (-1);
	if (context->t_sleep < 1)
		return (-1);
	if (context->max_eat != -1 && context->max_eat < 1)
		return (-1);
	return (0);
}

int	util_args(char **argv, t_context *context)
{
	context->n_philos = util_atoi(argv[1]);
	context->t_die = util_atoi(argv[2]);
	context->t_eat = util_atoi(argv[3]);
	context->t_sleep = util_atoi(argv[4]);
	if (argv[5])
		context->max_eat = util_atoi(argv[5]);
	else
		context->max_eat = -1;
	if (util_check_props(context))
		return (-1);
	return (0);
}
