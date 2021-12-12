/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eperaita <eperaita@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 13:02:17 by eperaita          #+#    #+#             */
/*   Updated: 2021/12/12 16:51:41 by eperaita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../philo.h"

static void free_mem(t_table *table, t_forks *forks)
{
	int i;

	i = -1;
   	while (++i < table->nphilo)
        pthread_mutex_destroy(&forks[i].mutex);
	//free mallocs : Array de forks, array de hilos y mutex deadlock 
	free(forks);
    free(table->thread);
    pthread_mutex_destroy(table->deadlock);
    free(table->deadlock);

}

static int philo_threads(t_table *table, t_forks **forks)
{
	t_philo philo;
	int i;
	
	////UN MUTEX PARA CREAR CADA HILO SU INSTANTANEA/////
	philo.mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!philo.mutex)
        return (1);
	pthread_mutex_init(philo.mutex, NULL);
	/////INIT THREADS/////
	i = -1;
   	while (++i < table->nphilo)
    {
		pthread_mutex_lock(philo.mutex);
        philo.table = table;
        philo.forks = *forks;
        philo.id = i + 1;
        if (pthread_create(&table->thread[i], NULL, &philos_dictator, &philo) != 0)
			return (1);
    }
	////EMPIEZA EL TIEMPO///////
	gettimeofday(&table->startime, NULL);
	//printf("startime: %ld:%d\n", table->startime.tv_sec, table->startime.tv_usec);
	////////JOIN THREADS/////////
	i = -1;
	while (++i < table->nphilo)
		pthread_join(table->thread[i], NULL);
	pthread_mutex_destroy(philo.mutex); //destroy el mutex de philo
    free(philo.mutex);
	return (0);
}

static int set_forks(t_table *table, t_forks **forks)
{
	int i;

    table->thread = (pthread_t *)malloc(table->nphilo * sizeof(pthread_t)); //hilos
    *forks = (t_forks *)malloc(table->nphilo * sizeof(t_forks)); //array de tenedores
    if(!table->thread || !forks)
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

static int set_table(t_table *table, t_forks **forks, char **argv, int argc)
{
	table->startime.tv_sec = 0;
    table->startime.tv_usec = 0;
	table->nphilo = ft_atoi(argv[1]);
	table->t_die = ft_atoi(argv[2]);
	table->t_eat = ft_atoi(argv[3]);
	table->t_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		table->ndinner = ft_atoi(argv[5]);
	else 
		table->ndinner = 0;
	table->death = 0;
	table->deadlock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t *));
	if(!table->deadlock)
		return (1);
	pthread_mutex_init(table->deadlock, NULL);
	if(set_forks(table, forks))
		return (1);	
	return (0);
}

int error(int error)
{
	if (error == 1)
		write(1, "Invalid arguments\n", 18);
	if (error == 2)
		write(1, "The table is not set\n", 21);
	if (error == 3)
		write(1, "Philosophers not created\n", 20);
	return (1);
}


int main(int argc, char **argv)
{
	t_table table;
	t_forks *forks;

	if (argc != 5 && argc != 6)
	   return (error(1));
	if(set_table(&table, &forks, argv, argc))
	   return (error(2));
	if (philo_threads(&table, &forks))
		return (error(3));
	/////////FREE MEM////////
	free_mem(&table, forks);
	//system("leaks philo");
	return (0);
}
