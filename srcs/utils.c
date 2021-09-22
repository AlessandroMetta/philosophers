#include "../incs/philo.h"

uint64_t getTime()
{
	static struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void message(int nu, char *msg, t_life *table)
{
	pthread_mutex_lock(&table->m_write);
	printf("[%llu]\tPhilosopher %d %s\n", getTime() - table->startTime, nu, msg);
	pthread_mutex_unlock(&table->m_write);
}

void	ft_usleep(int time)
{
	unsigned long	end;

	end = getTime() + time;
	while (getTime() < end)
		usleep(time);
}
