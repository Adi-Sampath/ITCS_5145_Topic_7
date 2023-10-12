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
    
    generateReduceData (arr, atoi(argv[1]));
    
    // insert reduction code here
    int sum = 0;

    #pragma omp parallel for schedule(runtime) num_threads(atoi(argv[2])) reduction(+:sum) if(atoi(argv[4])>0)
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
