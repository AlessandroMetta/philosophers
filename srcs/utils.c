#include "../incs/philo.h"

uint64_t get_time()
{
	static struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void message(int nu, char *msg, t_args *table)
{
	pthread_mutex_lock(&table->m_write);
	printf("[%llu]\tPhilosopher %d %s\n", get_time() - table->start_time, nu, msg);
	pthread_mutex_unlock(&table->m_write);
}

void	ft_usleep(uint64_t time_in_ms)
{
	uint64_t	start_time;

	start_time = 0;
	start_time = get_time();
	while ((get_time() - start_time) < time_in_ms)
		usleep(time_in_ms / 10);
}
