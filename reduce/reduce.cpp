#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>


#ifdef __cplusplus
extern "C" {
#endif
    void generateReduceData (int* arr, size_t n);
#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {
    //forces openmp to create the threads beforehand
#pragma omp parallel
    {
        int fd = open (argv[0], O_RDONLY);
        if (fd != -1) {
            close (fd);
        }
        else {
            std::cerr<<"something is amiss"<<std::endl;
        }
    }
    
    if (argc < 5) {
        std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <granularity>"<<std::endl;
        return -1;
    }

    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    
    int n = atoi(argv[1]);
    int * arr = new int [n];
    int nbthreads = atoi(argv[2]);
    int granularity = atoi(argv[4]);
    char* scheduling = argv[3];

    omp_set_num_threads(nbthreads);
    omp_sched_t sched;
    
    generateReduceData (arr, atoi(argv[1]));
    
    // insert reduction code here
    int sum = 0;

    if(strcmp(scheduling, "static") == 0)
        sched = omp_sched_static;
    else if(strcmp(scheduling, "dynamic") == 0)
        sched = omp_sched_dynamic;
    else if(strcmp(scheduling, "guided") == 0)
        sched = omp_sched_guided;
    else
        sched = omp_sched_static;
    
    omp_set_schedule(sched, granularity);
    
    #pragma omp parallel for reduction(+:sum)  
        for (int i = 0; i < n; i++) {
            sum += arr[i];
        }

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapased_seconds = end-start;

    std::cout<<sum<<std::endl;
    std::cerr<<elapased_seconds.count()<<std::endl;
    
    delete[] arr;
    
    return 0;
}
