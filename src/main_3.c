/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eperaita <eperaita@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 13:02:17 by eperaita          #+#    #+#             */
/*   Updated: 2021/12/09 12:17:14 by eperaita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../philo.h"

static int philo_threads(t_table *table, t_forks **forks)
{
	pthread_t *thread;
	t_philo philo;
	int i;
	
	philo.mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(philo.mutex, NULL);
	pthread_mutex_init(&table->deadlock, NULL);
	thread = (pthread_t *)malloc(table->nphilo * sizeof(pthread_t));
	if (!thread)
		return (1);
	i = 0;
	gettimeofday(&table->startime, NULL);
   	while (i++ < table->nphilo)
    {
		pthread_mutex_lock(philo.mutex);
        philo.table = table;
        philo.forks = *forks;
        philo.deadlock = &table->deadlock;
        philo.id = i;
        pthread_create(&thread[i], NULL, &philo_routine, &philo);
    }
	i = -1;
	while (++i < table->nphilo)
		pthread_join(thread[i], NULL);
	i = -1;
	/*while (++i < table->nphilo)
        pthread_mutex_destroy(, NULL);*///y si directamente libero el array?
	/*pthread_mutex_destroy(&table->deadlock);
	pthread_mutex_destroy(philo.mutex);*/
	free(philo.mutex);
	free(thread);
	return (0);
}

static int set_table(t_table *table, t_forks **forks, char **argv)
{
	int i;

	table->nphilo = ft_atoi(argv[1]);
	table->t_die = ft_atoi(argv[2]);
	table->t_eat = ft_atoi(argv[3]);
	table->t_sleep = ft_atoi(argv[4]);
	//printf("startime: %ld:%d\n", table->startime.tv_sec, table->startime.tv_usec);
	*forks = (t_forks *)malloc(table->nphilo * sizeof(t_forks));
	if(!forks)
		return (1);
	//////////ARRAY DOBLE (FORKS)- Incluye fork y mutex/////////////
	//pthread_mutex_init(forks->mutex, NULL);//No me deja inicializar el array entero 
	i = -1;
	while (++i < table->nphilo)
    {
        if (pthread_mutex_init(&(*forks + i)->mutex, NULL))
			return (1);
	}
	i = -1;
	while (++i < table->nphilo)
		(*forks + i)->fork = 1;
	return (0);
}

int error(int error)
{
	if (error == 1)
		write(1, "Invalid arguments\n", 18);
	if (error == 2)
		write(1, "You are not in time\n", 18);
	if (error == 3)
		write(1, "The table is not set\n", 18);
	return (1);
}


int main(int argc, char **argv)
{
	t_table table;
	t_forks *forks;

	if (argc != 5 && argc != 6)
	   return (error(1));
	if(set_table(&table, &forks, argv))
	   return (error(3));
	philo_threads(&table, &forks);
	free(forks);
	return (0);
}
