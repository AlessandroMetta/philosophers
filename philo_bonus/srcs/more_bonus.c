/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ametta <ametta@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 19:07:58 by ametta            #+#    #+#             */
/*   Updated: 2021/10/04 19:40:12 by ametta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/philo_bonus.h"

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
			printf("Error: wrong argument number\n");
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
			return (0);
		philo[i]->philo_number = i;
		philo[i]->table = table;
		philo[i]->philo_left_fork = i;
		philo[i]->philo_right_fork = (i + 1) % table->philo_ammount;
		sem_unlink("eatcounter");
		philo[i]->sem_eat = sem_open("eatcounter", O_CREAT, 0644, 1);
		if (philo[i]->sem_eat == SEM_FAILED)
			return (NULL);
		i++;
	}
	return (philo);
}

int	sems_init(t_args *table)
{
	sem_unlink("forking");
	table->sem_forks = sem_open("forking", O_CREAT, 0644,
			table->philo_ammount);
	if (table->sem_forks == SEM_FAILED)
		return (1);
	sem_unlink("writing");
	table->sem_write = sem_open("writing", O_CREAT, 0644, 1);
	if (table->sem_write == SEM_FAILED)
		return (1);
	return (0);
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
	if (sems_init(table))
		return (NULL);
	table->start_time = get_time();
	table->philo = philo_init(table);
	return (table);
}

void	take_fork(t_philo *philo)
{
	sem_wait(philo->table->sem_forks);
	message(philo->table, philo->philo_number, "has taken the left fork");
	sem_wait(philo->table->sem_forks);
	message(philo->table, philo->philo_number, "has taken the right fork");
	message(philo->table, philo->philo_number, "is eating");
	philo->meal_counter++;
	sem_wait(philo->sem_eat);
	philo->last_meal_time = get_time();
	ft_usleep(philo->table->time_to_eat);
	sem_post(philo->sem_eat);
}
