#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "sequence.h"
#include "shell_array.h"

long *Array_Load_From_File(char *filename, int *size){
    
    //FILE* load_file = fopen(filename, "rb");
    FILE* load_file = fopen(filename, "rb");
    if (!load_file) {
        //perror("fopen");
        printf("\n EXIT_FAILURE \n");
        exit(EXIT_FAILURE);
    }
    
    // calculate the size of the file
    fseek(load_file, 0L, SEEK_END);
    long int file_size = ftell(load_file);
    // calculate the size of the array for long
    *size = file_size/8; // sizeof(long) = 8
    // need to close the file as the fseek already go to the end of the file
    fclose(load_file);
    
    // re-open the file for reading
    load_file = fopen(filename, "rb");

    // malloc based on the size of calculated size
    long * ret = (long *)malloc((*size)*8); // sizeof(long) = 8
    
    int num_element = fread(ret, 8, (*size), load_file);
    printf("read %ld element  \n", num_element);
    
    for (int i=0; i<*size; i++){
        printf("read %d >>  %ld \n", i+1, ret[i]);
    }

    fclose(load_file);
    return ret;
}

int main(){
    char* filename = "1000.b";
    int size = 0;
    Array_Load_From_File(filename,&size);
    
    printf("Num of long in input file: %d \n",size);
    return 0;
}