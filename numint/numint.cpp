#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

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
  
  if (argc < 9) {
    std::cerr<<"Usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  //insert code here
  int functionid = atoi(argv[1]);
  double a = atof(argv[2]);
  double b = atof(argv[3]);
  int n = atoi(argv[4]);
  int intensity = atoi(argv[5]);
  int nbthreads = atoi(argv[6]);
  int granularity = atoi(argv[8]);
  char* scheduling = argv[7];

  double sum = 0;
  double x = 0;
  double h = (b-a)/n;
  omp_set_num_threads(nbthreads);

      if (strcmp(scheduling, "static") == 0) {
        // Use static scheduling
        #pragma omp parallel for reduction(+:sum) schedule(static, granularity)
          for (int i = 0; i < n; i++) {
              x = a + (i + 0.5) * h;
              if (functionid == 1) {
                  sum += h * f1(x, intensity);
              } else if (functionid == 2) {
                  sum += h * f2(x, intensity);
              } else if (functionid == 3) {
                  sum += h * f3(x, intensity);
              } else if (functionid == 4) {
                  sum += h * f4(x, intensity);
              }
        }
    } else if (strcmp(scheduling, "dynamic") == 0) {
        // Use dynamic scheduling
        #pragma omp parallel for reduction(+:sum) schedule(dynamic, granularity)
          for (int i = 0; i < n; i++) {
              x = a + (i + 0.5) * h;
              if (functionid == 1) {
                  sum += h * f1(x, intensity);
              } else if (functionid == 2) {
                  sum += h * f2(x, intensity);
              } else if (functionid == 3) {
                  sum += h * f3(x, intensity);
              } else if (functionid == 4) {
                  sum += h * f4(x, intensity);
              }
          }
    } else if(strcmp(scheduling, "guided") == 0) {
      // Use guided scheduling
      #pragma omp parallel for reduction(+:sum) schedule(guided, granularity)
        for (int i = 0; i < n; i++) {
            x = a + (i + 0.5) * h;
            if (functionid == 1) {
                sum += h * f1(x, intensity);
            } else if (functionid == 2) {
                sum += h * f2(x, intensity);
            } else if (functionid == 3) {
                sum += h * f3(x, intensity);
            } else if (functionid == 4) {
                sum += h * f4(x, intensity);
            }
        }
    }

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    std::cout<<sum<<std::endl;
    std::cerr<<elapsed_seconds.count()<<std::endl;
    
  return 0;
}
