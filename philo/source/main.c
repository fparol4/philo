/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 19:53:02 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/12 19:53:02 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philo.h"

static int	main_args(int argc, char **argv, t_context *context)
{
	if (argc < 5 || argc > 6)
	{
		printf("Error: Invalid number of arguments\n");
		printf("Usage: ./philo <num> <die> <eat> <sleep> [must_eat]\n");
		return (1);
	}
	if (util_args(argv, context))
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_context	context;

	context = (t_context){0};
	if (main_args(argc, argv, &context))
		return (1);
	if (context_init(&context))
		return (context_clear(&context));
	if (simulation_start(&context))
		return (context_clear(&context));
	monitor_start(&context);
	simulation_join(&context);
	return (context_clear(&context));
}
