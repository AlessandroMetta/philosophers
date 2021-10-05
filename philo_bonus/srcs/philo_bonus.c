/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ametta <ametta@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 19:07:54 by ametta            #+#    #+#             */
/*   Updated: 2021/10/05 12:03:59 by ametta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/philo_bonus.h"

void	*monitor_meal(void *args)
{
	t_args	*table;
	int		eat_counter;
	int		i;

	table = (t_args *)args;
	eat_counter = 0;
	while (eat_counter < table->meal_ammount)
	{
		i = 0;
		while (i <= table->philo_ammount)
		{
			sem_wait(table->sem_meal);
			i++;
		}
		eat_counter++;
	}
	sem_wait(table->sem_write);
	i = 0;
	while (i < table->philo_ammount)
		kill(table->philo[i++]->philo_pid, SIGKILL);
	return (NULL);
}

void	*monitor_die(void *arg)
{
	t_philo	*ph;

	ph = ((t_philo *)arg);
	while (1)
	{
		sem_wait(ph->sem_eat);
		if (get_time() - ph->last_meal_time >= ph->table->time_to_die)
		{
			message(ph->table, ph->philo_number, "died");
			exit(1);
		}
		sem_post(ph->sem_eat);
		usleep(100);
	}
	return (NULL);
}

void	*routine(t_philo	*ph)
{
	while (1)
	{
		take_fork(ph);
		sem_post(ph->table->sem_forks);
		sem_post(ph->table->sem_forks);
		message(ph->table, ph->philo_number, "is sleeping");
		ft_usleep(ph->table->time_to_sleep);
		message(ph->table, ph->philo_number, "is thinking");
		usleep(100);
	}
	return (NULL);
}

void	start_philo(t_args	*table)
{
	int	i;

	i = 0;
	if (table->meal_ammount)
		pthread_create(&table->monitor_meal, NULL, monitor_meal,
			(void *)table);
	while (i < table->philo_ammount)
	{
		table->philo[i]->philo_pid = fork();
		table->philo[i]->last_meal_time = get_time();
		if (table->philo[i]->philo_pid == 0)
		{
			pthread_create(&table->philo[i]->monitor_die, NULL, monitor_die,
				(void *)table->philo[i]);
			routine(table->philo[i]);
			exit(1);
		}
		i++;
		usleep(100);
	}
	close_processes(table);
}

int	main(int argc, char **argv)
{
	t_args		*table;

	if (checking_args_validity(argv, argc))
		return (1);
	table = init(argc, argv);
	if (!table)
		return (1);
	table->start_time = get_time();
	start_philo(table);
	return (0);
}
