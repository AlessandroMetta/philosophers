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

	pthread_mutex_lock(&ph->table->m_forks[ph->id]);
	message(ph->id + 1, "has taken the right fork", ph->table);
	pthread_mutex_lock(&ph->table->m_forks[(ph->id + 1) % ph->table->num_of_philo]);
	message(ph->id + 1, "has taken the left fork", ph->table);
	pthread_mutex_lock(&ph->m_eating);
	ph->last_meal = get_time();
	ph->iseating = 1;
	message(ph->id + 1, "is eating", ph->table);
	ft_usleep(ph->table->time_to_eat);
	ph->iseating = 0;
	ph->meal_counter++;
	pthread_mutex_unlock(&ph->m_eating);
}
