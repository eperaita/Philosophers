/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eperaita <eperaita@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 17:20:19 by eperaita          #+#    #+#             */
/*   Updated: 2021/12/09 12:26:06 by eperaita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../philo.h"

////////////////// TIMESTAMP //////////////////

long int o_clock(t_philo *philo)
{
    struct timeval now;
    long int ms;

    gettimeofday(&now, NULL);
    ms = (now.tv_sec * 1000 + now.tv_usec / 1000) - (philo->table->startime.tv_sec * 1000 + philo->table->startime.tv_usec / 1000);
    return (ms);
}


/////////////////////////DEADS///////////////////

static int is_dead(t_philo *philo)
{
	struct timeval now;
	long int period;

	gettimeofday(&now, NULL);
	period = (now.tv_sec * 1000 + now.tv_usec / 1000) - (philo->eatime.tv_sec * 1000 + philo->eatime.tv_usec / 1000);
	if (period > philo->table->t_die)
		return (1);
	return (0);
}

static int any_dead(t_philo *philo)
{
	if(is_dead(philo)) //estoy muerta
	{
		philo->table->death = 1;
		printf("%ld %d died\n", o_clock(philo) , philo->id);
	}
	if(philo->table->death)//hay alguien muerto!!!! KILL THE PROGRAM
		return(1);
	else
		return (0);
}

////////////////ROUTINE//////////////////

/*static void thinking(t_philo *philo)
{
	printf("%ld %d is thinking\n", o_clock(philo) , philo->id);
	usleep(1000);//cuanto tiempo piensan? el retraso justo para que no pille el tenedor
}*/


static void sleeping(t_philo *philo)
{
	struct timeval sleeptime;
	struct timeval now;

	printf("%ld %d is sleeping\n", o_clock(philo) , philo->id);
	//usleep(philo->table->t_sleep * 1000);
	gettimeofday(&sleeptime, NULL);
	gettimeofday(&now, NULL);
    while ((now.tv_sec * 1000 + now.tv_usec / 1000) - (sleeptime.tv_sec * 1000 + sleeptime.tv_usec / 1000) < philo->table->t_sleep)
    {
        usleep(philo->table->t_sleep);
        gettimeofday(&now, NULL);
    }
}

static void eating(t_philo *philo)
{
	struct timeval now;

	printf("%ld %d is eating\n", o_clock(philo) , philo->id);
	gettimeofday(&philo->eatime, NULL);
	gettimeofday(&now, NULL);
	//usleep(philo->table->t_eat * 1000);
	while ((now.tv_sec * 1000 + now.tv_usec / 1000) - (philo->eatime.tv_sec * 1000 + philo->eatime.tv_usec / 1000) < philo->table->t_eat)
	{
		usleep(philo->table->t_eat);
		gettimeofday(&now, NULL);
	}
}



static void get_fork(t_philo *philo)
{
	while (!is_dead(philo))
	{
		if (philo->id == 1)
		{
			pthread_mutex_lock(&philo->forks[philo->table->nphilo - 1].mutex);
			pthread_mutex_lock(&philo->forks[philo->id - 1].mutex);
			if(philo->forks[philo->table->nphilo - 1].fork && philo->forks[philo->id - 1].fork)
			{
				philo->forks[philo->table->nphilo - 1].fork = 0;
				printf("%ld %d has taken  a fork\n", o_clock(philo) , philo->id);
		   		philo->forks[philo->id - 1].fork = 0;
				printf("%ld %d has taken  a fork\n", o_clock(philo) , philo->id);
				pthread_mutex_unlock(&philo->forks[philo->table->nphilo - 1].mutex);
				pthread_mutex_unlock(&philo->forks[philo->id - 1].mutex);
				eating(philo);
				//y dejan de comer y sus tenedores en la mesa de nuevo
				pthread_mutex_lock(&philo->forks[philo->table->nphilo - 1].mutex);
   				pthread_mutex_lock(&philo->forks[philo->id - 1].mutex);
                philo->forks[philo->table->nphilo - 1].fork = 1;
                philo->forks[philo->id - 1].fork = 1;
                pthread_mutex_unlock(&philo->forks[philo->table->nphilo - 1].mutex);
                pthread_mutex_unlock(&philo->forks[philo->id - 1].mutex);
				break;
			}
			else 
			{
				pthread_mutex_unlock(&philo->forks[philo->table->nphilo - 1].mutex);
				pthread_mutex_unlock(&philo->forks[philo->id - 1].mutex);
			}
		}
		else
		{
			pthread_mutex_lock(&philo->forks[philo->id - 1].mutex);
			pthread_mutex_lock(&philo->forks[philo->id - 2].mutex);
			if(philo->forks[philo->id - 2].fork && philo->forks[philo->id - 1].fork)
        	{
				philo->forks[philo->id - 2].fork = 0;
				philo->forks[philo->id - 1].fork = 0;
				pthread_mutex_unlock(&philo->forks[philo->id - 1].mutex);
				pthread_mutex_unlock(&philo->forks[philo->id - 2].mutex);
				eating(philo);
				pthread_mutex_lock(&philo->forks[philo->id - 1].mutex);
            	pthread_mutex_lock(&philo->forks[philo->id - 2].mutex);
                philo->forks[philo->id - 2].fork = 1;
                philo->forks[philo->id - 1].fork = 1;
                pthread_mutex_unlock(&philo->forks[philo->id - 1].mutex);
                pthread_mutex_unlock(&philo->forks[philo->id - 2].mutex);
				break;
			}
			else
			{
				pthread_mutex_unlock(&philo->forks[philo->id - 1].mutex);
				pthread_mutex_unlock(&philo->forks[philo->id - 2].mutex);
			}
			/*if(is_dead(philo))
				break;*/
		}
	}
	/// OTRA VERSION: COLA ////
		////Tu tenedor libre?///
                //SI coge
            		//el tenedor n+1 libre?
                		//SI coge -> IS EATING
                		//NO Suelta tenedores
                //NO espera
		
}

void *philo_routine(void *philo)
{
	t_philo me_philo;

	me_philo = *(t_philo *)philo;
	pthread_mutex_unlock(me_philo.mutex);
	//printf("\nSoy la filosofa %d en %ld\n", me_philo.id, o_clock(&me_philo));
	usleep(1000);
	while(1)
	{
		if(any_dead(&me_philo))
			break;
		get_fork(&me_philo);///COMER		
		if(any_dead(&me_philo))
			break;
		sleeping(&me_philo);	//DORMIR
		//printf("%ld %d is sleeping\n", o_clock(philo) , me_philo.id);
   		//usleep(me_philo.table->t_sleep * 1000);
		////
		if(any_dead(&me_philo))
			break;
		//thinking(&me_philo);	//PENSAR
		printf("%ld %d is thinking\n", o_clock(philo) , me_philo.id);
    	usleep(500);
	}
	return (NULL);
}

