#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#define MAX 20

typedef struct {  //struct to hold data to be passed to "thread row method" function
    int* A;
    int** B;
    int* C;
    int colB;
    int cArB;
} struct_per_row;

typedef struct { //struct to hold data to be passed to "thread_per_element" function
    int* A;
    int** B;
    int *C;
    int col;
    int cArB;
} struct_per_element;

int thread_per_matrix(int** A,int** B,int** C,int rowA,int colA, int colB); //this function performs the matrix multiplication using only a single thread (the normal method).
int thread_per_row(int** A,int** B,int** C,int rowA,int colA, int colB); //this function computes the multiplication by assinging a thread for each row of the first matrix to compute the corresponding row in the resulting matrix.
int thread_per_element(int** A,int** B,int** C,int rowA,int colA, int colB); //this method is used to calculate the multiplication of two matrices by assigning a thread to compute each element in the resulting matrix.
void* multiply_by_row(void* args); //this function is called by "thread_per_row" function to compute the multiplication of each row in the first matrix with the whole second matrix to obtain the corresponding row in the resulting matrix.
void* multiply_by_element(void* args); //this function is called by "thread_per_element" function to compute the multiplication of a row in the first matix with a column in the second matrix to obtain the corresponding element in the resulting matrix. 



int main(int argc, char *argv[]){

char a[MAX],b[MAX],out_per_matrix[MAX],out_per_row[MAX],out_per_element[MAX],line[100];
int rowA,rowB,colA,colB;
FILE* f;
struct timeval stop1, start1, stop2, start2, stop3, start3;

switch (argc) //checking for main arguments and initializing arrays containing files names depending on the passed arguments
{
case 1:
    strcpy(a,"a");
    strcpy(b,"b");
    strcpy(out_per_matrix,"c");
    strcpy(out_per_row,"c");
    strcpy(out_per_element,"c");
    break;
case 2:
    strcpy(a,argv[1]);
    strcpy(b,"b");
    strcpy(out_per_matrix,"c");
    strcpy(out_per_row,"c");
    strcpy(out_per_element,"c");
    break;
case 3:
    strcpy(a,argv[1]);
    strcpy(b,argv[2]);
    strcpy(out_per_matrix,"c");
    strcpy(out_per_row,"c");
    strcpy(out_per_element,"c");
    break;
case 4:
    strcpy(a,argv[1]);
    strcpy(b,argv[2]);
    strcpy(out_per_matrix,argv[3]);
    strcpy(out_per_row,argv[3]);
    strcpy(out_per_element,argv[3]);
    break;
default:
    printf("invalid number of arguments!\n");
    return 0;
    break;
}

//reading fata from first file containing the first matrix
f=fopen(strcat(a,".txt"),"r");
fgets(line,100,f);
sscanf(line,"row=%d col=%d",&rowA,&colA);

//allocating 2d int array A to hold the first matrix
int** A=(int**) malloc(rowA * sizeof(int*));
for (int i=0;i<rowA;i++){
    A[i]= (int*) malloc(colA* sizeof(int));
}
for(int i=0;i<rowA;i++){
    for(int j=0;j<colA;j++){
        fscanf(f, "%d",&A[i][j]);
    }
}
fclose(f);


//reading fata from first file containing the first matrix
f=fopen(strcat(b,".txt"),"r");
fgets(line,100,f);
sscanf(line,"row=%d col=%d",&rowB,&colB);

//allocating 2d int array B to hold the second matrix
int** B=(int**) malloc(rowB * sizeof(int*));
for (int i=0;i<rowB;i++){
    B[i]= (int*) malloc(colB* sizeof(int));

}
for(int i=0;i<rowB;i++){
    for(int j=0;j<colB;j++){
        fscanf(f, "%d",&B[i][j]);
    }
}
fclose(f);

//allocating 2d int array c_per_matrix to hold the result of multiplication using "thread per matrix" method
int** c_per_matrix=(int**) malloc(rowA * sizeof(int*));
for (int i=0;i<rowA;i++){
    c_per_matrix[i]= (int*) malloc(colB* sizeof(int));

}

//allocating 2d int array c_per_row to hold the result of multiplication using "thread per row" method
int** c_per_row=(int**) malloc(rowA * sizeof(int*));
for (int i=0;i<rowA;i++){
    c_per_row[i]= (int*) malloc(colB* sizeof(int));

}

//allocating 2d int array c_per_element to hold the result of multiplication using "thread per element" method
int** c_per_element=(int**) malloc(rowA * sizeof(int*));
for (int i=0;i<rowA;i++){
    c_per_element[i]= (int*) malloc(colB* sizeof(int));

}


gettimeofday(&start1, NULL); //start checking time
int t_num1=thread_per_matrix(A,B,c_per_matrix,rowA,colA,colB); //calling the "thread per matrix" function that performs multiplication using a single thread
gettimeofday(&stop1, NULL); //end checking time
f=fopen(strcat(out_per_matrix,"_per_matrix.txt"),"w"); //writing the results into "thread per matrix" file
fprintf(f,"Method: A thread per matrix\n");
fprintf(f,"row=%d col=%d\n",rowA,colB);
for(int i=0;i<rowA;i++){
    for(int j=0;j<colB;j++){
        fprintf(f,"%d ",c_per_matrix[i][j]);
    }
    fprintf(f,"\n");
}
fclose(f);

gettimeofday(&start2, NULL); //start checking time
int t_num2=thread_per_row(A,B,c_per_row,rowA,colA,colB); //calling the "thread per row" function that performs multiplication using a thread for each row in the first matrix
gettimeofday(&stop2, NULL); //end checking time
f=fopen(strcat(out_per_row,"_per_row.txt"),"w"); //writing the results into "thread per row" file
fprintf(f,"Method: A thread per row\n");
fprintf(f,"row=%d col=%d\n",rowA,colB);
for(int i=0;i<rowA;i++){
    for(int j=0;j<colB;j++){
        fprintf(f,"%d ",c_per_row[i][j]);
    }
    fprintf(f,"\n");
}
fclose(f);

gettimeofday(&start3, NULL); //start checking time
int t_num3=thread_per_element(A,B,c_per_element,rowA,colA,colB); //calling the "thread per element" function that performs multiplication using a thread for each element
gettimeofday(&stop3, NULL); //end checking time
f=fopen(strcat(out_per_element,"_per_element.txt"),"w"); //writing the results into "thread per element" file
fprintf(f,"Method: A thread per element\n");
fprintf(f,"row=%d col=%d\n",rowA,colB);
for(int i=0;i<rowA;i++){
    for(int j=0;j<colB;j++){
        fprintf(f,"%d ",c_per_element[i][j]);
    }
    fprintf(f,"\n");
}
fclose(f);

//deallocating memory that held the matrices and their results of multiplication
for (int i = 0; i < rowA; i++) {
        free(A[i]);
        free(c_per_matrix[i]);
        free(c_per_row[i]);
        free(c_per_element[i]);
    }
free(A);
free(c_per_matrix);
free(c_per_row);
free(c_per_element);
for (int i = 0; i < rowB; i++) {
        free(B[i]);
    }
free(B);


// comparing the number of threads and execution time for each method
printf("Method 1: A thread per matrix\n");
printf("Number of threads created = %d\n",t_num1);
printf("Time taken for this method in Seconds = %lu\n",stop1.tv_sec-start1.tv_sec);
printf("Time taken for this method in Microseconds= %lu\n\n",stop1.tv_usec-start1.tv_usec);

printf("Method 2: A thread per row\n");
printf("Number of threads created = %d\n",t_num2);
printf("Time taken for this method in Seconds = %lu\n",stop2.tv_sec-start2.tv_sec);
printf("Time taken for this method in Microseconds= %lu\n\n",stop2.tv_usec-start2.tv_usec);

printf("Method 3: A thread per element\n");
printf("Number of threads created = %d\n",t_num3);
printf("Time taken for this method in Seconds = %lu\n",stop3.tv_sec-start3.tv_sec);
printf("Time taken for this method in Microseconds= %lu\n\n",stop3.tv_usec-start3.tv_usec);

}


