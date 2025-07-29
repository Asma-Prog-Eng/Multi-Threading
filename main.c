/*
 **************************************************************************************
 * @file           : main.c
 * @author         : Asma Askri
 * @brief          : Main program body
 ***************************************************************************************
 * A program that runs multiple threads (10) print a message
 * even message indexes are displayed at first then the odd ones
 * All of the odd threads wait until all the even threads have printed
 * Output Sample :
 * message is 0, thread id = 257055447d modified the counter to 1
 * message is 0, thread id = 257055447d read counter 2
 * Test:
 * Run the program many times, what do you notice about the global counter values
 * for the two print statements in the same thread
 ***************************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 10

int count = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER ;

pthread_mutex_t cond = PTHREAD_MUTEX_INITIALIZER ;

pthread_cond_t even_cond = PTHREAD_COND_INITIALIZER;

void *process(void *threadNum);


int number_even_finished = 0 ;

int main()

{
    pthread_t thread[NUM_THREADS];

    int value[NUM_THREADS] ;

    // create and run the 10 threads

    for (int i = 0; i < NUM_THREADS; i++) {

      value[i] = i;

      pthread_create(&thread[i], NULL, process,(void*)&value[i] );
    }

    sleep(1);

    while(1) {

         // if all even thread were executed , then signal the remaining threads
        if (number_even_finished == NUM_THREADS / 2) {

            pthread_cond_broadcast(&even_cond);
            break;

        }

     }


    // wait till the 10 threads complet running
    for (int j = 0; j < NUM_THREADS; j++) {

        pthread_join(thread[j], NULL);

    }

    return 0;
}


void *process(void *threadNum) {


    int *num = (int*) threadNum;

        pthread_mutex_lock(&cond);

       // check for odd thread

       if ((*num %2) == 0 ) {

               number_even_finished ++;

       }

       // if thread is odd , then wait for condition to be signaled

       else {

             pthread_cond_wait(&even_cond, &cond);

       }

       // if thread is even then execute the following

        pthread_mutex_unlock(&cond);
        pthread_mutex_lock(&lock);
        count ++;

        printf("message is %d, thread id = %lud modified the counter to %d\n", *num, pthread_self(), count);
        printf("message is %d, thread id = %lud read the counter %d\n", *num, pthread_self(), count);

        pthread_mutex_unlock(&lock);


    return NULL;


}



