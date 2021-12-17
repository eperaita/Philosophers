/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eperaita <eperaita@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 13:02:17 by eperaita          #+#    #+#             */
/*   Updated: 2021/12/17 19:53:43 by eperaita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../philo.h"

static int	philo_threads(t_table *table, t_forks **forks)
{
	t_philo	philo;
	int		i;

	philo.mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!philo.mutex)
		return (1);
	pthread_mutex_init(philo.mutex, NULL);
	i = -1;
	while (++i < table->nphilo)
	{
		pthread_mutex_lock(philo.mutex);
		philo.table = table;
		philo.forks = *forks;
		philo.id = i + 1;
		if (pthread_create(&table->thread[i]
				, NULL, &philos_dictator, &philo) != 0)
			return (1);
	}
	gettimeofday(&table->startime, NULL);
	i = -1;
	while (++i < table->nphilo)
		pthread_join(table->thread[i], NULL);
	pthread_mutex_destroy(philo.mutex);
	free(philo.mutex);
	return (0);
}

static int	set_forks(t_table *table, t_forks **forks)
{
	int	i;

	table->thread = (pthread_t *)malloc(table->nphilo * sizeof(pthread_t));
	*forks = (t_forks *)malloc(table->nphilo * sizeof(t_forks));
	if (!table->thread || !forks)
		return (1);
	i = -1;
	while (++i < table->nphilo)
	{
		(*forks + i)->fork = 1;
		if (pthread_mutex_init(&(*forks + i)->mutex, NULL))
			return (1);
	}
	return (0);
}

static int	set_table(t_table *table, t_forks **forks, char **argv, int argc)
{
	table->startime.tv_sec = 0;
	table->startime.tv_usec = 0;
	table->nphilo = ft_atoi(argv[1]);
	table->t_die = ft_atoi(argv[2]);
	table->t_eat = ft_atoi(argv[3]);
	table->t_sleep = ft_atoi(argv[4]);
	if (table->nphilo < 0 || table->t_die <= 0
		|| table->t_eat <= 0 || table->t_sleep <= 0)
		return (1);
	if (argc == 6)
	{
		table->ndinner = ft_atoi(argv[5]);
		if (table->ndinner == 0)
			return (1);
	}
	else
		table->ndinner = 0;
	table->death = 0;
	table->deadlock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!table->deadlock)
		return (1);
	pthread_mutex_init(table->deadlock, NULL);
	if (set_forks(table, forks))
		return (1);
	return (0);
}

int	error(int error)
{
	if (error == 1)
		write(1, "Invalid arguments\n", 18);
	if (error == 2)
		write(1, "The table is not set\n", 21);
	if (error == 3)
		write(1, "Philosophers not created\n", 20);
	return (1);
}

int	main(int argc, char **argv)
{
	t_table	table;
	t_forks	*forks;

	if (argc != 5 && argc != 6)
		return (error(1));
	if (set_table(&table, &forks, argv, argc))
		return (error(2));
	if (table.nphilo == 0)
		return (0);
	if (philo_threads(&table, &forks))
		return (error(3));
	free_mem(&table, forks);
	return (0);
}
