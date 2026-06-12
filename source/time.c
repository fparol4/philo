#include "../headers/philo.h"

long time_now(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000L + (time.tv_usec / 1000));
}

long time_lapsed(long timestamp)
{
	return (time_now() - timestamp);
}

void time_sleep(long ms)
{
	long now;
	long start;

	start = time_now();
	while (1)
	{
		now = time_now();
		if (now - start >= ms)
			break;
		usleep(50);
	}
}
