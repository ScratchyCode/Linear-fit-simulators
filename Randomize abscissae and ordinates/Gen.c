// Coded by ScratchyCode
// Program that generates linearly distributed events based on the information provided about the line.
// Compile with -lm
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define limDef 0.1

double Mbest(int N, double *x, double *y);
double Qbest(int N, double *x, double *y);
double input(double min, double max);
double randReal(double min, double max);
double randGaussInit(void);
double randGauss(double mu, double sigma);

int main(){
    long long int i=0, N;
    double M, Q, inf, sup, error, inc, xmax, x, y;
    
    srand48(time(NULL));
    
    printf("\nEnter the expected angular coeff. of the line: ");
    M = input(-1E20,1E20);
    
    printf("\nEnter the expected quota of the line: ");
    Q = input(-1E20,1E20);
    
    printf("\nEnter the experimental uncertainty of the data: ");
    error = input(0,1E20);
    
    printf("\nEnter the lower abscissa of line definition: ");
    inf = input(-1E20,1E20);
    
    printf("\nEnter the upper abscissa of line definition: ");
    sup = input(-1E20,1E20);
    if(inf >= sup){
        printf("\nINF must be less than SUP!\n");
        exit(2);
    }
    
    printf("\nEnter a maximum number of points to generate: ");
    N = input(1,1E20);
    
    // maximum density of the points around the line
    xmax = (int)((int)(sup-inf)/N);
    
    // creating data arrays
    double *X = calloc(N*2,sizeof(double));
	double *Y = calloc(N*2,sizeof(double));
	if(X == NULL || Y == NULL){
	    perror("\nerror");
	    printf("\n");
	    exit(1);
    }
    
    x = inf;
    for(i=0; i<N; i++){
        y = M * x + Q;
        y += randReal(-error,error);
        X[i] = x;
        Y[i] = y;
        inc = randReal(0,xmax);
        if(inc < limDef){
            inc = limDef;
        }
        x += inc;
    }
    
    long long int dim = i;
    
	// estimation of linear coeff.
	M = Mbest(dim,X,Y);
	Q = Qbest(dim,X,Y);
	
	printf("\nThe best line that interpolates data is: Y = %.3lf X + %.3lf\n",M,Q);
	
	FILE *data = fopen("data.dat","w");
	if(data == NULL){
	    perror("\nError");
	    exit(1);
	}
	
	// writing experimental data and fit on the file with the associated error
	for(i=0; i<dim; i++){
	    fprintf(data,"%lf %lf %lf\n",X[i],Y[i],error);
	}
	
	fflush(data);
	fclose(data);
	
	free(X);
	free(Y);
    
    // return to script
	return 0;
}

double Mbest(int N, double *x, double *y){
    int i;
    double Msum1=0, Msum2=0, Msum3=0, Msum4=0;
    
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

double randReal(double min, double max){
    return (double)(lrand48()%(int)(max-min+1)+min);
}
