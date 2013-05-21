#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "log_data.h"
#include "states.h"

int i = 1;
double start_time;

void logdata(void){
	while(i>0){
		gettimeofday(&ts, NULL);
		start_time = ts.tv_sec+(ts.tv_usec/1000000.0);
		i--;
	}
	gettimeofday(&ts, NULL);
	double dTime1 = ts.tv_sec+(ts.tv_usec/1000000.0);
	fprintf(f, "VX:%f; VY:%f; VZ:%f; ALT:%d; PSI:%f; PHI:%f; THETA:%f; LON:%f; LAT:%f; TIME:%f\n",
																		navdata_unpacked.navdata_demo.vx,
																		navdata_unpacked.navdata_demo.vy,
																		navdata_unpacked.navdata_demo.vz,
																		navdata_unpacked.navdata_demo.altitude,
																		navdata_unpacked.navdata_demo.psi,
																		navdata_unpacked.navdata_demo.phi,
																		navdata_unpacked.navdata_demo.theta,
																		pos_lon,
																		pos_lat,
																		dTime1-start_time);

}
