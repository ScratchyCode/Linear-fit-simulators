// Coded by ScratchyCode
// Find the best line that interpolates entered data.
// Compile with -lm
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define INC 0.0001

int linesFile(char file[]);
double Mbest(int N, double *x, double *y);
double Qbest(int N, double *x, double *y);
void fit(double M, double Q, double array[], int dim);
double min(double array[], int dim);
double max(double array[], int dim);
int control(void);

int main(){
    int i, N;
    double M, Q, error;
    char c, file[] = "data.dat";
    
    // control that gnuplot is present on the system
    int gnupl = control();
    if(gnupl == 1){
        printf("\nYou need gnuplot to graph the results.");
        printf("\nInstall it with: sudo apt-get install gnuplot\n\n");
        exit(2);
    }
    
    // the number of lines in the file is the number of data to be saved
    N = linesFile(file);
    if(N < 2){
        printf("\nError: insufficient data number\n");
        exit(2);
    }
    
    // creating data array
    double *x = calloc(N,sizeof(double));
	double *y = calloc(N,sizeof(double));
	if(x == NULL || y == NULL){
	        perror("\nerror");
	        printf("\n");
	        exit(1);
    }
    
    FILE *inputFile = fopen(file,"r");
    if(inputFile == NULL){
        perror("\nError");
        exit(1);
    }
    
    // data reading from file
    for(i=0; i<N; i++){
        fscanf(inputFile,"%lf %lf %lf\n",&x[i],&y[i],&error);
    }
    
    fclose(inputFile);

	// determine linear coeff.
	M = Mbest(N,x,y);
	Q = Qbest(N,x,y);
	
	// data fitting and plotting graph
	fit(M,Q,x,N);
	
	free(x);
	free(y);
    
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

void fit(double M, double Q, double array[], int dim){
    long long int i;
    double x, y, inf, sup;
    FILE *fit = fopen("fit.dat","w");
    if(fit == NULL){
        perror("\nError");
        exit(1);
    }
    
    inf = min(array,dim);
    sup = max(array,dim);
    x = inf - 1;
    // +1 e -1 to make an extrapolation from the fit of a unit
    
    do{
        y = M * x + Q;
        fprintf(fit,"%lf %lf\n",x,y);
        x += INC;
    }while(x <= sup + 1);
    
    fflush(fit);
    fclose(fit);
    
    return ;
}

double max(double array[], int dim){
    long long int i;
    double max=-1E20;
    
    for(i=0; i<dim; i++){
        if(max < array[i]){
            max = array[i];
        }
    }
    
    return max;
}

double min(double array[], int dim){
    long long int i;
    double min=1E20;
    
    for(i=0; i<dim; i++){
        if(min > array[i]){
            min = array[i];
        }
    }
    
    return min;
}

int linesFile(char file[]){
    int lines=0;
    char c;
    
    FILE *input = fopen(file,"r");
    if(input == NULL){
        perror("\nError");
        exit(1);
    }
    
    while((c = getc(input)) != EOF){
        if(c == '\n'){
            lines++;
        }
    }
    
    fclose(input);
    
    return lines;
}

int control(void){
    char path[] = "/usr/bin/gnuplot";
    
    FILE *pf = fopen(path,"r");
    if(pf == NULL){
        fclose(pf);
        return 1;
    }else{
        fclose(pf);
        return 0;
    }
}
