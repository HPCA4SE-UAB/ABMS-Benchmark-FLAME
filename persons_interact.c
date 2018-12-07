/* 
 * Benchmark model for FLAME ABMS
 * This file is part of the ABMS-Benchmark-FLAME distribution (https://github.com/HPCA4SE-UAB/ABMS-Benchmark-FLAME.git).
 * Copyright (c) 2018 Universitat Autònoma de Barcelona, Escola Universitària Salesiana de Sarrià
 *
 *Based on: Alban Rousset, Bénédicte Herrmann, Christophe Lang, Laurent Philippe
 *A survey on parallel and distributed multi-agent systems for high performance comput-
 *ing simulations Computer Science Review 22 (2016) 27–46 
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License 
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#include "header.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <uuid/uuid.h>
#include "person_agent_header.h"
#include <fftw3.h>
#include "fnv.h"

/*
 * Parameter: size of communitacion message
*/
#define COM_BUFFER_SIZE 256
//#define COM_BUFFER_SIZE 64
//#define COM_BUFFER_SIZE 32
//#define COM_BUFFER_SIZE 16

/*
 * Parameter: size of FFT vector
*/
#define FFT_VECTOR_SIZE 16384
//#define FFT_VECTOR_SIZE 512
//#define FFT_VECTOR_SIZE 16

/*
 * Parameter: Max number of agents to play with
*/ 
#define MAX_AGENTS_TO_PLAY 10


/*
 * Function:  frand 
 * --------------------
 * get random number scaled between 0 and RAND_MAX
 * 
 * -: No parameters
 *
 * returns: random number
 */
double frand() {
	double value;
	value = ((double)rand()/(RAND_MAX));
	return value;
}

/*
 * Function:  isIntoCircle 
 * --------------------
 * check if distance between two points is less than or equal
 * 
 * x,y: position A coordinates
 * xCircle,yCircle: posicion B coordinates
 * rCircle: distance to check between position A to B
 *
 * returns: 0 distance between A to B is less than or equal rCircle
 *          1 distance between A to B is more than rCicle
 */
int isIntoCircle(int x, int y, int xCircle, int yCircle, int rCircle) {
	int dist = sqrt(pow(x-xCircle, 2)+pow(y-yCircle, 2));

	if(dist <= rCircle) {
		return 0;
	} else {
		return 1;
	}
}

/*
 * Function:  isIntoSquare 
 * --------------------
 * check if two point are into a squeare
 * 
 * x,y: position A coordinates
 * xCircle,yCircle: posicion B coordinates
 * rAquare: distance to check between position A to B
 *
 * returns: 0 A and B are into rSquare
 *          1 A to B are not into rSquare
 */
int isIntoSquare(int x, int y, int xCircle, int yCircle, int rSquare) {
	int dist_x = abs(x-xCircle);
	int dist_y = abs(y-yCircle);

	if((dist_x <= rSquare)&&(dist_y <= rSquare)) {
		return 0;
	} else {
		return 1;
	}
}

/*
 * Function:  walk 
 * --------------------
 * move agent
 * 
 * -: No parameters
 *
 * returns: 0
 */
int walk() {
	int newx; // new posX
	int newy; // new posY
	int xdir=0; // new dirX
	int ydir=0; // new dirY


	xdir = (rand()/(float)RAND_MAX) < 0.5 ? -1 : 1;
	ydir = (rand()/(float)RAND_MAX) < 0.5 ? -1 : 1;

	newx = X + xdir;
	newy = Y + ydir;
          
	if (newx < 0) { 
       		newx = (WIDTH-1); 
	} else if (newx >= (HEIGHT-1)) {
		newx = 0; 
	}
        
	if (newy < 0) { 
		newy = (HEIGHT-1);  	
	} else if (newy >= (HEIGHT-1)) {
       		newy = 0;  
	}
			
	X = newx;
	Y = newy;
	
	add_agentLocation_message(ID, X, Y);

	return 0;
}

/*
 * Function:  walk 
 * --------------------
 * move agent
 * 
 * -: No parameters
 *
 * returns: 0
 */
