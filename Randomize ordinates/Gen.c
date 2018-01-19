// Coded by ScratchyCode
// Program that generates events distributed linearly based on the information provided on the line
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

double Mbest(int N, double *x, double *y);
double Qbest(int N, double *x, double *y);
double input(double min, double max);
double realRand(double min, double max);
double randGaussInit(void);
double randGauss(double mu, double sigma);

int main(){
    long long int i=0, N;
    double M, Q, inf, sup, error, inc, x, y;
    
    srand48(time(NULL));
    
    printf("\nEnter the angular coeff. of the expected line: ");
    M = input(-1E20,1E20);
    
    printf("\nEnter the y quote of the expected line: ");
    Q = input(-1E20,1E20);
    
    printf("\nEnter the data's uncertainty: ");
    error = input(0,1E20);
    
    printf("\nEnter the INF to be plotted: ");
    inf = input(-1E20,1E20);
    
    printf("\nEnter the SUP to be plotted: ");
    sup = input(-1E20,1E20);
    if(inf >= sup){
        printf("\nINF must be less than SUP!\n");
        exit(2);
    }
    
    printf("\nEnter the abscissa increment: ");
    inc = input(0,1E20);
    if(inc >= (int)sup){
        printf("\nThe increase must be less than SUP = %d.\n",(int)sup);
        exit(2);
    }
    
    // density of the points around the line
    N = (int)((int)(sup-inf)/inc);
    
    // arrays that will contain the date
    double *X = calloc(N*2,sizeof(double));
	double *Y = calloc(N*2,sizeof(double));
	if(X == NULL || Y == NULL){
	    perror("\nError");
	    printf("\n");
	    exit(1);
    }
    
    // generating data
    x = inf;
    do{
        y = M * x + Q;
        y += realRand(-error,error);
        X[i] = x;
        Y[i] = y;
        i++;
        x += inc;
    }while((int)x <= (int)sup);
    
    long long int dim = i;

	// data interpolation
	M = Mbest(dim,X,Y);
	Q = Qbest(dim,X,Y);
	
	printf("\nThe line that best interpolates the generated data is: Y = %.3lf X + %.3lf\n",M,Q);
	
	FILE *data = fopen("data.dat","w");
	if(data == NULL){
	    perror("\nError");
	    exit(1);
	}
	
	// writing data with associated error for fit
	for(i=0; i<dim; i++){
	    fprintf(data,"%lf %lf %lf\n",X[i],Y[i],error);
	}
	
	fflush(data);
	fclose(data);
	
	free(X);
	free(Y);
    
    // go to fit data
	return 0;
}

double Mbest(int N, double *x, double *y){
    int i;
    double Msum1=0, Msum2=0, Msum3=0, Msum4=0;
    
    // best angular coeff. M
	for(i=0; i<N; i++){
	    Msum1 = Msum1 + x[i] * y[i];
	    Msum2 = Msum2 + x[i];
	    Msum3 = Msum3 + y[i];
	    Msum4 = Msum4 + pow(x[i],2);
	}
	
	return ((N*Msum1 - Msum2*Msum3) / (N*Msum4 - pow(Msum2,2)));
    
}

double Qbest(int N, double *x, double *y){
    int i;
    double Qsum1=0, Qsum2=0, Qsum3=0, Qsum4=0;
    
    // best quote Q
	for(i=0; i<N; i++){
	    Qsum1 = Qsum1 + y[i];
	    Qsum2 = Qsum2 + pow(x[i],2);
	    Qsum3 = Qsum3 + x[i];
	    Qsum4 = Qsum4 + x[i] * y[i];
	}
	
	return ((Qsum1*Qsum2 - Qsum3*Qsum4) / (N*Qsum2 - pow(Qsum3,2)));
}

double input(double min, double max){
    double x;
    
    do{
        scanf("%lf",&x);
        if(!(min<x && x<max)){
            printf("\nThe number must be between %.1lf and %.1lf.\n",min,max);
            printf("Try again: ");
        }
    }while(!(min<x && x<max));
    
    return x;
}

double realRand(double min, double max){
    double range = (max - min); 
    double dinom = RAND_MAX / range;
    return min + (lrand48() / dinom);
}
