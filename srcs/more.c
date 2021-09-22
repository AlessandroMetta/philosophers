#include "../incs/philo.h"

int init(t_args *table, int argc, char **argv)
{
	int i;

	i = 0;
	table->num_of_philo = (uint64_t)atoi(argv[1]);
	table->time_to_die = (uint64_t)atoi(argv[2]);
	table->time_to_eat = (uint64_t)atoi(argv[3]);
	table->time_to_sleep = (uint64_t)atoi(argv[4]);
	if (argc == 6)
		table->meal_ammount = atoi(argv[5]);
	else
		table->meal_ammount = 0;
	pthread_mutex_init(&table->m_write, NULL);
	pthread_mutex_init(&table->m_somebodydied, NULL);
	pthread_mutex_lock(&table->m_somebodydied);
	table->start_time = get_time();
	table->m_forks = malloc(sizeof(pthread_mutex_t) * table->num_of_philo);
	if (!table->m_forks)
		return (1);
	while (i < (int)table->num_of_philo)
	{
		pthread_mutex_init(&table->m_forks[i], NULL);
		i++;
	}
	return(0);
}

void take_fork(t_args *table, int nu)
{
	pthread_mutex_lock(&table->m_forks[nu]);
	message(nu + 1, "has taken a fork", table);
	pthread_mutex_lock(&table->m_forks[(nu + 1) % table->num_of_philo]);
	message(nu + 1, "has taken a fork", table);
}

void eat(t_philo *ph, int nu)
{
	ph->last_meal = get_time();
	message(nu + 1, "is eating", ph->table);
	ft_usleep(ph->table->time_to_eat);
}

void go_to_bed(t_args *table, int nu)
{	
	pthread_mutex_unlock(&table->m_forks[nu]);
	pthread_mutex_unlock(&table->m_forks[(nu + 1) % table->num_of_philo]);
	message(nu + 1, "is sleeping", table);
	ft_usleep(table->time_to_sleep);
	message(nu + 1, "is thinking", table);
}
