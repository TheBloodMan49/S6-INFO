#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void* do_loop(void* data){
    int i;
    int j;
    int me = *((int *) data);
    int nb_iter;
    if (me == 0) nb_iter = 10;
    else nb_iter = 100;
    for (i=0;i<nb_iter;i++){
        printf("Thread id %d - etape %d\n", me, i);
        //for (j=0;j<1000;j++); // pour simuler du temps de calcul…
        sleep(1); // [**] ligne a de-commenter dans la v2
    }
    pthread_exit(NULL);
}
int main(int argc, char* argv[]){
    int t_id;
    pthread_t p_thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int a =0;
    int b =1;
    t_id = pthread_create(&p_thread, &attr, do_loop, (void*) &a);
    a= 1; // ligne a de-commenter dans la v3
    //do_loop((void*) &b); // ligne a commenter dans la v3
    do_loop((void*) &a); // ligne a de-commenter dans la v3
    // sleep(1);//[*] commenter dans la V1
    pthread_join(p_thread, NULL);
    return 0;
}