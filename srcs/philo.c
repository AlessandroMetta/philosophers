#include "../incs/philo.h"

void *monitor(void *arg)
{
	t_philo	*ph = ((t_philo *)arg);	
	while (1)
	{
		pthread_mutex_lock(&ph->table->action);
		pthread_mutex_lock(&ph->table->m_write);
		if (get_time() > ph->last_meal + ph->table->time_to_die)
		{
			if (((void *)&ph->table->m_somebodydied))
				printf("---------------------\n");
			printf("[%llu]\tPhilosopher %llu %s\n", get_time() - ph->table->start_time, ph->id + 1, "died");
			pthread_mutex_unlock(&ph->table->m_somebodydied);
			break ;
		}
		pthread_mutex_unlock(&ph->table->m_write);
		pthread_mutex_unlock(&ph->table->action);
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
	while (1)
	{
		pthread_mutex_lock(&ph->table->action);
		take_fork(ph->table, ph->id);
		pthread_mutex_unlock(&ph->table->action);
		pthread_mutex_lock(&ph->table->action);
		eat(ph, ph->id);
		pthread_mutex_unlock(&ph->table->action);
		pthread_mutex_lock(&ph->table->action);
		go_to_bed(ph->table, ph->id);
		pthread_mutex_unlock(&ph->table->action);
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
	return (0);
}
