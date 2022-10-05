#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sequence.h"

long *Generate_2p3q_Seq(int length, int *seq_size){
    // calculate max(p,q)
    int p_plus_q = 1 + log(length)/log(2);
    
    // pointer to for the sequence
    long *sequence = (long *)malloc(length*sizeof(long));
    
    // size of the sequence
    int sequence_size = 0;
    

    for (int p = 0; p<p_plus_q; p++){
        for (int q=0; q<p_plus_q; q++){
            int element = pow(2,p)*pow(3,q);
            if (element < length){
                // printf("size of the sequence is: %d \n", sequence_size);
                sequence[sequence_size++] = element; 
            }
        }
    }
    *seq_size = sequence_size; 
    
    // sort the sequence with insert sort
    for (int i=1; i<sequence_size; i++){
        int temp = sequence[i];
        while (i>0 && sequence[i-1] > temp){
            sequence[i] = sequence[i-1];
            i--;
        }
        sequence[i] = temp;
    }

    return sequence;
};

int main(){
    int length = 16;
    int seq_size;

    long *sequence = (long *)malloc(length*sizeof(long));

    sequence = Generate_2p3q_Seq(length, &seq_size);
    
    printf("size of the sequence is: %d \n", seq_size);

    // print all element of the sequence
    printf("Sequence:");
    for (int i=0; i < seq_size; i++){
        printf("%d,", sequence[i]);
    }
    printf("\n");

    printf("\n >>>>>>>>>>>> complete \n");
    free(sequence);
    printf("\n >>>>>>>>>>>> free \n");
    return 0;
}