/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ametta <ametta@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 19:07:54 by ametta            #+#    #+#             */
/*   Updated: 2021/10/04 19:59:39 by ametta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/philo_bonus.h"

void	checking_meal(t_philo	*ph)
{
	uint64_t	i;

	i = 0;
	while (i < ph->table->philo_ammount)
	{
		if (ph->table->philo[i]->meal_counter < ph->table->meal_ammount)
			return ;
		if (i == ph->table->philo_ammount - 1)
		{
			sem_wait(ph->table->sem_write);
			exit(1);
		}
		sem_wait(ph->table->sem_write);
		printf("\t\t%llu\n", i);
		sem_post(ph->table->sem_write);
		i++;
	}
}

void	*monitor(void *arg)
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
		if (ph->table->meal_ammount
			&& ph->meal_counter >= ph->table->meal_ammount)
			checking_meal(ph);
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

void	start_philo(t_args		*table)
{
	uint64_t	i;

	i = 0;
	while (i < table->philo_ammount)
	{
		table->philo[i]->philo_pid = fork();
		table->philo[i]->last_meal_time = get_time();
		if (table->philo[i]->philo_pid == 0)
		{
			pthread_create(&table->philo[i]->monitor_thread, NULL, monitor,
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
