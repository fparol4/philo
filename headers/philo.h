#ifndef PHILO_H
# define PHILO_H

/* common */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_context
{
	int n_philos;
	int max_eat;
	int	sim_over;
	long t_die;
	long t_eat;
	long t_sleep;
	long t_start;
	struct s_philo *philos;
	pthread_mutex_t *forks;
	pthread_mutex_t mtx_action;
} t_context;

typedef struct s_philo
{
	int	id;
	int eaten;
	long last_meal;
	pthread_t thread;
	pthread_mutex_t l_fork;
	pthread_mutex_t r_fork;
	struct s_context *context;
} t_philo;


int	util_digit(char *v);
int	util_atoi(char *v);
int util_check_props(t_context *context);
int util_args(char **argv, t_context *context);

long time_now(void);
long time_lapsed(long timestamp);
void time_sleep(long ms);

int context_clear(t_context *context);
int context_init(t_context *context);

int init_forks(t_context *context);
int init_philos(t_context *context);
int init_table(t_context *context);

void *philo_alone(t_philo *philo);
void *philo_routine(void *philo_ctx);

int monitor_start(t_context *context);

int sim_start(t_context *context);

#endif