int thread_per_matrix(int** A,int** B,int** C,int rowA,int colA, int colB){
int res=0;
int t_num=1;
for(int i=0;i<rowA;i++){
    for(int j=0;j<colB;j++){
        for(int h=0;h<colA;h++){
            res+=A[i][h]*B[h][j];
        }
        C[i][j]=res;
        res=0;
    }
}
return t_num;
}
int thread_per_row(int** A,int** B,int** C,int rowA,int colA, int colB){
int t_num=rowA;
pthread_t* threads= malloc(rowA* sizeof(pthread_t));
struct_per_row* mystructs = malloc(rowA* sizeof(struct_per_row));
for(int i=0;i<rowA;i++){
    mystructs[i].A=A[i];
    mystructs[i].B=B;
    mystructs[i].C=C[i];
    mystructs[i].colB=colB;
    mystructs[i].cArB=colA;
    pthread_create(&threads[i],NULL,multiply_by_row,(void *)&mystructs[i]);
}

for(int x=0;x<rowA;x++){
    pthread_join(threads[x],NULL);
}

free(threads);
free(mystructs);
return t_num;
}
int thread_per_element(int** A,int** B,int** C,int rowA,int colA, int colB){
int t_num=rowA*colB;
pthread_t** mythreads=malloc(rowA*sizeof(pthread_t*));
for(int i=0;i<rowA;i++){
    mythreads[i]=malloc(colB*sizeof(pthread_t));
}

struct_per_element** mystructs=malloc(rowA*sizeof(struct_per_element*));
for(int i=0;i<rowA;i++){
    mystructs[i]= malloc(colB*sizeof(struct_per_element));
}

for(int i=0;i<rowA;i++){
    for (int j=0;j<colB;j++){
        mystructs[i][j].A=A[i];
        mystructs[i][j].B=B;
        mystructs[i][j].col=j;
        mystructs[i][j].C=&C[i][j];
        mystructs[i][j].cArB=colA;
        pthread_create(&mythreads[i][j],NULL,multiply_by_element,(void*)&mystructs[i][j]);
    }
}

for(int i=0;i<rowA;i++){
    for (int j=0;j<colB;j++){
        pthread_join(mythreads[i][j],NULL);
    }
}
for(int i=0;i<rowA;i++){
 free(mythreads[i]);
 free(mystructs[i]);   
}
free(mythreads);
free(mystructs);
return t_num;
}
void* multiply_by_row(void* args){
struct_per_row *new= (struct_per_row*) args;
int res=0;
for(int i=0;i<new->colB;i++){
    for(int j=0;j<new->cArB;j++){
        res+=new->A[j]*new->B[j][i];
    }
    new->C[i]=res;
    res=0;
}
}
void* multiply_by_element(void* args){
    struct_per_element* new=(struct_per_element*) args;
    int res=0;
    for(int i=0;i<new->cArB;i++){
        res+=(new->A[i])*(new->B[i][new->col]);
    }
    *(new->C)=res;

}




