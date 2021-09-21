#include "../incs/philo.h"

int init(t_life *table, int argc, char **argv)
{
	int i;

	i = 0;
	while (i < 4)
	{
		table->no_philo[i] = atoi(argv[i + 1]);
		i++;
	}
	if (argc == 6)
		table->no_philo[i] = atoi(argv[i + 1]);
	else
		table->no_philo[i] = 0;
	pthread_mutex_init(&table->m_write, NULL);
	pthread_mutex_init(&table->m_somebodydied, NULL);
	pthread_mutex_lock(&table->m_somebodydied);
	table->startTime = getTime();
	table->m_forks = malloc(sizeof(pthread_mutex_t) * table->no_philo[0]);
	if (!table->m_forks)
		return (1);
	i = 0;
	while (i < table->no_philo[0])
	{
		pthread_mutex_init(&table->m_forks[i], NULL);
		i++;
	}
	return(0);
}

void take_fork(t_life *table, int nu)
{
	pthread_mutex_lock(&table->m_forks[nu]);
	message(nu + 1, "take a fork", table);
	pthread_mutex_lock(&table->m_forks[(nu + 1) % table->no_philo[0]]);
	message(nu + 1, "take a fork", table);
}

void eat(t_life *table, int nu)
{
	table->lastTimeEat[nu] = getTime();
	message(nu + 1, "is eating", table);
	ft_usleep(table->no_philo[2]);
}

void go_to_bed(t_life *table, int nu)
{	
	pthread_mutex_unlock(&table->m_forks[nu]);
	pthread_mutex_unlock(&table->m_forks[(nu + 1) % table->no_philo[0]]);
	message(nu + 1, "is sleeping", table);
	ft_usleep(table->no_philo[3]);
	message(nu + 1, "is thinking", table);
}
