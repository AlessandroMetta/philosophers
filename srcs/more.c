#include "../incs/philo.h"

int init(args_t *table, int argc, char **argv)
{
	int i;

	i = 0;
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
		return (1);
	while (i < (int)table->philo_ammount)
	{
		pthread_mutex_init(&table->mutex_forks[i], NULL);
		i++;
	}
	return(0);
}

void take_fork(philo_t *philo)
{

	pthread_mutex_lock(&philo->table->mutex_forks[philo->philo_left_fork]);
	message(philo->table, philo->philo_number, "has taken the right fork");
	pthread_mutex_lock(&philo->table->mutex_forks[philo->philo_right_fork]);
	message(philo->table, philo->philo_number, "has taken the left fork");
	pthread_mutex_lock(&philo->mutex_eating);
	philo->last_meal_time = get_time();
	philo->is_eating = 1;
	message(philo->table, philo->philo_number, "is eating");
	ft_usleep(philo->table->time_to_eat);
	philo->is_eating = 0;
	philo->meal_counter++;
	pthread_mutex_unlock(&philo->mutex_eating);
}
