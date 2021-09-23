#include "../incs/philo.h"

void *monitor(void *arg)
{
	t_philo	*ph = ((t_philo *)arg);	
	while (!ph->table->finish)
	{
		pthread_mutex_lock(&ph->table->m_die_checker);
		if (get_time() > ph->last_meal + ph->table->time_to_die)
		{
			ph->table->finish = 1;
			message(ph->id + 1, "died", ph->table);
			pthread_mutex_unlock(&ph->table->m_somebodydied);
			pthread_mutex_unlock(&ph->table->m_die_checker);
			break ;
		}
		pthread_mutex_unlock(&ph->table->m_die_checker);
	}
	return (NULL);
}

void *routine(void *arg)
{
	t_philo	*ph = ((t_philo *)arg);	
	pthread_t philo;
	ph->last_meal = get_time();
	if (ph->id % 2 == 0)
		ft_usleep(ph->table->time_to_eat);
	pthread_create(&philo, NULL, monitor, arg);
	pthread_detach(philo);
	while (!ph->table->finish)
	{
		take_fork(ph);
		eat(ph);
		go_to_bed(ph);
	}
	return (NULL);
}

int main(int argc, char **argv)
{
	int			i;
	t_args		table;
	t_philo		*ph;	

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
	ph = malloc(sizeof(t_philo) * table.num_of_philo);
	if (!ph)
		return (1);
	
	table.finish = 0;
	// start the cronometer
	table.start_time = get_time();
	// creation of philosophers threads
	i = 0;
	while (i < (int)table.num_of_philo)
	{
		ph[i].id = i;
		ph[i].last_meal = 0;
		ph[i].table = &table;
		pthread_create(&(ph[i].philo), NULL, routine, (void *)&(ph[i]));
		i++;
	}

	// Wait until a philosopher die
	pthread_mutex_lock(&table.m_somebodydied);

	i = 0;
	while (i < (int)table.num_of_philo)
	{
		// free(&table.m_forks[i]);
		pthread_join(ph[i].philo, NULL);
		// free(&ph[i]);
		i++;
	}
	printf("all thread are joined\n");
	return (0);
}
