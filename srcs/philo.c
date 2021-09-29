#include "../incs/philo.h"

void *monitor(void *arg)
{
	t_philo	*ph = ((t_philo *)arg);	
	while (ph->table->finish)
	{
		if (!ph->iseating &&
			get_time() - ph->last_meal >= ph->table->time_to_die)
		{
			pthread_mutex_lock(&ph->m_eating);
			ph->table->finish = 0;
			pthread_mutex_lock(&ph->table->m_write);
			printf("[%llu]\tPhilo %llu died\n", get_time() - ph->table->start_time, ph->id + 1);
			printf("---------qui dovrebbe uscire-----------\n");
			pthread_mutex_unlock(&ph->table->m_write);
			// message(ph->id + 1, "died", ph->table);
			pthread_mutex_unlock(&ph->m_eating);
		}
		usleep(100);
	}
	return (NULL);
}

void *routine(void *arg)
{
	t_philo	*ph = ((t_philo *)arg);	
	// if (ph->id % 2 == 0)
	// 	ft_usleep(ph->table->time_to_eat);
	while (ph->table->finish)
	{
		take_fork(ph);
		message(ph->id + 1, "is sleeping", ph->table);
		pthread_mutex_unlock(&ph->table->m_forks[ph->id]);
		pthread_mutex_unlock(&ph->table->m_forks[(ph->id + 1) % ph->table->num_of_philo]);
		ft_usleep(ph->table->time_to_sleep);
		message(ph->id + 1, "is thinking", ph->table);
		usleep(100);
	}
	return (NULL);
}

int main(int argc, char **argv)
{
	int			i;
	t_args		table;
	t_philo		**ph;
	pthread_t	philo;

	i = 0;
	// check the number of argument
	// MISSING THE CHECKING
	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong argument number\n");
		return (1);
	}

	init(&table, argc, argv);
	// taking and converting the arguments
	ph = (t_philo **)malloc(sizeof(t_philo *) * table.num_of_philo);
	if (!ph)
		return (1);


	table.finish = 1;

	// start the cronometer
	table.start_time = get_time();

	// creation of philosophers threads
	i = 0;
	while (i < (int)table.num_of_philo)
	{
		ph[i] = (t_philo *)malloc(sizeof(t_philo) * 1);
		if (!ph[i])
			return (0);
		ph[i]->id = i;
		ph[i]->last_meal = 0;
		ph[i]->table = &table;
		ph[i]->iseating = 0;
		ph[i]->meal_counter = 0;
		ph[i]->last_meal = get_time();
		pthread_mutex_init(&ph[i]->m_eating, NULL);
		pthread_create(&ph[i]->philo, NULL, routine, (void *)ph[i]);
		i++;
		usleep(100);
	}

	i = 0;
	while (i < (int)table.num_of_philo)
	{
		pthread_create(&philo, NULL, monitor, (void *)ph[i]);
		i++;
		usleep(100);
	}
	while (table.finish)
		continue ;
	printf("---------exiting-----------\n");
	return (0);
}
