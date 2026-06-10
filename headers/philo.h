#ifndef PHILO_H
# define PHILO_H

/* external */
#include <stdio.h>
#include <stdlib.h>

typedef struct s_context
{
	int n_philos;
	int t_die;
	int t_eat;
	int t_sleep;
	int m_eat;
} t_context;

int	util_digit(char *v);
int	util_atoi(char *v);
int util_check_props(t_context *context);
int util_args(char **argv, t_context *context);

#endif
