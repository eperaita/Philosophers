/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eperaita <eperaita@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 20:28:38 by eperaita          #+#    #+#             */
/*   Updated: 2021/12/12 20:08:04 by eperaita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../philo.h"

/////////////////////////DEADS///////////////////

int	is_dead(t_philo *philo)
{
	struct timeval	now;
	long int		period;

	gettimeofday(&now, NULL);
	period = (now.tv_sec * 1000 + now.tv_usec / 1000)
		- (philo->eatime.tv_sec * 1000 + philo->eatime.tv_usec / 1000);
	if (period > philo->table->t_die)
		return (1);
	return (0);
}

int	any_dead(t_philo *philo)
{
	pthread_mutex_lock(philo->table->deadlock);
	if (philo->table->death)
	{
		pthread_mutex_unlock(philo->table->deadlock);
		return (1);
	}
	if (is_dead(philo))
	{
		philo->table->death = 1;
		pthread_mutex_unlock(philo->table->deadlock);
		printf("%ld %d died\n", o_clock(philo), philo->id);
		return (1);
	}
	pthread_mutex_unlock(philo->table->deadlock);
	return (0);
}

////////////////// TIMESTAMP //////////////////

long int	o_clock(t_philo *philo)
{
	struct timeval	now;
	long int		ms;

	gettimeofday(&now, NULL);
	ms = (now.tv_sec * 1000 + now.tv_usec / 1000)
		- (philo->table->startime.tv_sec * 1000
			+ philo->table->startime.tv_usec / 1000);
	return (ms);
}

//////////////////MY USLIP //////////////////////////

int	my_uslip(t_philo *philo, int time)
{
	struct timeval	now;
	struct timeval	end;
	int				i;

	gettimeofday(&now, NULL);
	gettimeofday(&end, NULL);
	i = 0;
	while ((end.tv_sec * 1000 + end.tv_usec / 1000)
		- (now.tv_sec * 1000 + now.tv_usec / 1000) < time)
	{
		usleep(100);
		gettimeofday(&end, NULL);
		if ((end.tv_sec * 1000 + end.tv_usec / 1000)
			- (now.tv_sec * 1000 + now.tv_usec / 1000) - i > 10)
		{
			i += 10;
			if (any_dead(philo))
				return (1);
		}
	}
	if (any_dead(philo))
		return (1);
	return (0);
}
