#include "../headers/philo.h"

int time_now(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return ((int)(time.tv_sec * 1000L + (time.tv_usec / 1000)));
}

int time_lapsed(int timestamp)
{
	return (time_now() - timestamp);
}

void time_sleep(t_philo *philo, int ms)
{
	int	start;
	int	now;

	start = time_now();
	while ((now = time_now()) - start < ms)
	{
		if (simulation_over(philo->context))
			break ;
		usleep(500);
	}
}
