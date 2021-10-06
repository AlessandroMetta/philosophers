/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ametta <ametta@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 18:58:45 by ametta            #+#    #+#             */
/*   Updated: 2021/10/06 15:30:29 by ametta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/philo.h"

int	checking_args_validity(char **argv, int argc)
{
	int	i;

	i = 1;
	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong argument number\n");
		return (1);
	}
	while (i < argc)
	{
		if (argv[i][0] == '-' || ft_str_is_number(argv[i])
			|| ft_atoi(argv[i]) == 0)
		{
			printf("Error: Invalid Arguments\n");
			return (1);
		}
		i++;
	}
	return (0);
}

t_philo	**philo_init(t_args *table)
{
	t_philo	**philo;
	int		i;

	philo = (t_philo **)malloc(sizeof(t_philo *) * table->philo_ammount);
	if (!philo)
		return (NULL);
	i = 0;
	while (i < (int)table->philo_ammount)
	{
		philo[i] = (t_philo *)malloc(sizeof(t_philo) * 1);
		if (!philo[i])
			return (NULL);
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

pthread_mutex_t	*fork_init(t_args	*table)
{
	pthread_mutex_t	*mutex_forks;
	int				i;

	i = 0;
	if (table->philo_ammount == -1 || table->time_to_die == -1
		|| table->time_to_eat == -1 || table->time_to_sleep == -1
		|| table->meal_ammount == -1 )
	{
		printf("Error: Invalid Arguments\n");
		return (NULL);
	}
	mutex_forks = malloc(sizeof(pthread_mutex_t) * table->philo_ammount);
	if (!mutex_forks)
		return (NULL);
	while (i < table->philo_ammount)
	{
		pthread_mutex_init(&mutex_forks[i], NULL);
		i++;
	}
	return (mutex_forks);
}

t_args	*init(int argc, char **argv)
{
	t_args	*table;

	table = malloc(sizeof(t_args) * 1);
	if (!table)
		return (NULL);
	table->philo_ammount = (uint64_t)ft_atoi(argv[1]);
	table->time_to_die = (uint64_t)ft_atoi(argv[2]);
	table->time_to_eat = (uint64_t)ft_atoi(argv[3]);
	table->time_to_sleep = (uint64_t)ft_atoi(argv[4]);
	table->meal_ammount = 0;
	if (argc == 6)
		table->meal_ammount = ft_atoi(argv[5]);
	pthread_mutex_init(&table->mutex_write, NULL);
	table->start_time = get_time();
	table->mutex_forks = fork_init(table);
	table->philo = philo_init(table);
	if (!table->philo || !table->mutex_forks)
		return (NULL);
	return (table);
}

void	take_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->mutex_forks[philo->philo_left_fork]);
	message(philo->table, philo->philo_number, "has taken the left fork");
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
