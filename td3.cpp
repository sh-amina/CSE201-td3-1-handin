#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
    double* a = new double[new_size];
    for (int i=0;i<new_size;i++){
        if (i<length) {
            a[i] = array [i];
        }
        else {
        a[i] = 0;
        }
    }
    delete[] array;
  return a;
}

double* shrink_array(double* array, int length, int new_size) {
  double* a = new double[new_size];
  for (int i=0;i<new_size;i++){
      a[i] = array [i];
  }
  delete[] array;
  return a;
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
    if (current_size == max_size){
        max_size+=5;
        array = extend_array(array, current_size, max_size);
         }
    double* a = new double[current_size+1];
    for (int i=0;i<current_size+1;i++){
      if (i==current_size){
          a[i] = element;
      }
      else{
      a[i] = array[i];
      }
    }
    current_size+= 1;
    delete[] array;
    return a;
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
    if (current_size>0){
        current_size-=1;
    }
    if (abs(current_size-max_size)>= 5){
        array = shrink_array(array,max_size,max_size-5);
        max_size-=5;
    }
  return array;
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;

  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
      telemetry =append_to_array(t,telemetry,telemetry_current_size,telemetry_max_size);
      telemetry =append_to_array(x,telemetry,telemetry_current_size,telemetry_max_size);
      telemetry =append_to_array(y,telemetry,telemetry_current_size,telemetry_max_size);
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;
    }
  }

  return hit_target;
}

void sort(double *telemetries, const int telemetry_current_size) {
  for (int i=0;i<telemetry_current_size;i += 3) {
      int min=i;
      for (int j =i+3;j<telemetry_current_size;j += 3) {
        if (telemetries[j] < telemetries[min]) min = j;}
      if (min != i) {
       double l1 = telemetries[i];
       double l1_2 = telemetries[i+1];
       double l1_3 = telemetries[i+2];
       telemetries[i] = telemetries[min];
       telemetries[i+1] = telemetries[min+1];
       telemetries[i+2] = telemetries[min+2];
       telemetries[min] = l1;
       telemetries[min+1] = l1_2;
       telemetries[min+2] = l1_3;
      }
  }
}

void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &telemetry,
                     int &telemetry_current_size,
                     int &telemetry_max_size) {
  for (int i = 0; i < tot_telemetries ; i++){
      double* temp_telemtry = telemetries[i];
      int temp_tot = telemetries_sizes[i];
      for (int j=0; j<temp_tot;j++){
          telemetry = append_to_array(temp_telemtry[j],
                                      telemetry,telemetry_current_size
                                      ,telemetry_max_size);
      }
  }
   sort(telemetry,telemetry_current_size);
}
