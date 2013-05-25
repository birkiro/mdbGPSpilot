#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "log_data.h"
#include "states.h"

int i = 1;
double start_time;

void logdata(void){
	while( i > 0 ){ // Only do this once
		gettimeofday(&ts, NULL);
		start_time = ts.tv_sec+(ts.tv_usec/1000000.0);
		i--;
	}
	gettimeofday(&ts, NULL);
	double dTime1 = ts.tv_sec+(ts.tv_usec/1000000.0);
	fprintf(f, "%f; %f; %f; %d; %f; %f; %f; %f; %f; %f; %d\n",
						navdata_unpacked.navdata_demo.vx,
						navdata_unpacked.navdata_demo.vy,
						navdata_unpacked.navdata_demo.vz,
						navdata_unpacked.navdata_demo.altitude,
						navdata_unpacked.navdata_demo.psi,
						navdata_unpacked.navdata_demo.phi,
						navdata_unpacked.navdata_demo.theta,
						pos_lat,
						pos_lon,
						dTime1-start_time,
						navdata_unpacked.navdata_demo.vbat_flying_percentage );
} // vx, vy, vz, alt, psi, phi, theta, lat, lon, time, batt
