/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ametta <ametta@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 18:58:40 by ametta            #+#    #+#             */
/*   Updated: 2021/10/05 11:52:32 by ametta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/philo.h"

void	checking_meal(t_philo	*ph)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&ph->mutex_eating);
	while (i < ph->table->philo_ammount)
	{
		if (ph->table->philo[i]->meal_counter >= ph->table->meal_ammount)
		{
			if (i == ph->table->philo_ammount - 1)
			{
				pthread_mutex_lock(&ph->table->mutex_write);
				ph->table->finish = 0;
			}
			i++;
		}
		else
			break ;
	}
	pthread_mutex_unlock(&ph->mutex_eating);
}

void	*monitor(void *arg)
{
	t_philo	*ph;

	ph = ((t_philo *)arg);
	while (ph->table->finish)
	{
		if (!ph->is_eating
			&& get_time() - ph->last_meal_time >= ph->table->time_to_die)
		{
			pthread_mutex_lock(&ph->mutex_eating);
			message(ph->table, ph->philo_number, "died");
			ph->table->finish = 0;
			pthread_mutex_unlock(&ph->mutex_eating);
		}
		if (ph->table->meal_ammount
			&& ph->meal_counter >= ph->table->meal_ammount)
			checking_meal(ph);
		usleep(100);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*ph;

	ph = ((t_philo *)arg);
	while (ph->table->finish)
	{
		take_fork(ph);
		pthread_mutex_unlock(&ph->table->mutex_forks[ph->philo_left_fork]);
		pthread_mutex_unlock(&ph->table->mutex_forks[ph->philo_right_fork]);
		message(ph->table, ph->philo_number, "is sleeping");
		ft_usleep(ph->table->time_to_sleep);
		message(ph->table, ph->philo_number, "is thinking");
	}
	return (NULL);
}

void	start_philo(t_args *table)
{
	int	i;

	i = 0;
	while (i < table->philo_ammount)
	{
		table->philo[i]->last_meal_time = get_time();
		pthread_create(&table->philo[i]->philo_thread, NULL, routine,
			(void *)table->philo[i]);
		i++;
		usleep(100);
	}
	i = 0;
	while (i < table->philo_ammount)
	{
		pthread_create(&table->philo[i]->monitor_thread, NULL, monitor,
			(void *)table->philo[i]);
		i++;
		usleep(100);
	}
}

int	main(int argc, char **argv)
{
	t_args		*table;

	if (checking_args_validity(argv, argc))
		return (1);
	table = init(argc, argv);
	if (!table)
		return (1);
	table->finish = 1;
	table->start_time = get_time();
	start_philo(table);
	while (table->finish)
		continue ;
	return (0);
}
