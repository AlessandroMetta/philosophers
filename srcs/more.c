#include "../incs/philo.h"

int	checking_args_validity(char **argv, int argc)
{
	int i;

	i = 0;
	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong argument number\n");
		return (1);
	}
	while (i < argc)
	{
		if (argv[i][0] == '-' && ft_str_is_number(argv[i]))
		{
			printf("Error: wrong argument number\n");
			return (1);
		}
		i++;
	}
	return (0);
}

philo_t	**philo_init(args_t *table)
{
	philo_t	**philo;
	int		i;

	philo = (philo_t **)malloc(sizeof(philo_t *) * table->philo_ammount);
	if (!philo)
		return (NULL);
	i = 0;
	while (i < (int)table->philo_ammount)
	{
		philo[i] = (philo_t *)malloc(sizeof(philo_t) * 1);
		if (!philo[i])
			return (0);
		if (pthread_mutex_init(&philo[i]->mutex_eating, NULL))
			return (NULL);
		philo[i]->philo_number = i;
		philo[i]->table = table;
		philo[i]->is_eating = 0;
		philo[i]->meal_counter = 0;
		philo[i]->philo_left_fork = i;
		philo[i]->philo_right_fork = (i + 1) % table->philo_ammount;
		i++;
	}
	return (philo);
}

args_t	*init(int argc, char **argv)
{
	uint64_t	i;
	args_t	*table;

	i = 0;
	table = malloc(sizeof(args_t) * 1);
	if (!table)
		return (NULL);
	table->philo_ammount = (uint64_t)ft_atoi(argv[1]);
	table->time_to_die = (uint64_t)ft_atoi(argv[2]);
	table->time_to_eat = (uint64_t)ft_atoi(argv[3]);
	table->time_to_sleep = (uint64_t)ft_atoi(argv[4]);
	if (argc == 6)
		table->meal_ammount = ft_atoi(argv[5]);
	else
		table->meal_ammount = 0;
	pthread_mutex_init(&table->mutex_write, NULL);

	table->start_time = get_time();
	table->mutex_forks = malloc(sizeof(pthread_mutex_t) * table->philo_ammount);
	if (!table->mutex_forks)
		return (NULL);
	while (i < table->philo_ammount)
	{
		pthread_mutex_init(&table->mutex_forks[i], NULL);
		i++;
	}

	table->philo = philo_init(table);
	return(table);
}

void take_fork(philo_t *philo)
{
	// pthread_mutex_lock(&philo->table->mutex_write);
	// printf("[%llu]\tPhilosopher %llu %s %llu\n", get_time() - philo->table->start_time,
	// 		philo->philo_number + 1, "is tring to take the fork no:", philo->philo_left_fork);
	// pthread_mutex_unlock(&philo->table->mutex_write);


	pthread_mutex_lock(&philo->table->mutex_forks[philo->philo_left_fork]);
	message(philo->table, philo->philo_number, "has taken the left fork");

	// pthread_mutex_lock(&philo->table->mutex_write);
	// printf("[%llu]\tPhilosopher %llu %s %llu\n", get_time() - philo->table->start_time,
	// 		philo->philo_number + 1, "is tring to take the fork no:", philo->philo_right_fork);
	// pthread_mutex_unlock(&philo->table->mutex_write);


	pthread_mutex_lock(&philo->table->mutex_forks[philo->philo_right_fork]);
	message(philo->table, philo->philo_number, "has taken the right fork");
	pthread_mutex_lock(&philo->mutex_eating);
	philo->last_meal_time = get_time();
	philo->is_eating = 1;
	message(philo->table, philo->philo_number, "is eating");
	ft_usleep(philo->table->time_to_eat);
	philo->is_eating = 0;
	philo->meal_counter++;
	pthread_mutex_unlock(&philo->mutex_eating);
}