int cooperate() {
	int cooperate_;
	char message[COM_BUFFER_SIZE] = "0123456789";

	agentLocation_message = get_first_agentLocation_message();
	
	while(agentLocation_message) {
		if( ID != agentLocation_message->my_id) {
			//if(isIntoCircle(X, Y, agentLocation_message->x, agentLocation_message->y, RADIUS) == 0) {
			if(isIntoSquare(X, Y, agentLocation_message->x, agentLocation_message->y, RADIUS) == 0) {
				if (frand() < (C*1.0)/TOTAL){
					cooperate_ = 1;
				}
				else
					cooperate_ = 0;
				add_agentCooperate_message(ID, agentLocation_message->my_id, cooperate_, message);			
			} 
		}
		agentLocation_message = get_next_agentLocation_message(agentLocation_message);
	}
	return 0;
}

/*
 * Function:  play 
 * --------------------
 * play prisoner’s dilemma with all agents located until RADIUS distance
 * 
 * -: No parameters
 *
 * returns: 0
 */
int play() {
	int iCooperated=0, cPayoff=0, totalPayoff=0, i=0;

	agentCooperate_message = get_first_agentCooperate_message();
	
	while(agentCooperate_message) {
	
			// Do I cooperate?     
       			if (frand() < (C*1.0)/TOTAL){
				iCooperated = 1;			
			}
			else
				iCooperated = 0;
               
        		double payoff = (iCooperated ?
						 (agentCooperate_message->cooperate ?  7 : 1) :     // If I cooperated, did my opponent?
						 (agentCooperate_message->cooperate ? 10 : 3));     // If I didn't cooperate, did my opponent?
        		if(iCooperated) cPayoff += payoff;
        		totalPayoff             += payoff;

		agentCooperate_message = get_next_agentCooperate_message(agentCooperate_message);
		i++;
                if (i >= MAX_AGENTS_TO_PLAY) break;     //Control max number agents to play with

	}
	
	C      += cPayoff;
	TOTAL  += totalPayoff;
	
	return 0;
}

/*
 * Function:  compute 
 * --------------------
 * compute a FFT of a FFT_VECTOR_SIZE vector size
 * 
 * -: No parameters
 *
 * returns: 0
 */
int compute() {
	
	int i;
	int N = FFT_VECTOR_SIZE;	 
	fftw_complex *in, *out;
	fftw_plan p;
   
	srand(123456); //To get the same computing time
	in = fftw_malloc(sizeof(fftw_complex) * N);
	out = fftw_malloc (sizeof(fftw_complex) * N);
    
	for (i = 0; i < N; i++ ) {
		in[i][0] = frand();
		in[i][1] = frand();
	}
	
	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p); 
	fftw_destroy_plan(p);
	fftw_free(in); fftw_free(out);

	srand(time(NULL)); //To have aleatorial numbers in others frand()
	
	return 0;
}

/*
 * Function:  birthdeath
 * --------------------
 * compute birth and death algorithms
 * 
 * -: No parameters
 *
 * returns: 0 keep alive
 *          1 death
 */
int birthdeath(){

	double p;
	double birth_rate_factor = BIRTH_RATE * (1 - fmin(1 , sqrt( pow(abs(X-CENTER_BIRTH_X),2) + pow(abs(Y-CENTER_BIRTH_Y),2) )/((HEIGHT+WIDTH)/2)));
	double death_rate_factor = DEATH_RATE * (1 - fmin(1 , sqrt( pow(abs(X-CENTER_DEATH_X),2) + pow(abs(Y-CENTER_DEATH_Y),2) )/((HEIGHT+WIDTH)/2)));
	
	uuid_t uuid;
	Fnv32_t hash_val;

	//Birth algorithm
	p = frand();
	if (p<=birth_rate_factor){
		// generate
		uuid_generate(uuid);
		hash_val = fnv_32_buf(uuid, 16, ID);
		add_person_agent(hash_val, 100, 200, X, Y, 0); //Add new person agent in the same position with my_id, new_id, c, total, x, y
	}
	
	//Death algorithm
	p = frand();
	if (p<=death_rate_factor){
		return 1; //Death
	}else
		return 0; //Keep alive		
}

