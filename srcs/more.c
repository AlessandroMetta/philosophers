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

void take_fork(t_philo *ph)
{
	pthread_mutex_lock(&ph->table->m_die_checker);
	if (!ph->table->finish)
	{
		pthread_mutex_lock(&ph->table->m_forks[ph->id]);
		message(ph->id + 1, "has taken a fork", ph->table);
		pthread_mutex_unlock(&ph->table->m_forks[ph->id]);
	}
	pthread_mutex_unlock(&ph->table->m_die_checker);
	pthread_mutex_lock(&ph->table->m_die_checker);
	if (!ph->table->finish)
	{
		pthread_mutex_lock(&ph->table->m_forks[(ph->id + 1) % ph->table->num_of_philo]);
		message(ph->id + 1, "has taken a fork", ph->table);
		pthread_mutex_unlock(&ph->table->m_forks[(ph->id + 1) % ph->table->num_of_philo]);
	}
	pthread_mutex_unlock(&ph->table->m_die_checker);
}

void eat(t_philo *ph)
{
	pthread_mutex_lock(&ph->table->m_die_checker);
	if (!ph->table->finish)
	{
		ph->last_meal = get_time();
		message(ph->id + 1, "is eating", ph->table);
		ft_usleep(ph->table->time_to_eat);
	}
	pthread_mutex_unlock(&ph->table->m_die_checker);
}

void go_to_bed(t_philo *ph)
{
	pthread_mutex_lock(&ph->table->m_die_checker);
	if (!ph->table->finish)
	{
		message(ph->id + 1, "is sleeping", ph->table);
	}
	pthread_mutex_unlock(&ph->table->m_die_checker);
	pthread_mutex_unlock(&ph->table->m_forks[ph->id]);
	pthread_mutex_unlock(&ph->table->m_forks[(ph->id + 1) % ph->table->num_of_philo]);
	ft_usleep(ph->table->time_to_sleep);
	pthread_mutex_lock(&ph->table->m_die_checker);
	if (!ph->table->finish)
	{
		message(ph->id + 1, "is thinking", ph->table);
	}
	pthread_mutex_unlock(&ph->table->m_die_checker);
}
