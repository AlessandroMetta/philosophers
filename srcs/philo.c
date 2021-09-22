#include "../incs/philo.h"

void *amidie(void *arg)
{
	t_person	*ph = ((t_person *)arg);	
	while (1)
	{
		if (getTime() > ph->lastMeal + (uint64_t)ph->table->no_philo[1])
		{
			message(ph->index + 1, "died", ph->table);
			pthread_mutex_lock(&ph->table->m_write);
			pthread_mutex_unlock(&ph->table->m_somebodydied);
			break ;
		}
	}
	return (NULL);
}

void *routine(void *arg)
{
	t_person	*ph = ((t_person *)arg);	
	pthread_t philo;
	ph->lastMeal = getTime();
	pthread_create(&philo, NULL, amidie, arg);
	while (1)
	{
		take_fork(ph->table, ph->index);
		eat(ph, ph->index);
		go_to_bed(ph->table, ph->index);
	}
}

int main(int argc, char **argv)
{
	pthread_t	philo;
	int			i;
	t_life		table;
	t_person	*ph;	

	i = 0;

	// check the number of argument
	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong argument number\n");
		return (1);
	}

	init(&table, argc, argv);
	// taking and converting the arguments
	
	ph = malloc(sizeof(t_person) * table.no_philo[0]);
	if (!ph)
		return (1);
	
	// start the cronometer
	table.startTime = getTime();

	// creation of philosophers threads
	i = 0;
	while (i < table.no_philo[0])
	{
		ph[i].index = i;
		ph[i].lastMeal = 0;
		ph[i].table = &table;
		pthread_create(&philo, NULL, routine, (void *)&(ph[i]));
		i++;
	}
	// Wait until a philosopher die
	pthread_mutex_lock(&table.m_somebodydied);
	return (0);
}
