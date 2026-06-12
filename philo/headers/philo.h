#ifndef PHILO_H
# define PHILO_H

/* common */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_context
{
	int				n_philos;
	int				max_eat;
	int				simulation_over;
	int				action_ready;
	int				forks_ready;
	int				philos_ready;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				t_start;
	struct s_philo	*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mtx_action;
} t_context;

typedef struct s_philo
{
	int				id;
	int				eaten;
	int				last_meal;
	pthread_t		thread;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	mtx_state;
	struct s_context *context;
} t_philo;


int	util_digit(char *v);
int	util_atoi(char *v);
int util_check_props(t_context *context);
int util_args(char **argv, t_context *context);

int		time_now(void);
int		time_lapsed(int timestamp);
void	time_sleep(t_philo *philo, int ms);

int context_clear(t_context *context);
int context_init(t_context *context);

int init_forks(t_context *context);
int init_philos(t_context *context);
int init_table(t_context *context);

void	*philo_alone(t_philo *philo);
void	*philo_routine(void *philo_ctx);
int		philo_action_forks(t_philo *philo);
int		philo_action_eat(t_philo *philo);
int		philo_action_sleep(t_philo *philo);
int		philo_action_think(t_philo *philo);
int		print_action(t_philo *philo, char *msg);
int		action_death(t_context *context, int philo_id);
int		action_alleat(t_context *context);
int		philo_lastmeal(t_philo *philo);
int		monitor_satisfied(t_context *context);

int monitor_start(t_context *context);

int simulation_start(t_context *context);
int simulation_over(t_context *context);
int simulation_join(t_context *context);

#endif
