#include "../incs/philo.h"

void *amidie(void *arg)
{
	printf("Entarto qui\n");
	t_person *ph=((t_person *) arg);
	t_life *table=ph->table;
	int nu = ph->index;
	while (1)
	{
		if (getTime() > table->lastTimeEat[nu] + (uint64_t)table->no_philo[1])
		{
			message(nu + 1, "died", table);
			pthread_mutex_unlock(&table->m_somebodydied);
			table->check = 1;
			break ;
		}
		if (table->check)
			break;
	}
	return (NULL);
}

void *routine(void *arg)
{
	t_person *ph=((t_person *) arg);
	t_life *table=ph->table;
	int nu = ph->index;
	// pthread_t philo;
	table->lastTimeEat[nu] = getTime();
	// pthread_create(&philo, NULL, amidie, arg);
	// pthread_detach(philo);
	printf("entarto qui\n");
	while (!table->check)
	{
		take_fork(table, nu);
		eat(table, nu);
		go_to_bed(table, nu);
	}
	return (NULL);
}

int main(int argc, char **argv)
{
	int			i;
	t_life		table;
	t_person	*ph;

	// check the number of argument
	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong argument number\n");
		return (1);
	}

	init(&table, argc, argv);

	ph = malloc((sizeof(t_person) * table.no_philo[0]) + 1);
	if (!ph)
		return (1);

	table.check = 0;
	// creation of philosophers threads
	i = 0;
	while (i < table.no_philo[0])
	{
		ph[i].index = i + 1;
		ph[i].lastTimeEat = 0;
		ph[i].table = &table;
		pthread_create(&(ph[i].philo), NULL, routine, (void *)&(ph[i]));
		i++;
	}
	i = 0;
	/*
	while (i < table.no_philo[0])
	{
		pthread_join(ph[i].philo, NULL);
		i++;
	}*/
	// Wait until a philosopher die
	pthread_mutex_lock(&table.m_somebodydied);

	// Destroing everything
	pthread_mutex_destroy(&table.m_somebodydied);
	pthread_mutex_destroy(&table.m_write);

	return (0);
}
