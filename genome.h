#ifndef __GENOME_H_
#define __GENOME_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_KMER_LEN 16

// project char A,C,G,T to int 0,1,2,3
int getval(char c){
  switch (c){
    case 'A': case 'a':
      return 0;
    case 'C': case 'c':
      return 1;
    case 'G': case 'g':
      return 2;
    case 'T': case 't':
      return 3;
    default:
      return 0;
  }
  return 0;
}

// constuct GNode
typedef struct GNode{ 
    char* key;/// Will hold strings only consisting of ACGT 
    struct GNode* inEdges[4]; // what is the meaning of [4]
    struct GNode* outEdges[4]; 
    int key_len;/// this is our k length 
}GNode;


void InitGNode(GNode* node, char* k, int kl){
  node->key = (char*)malloc(sizeof(char)*(kl+1));
  strncpy(node->key, k, kl); // copy lenght kl of k to node->key
  node->key[kl] = '\0'; // indicating the end of the str
  //printf("Init GNode >>> %s \n",node->key);
  node->key_len = kl; // len of k
  for(int i = 0; i < 4; ++i){
    node->inEdges[i] = NULL;
    node->outEdges[i] = NULL;
  }
}

int hash(char* key, int key_len, int GRAPH_SIZE){
  ///this can be used to determine if string exists already
  int toRet = 0;
  int i;
  for(i = 0; i < key_len; ++i){
    toRet = toRet << 2; // shift bit by 2
    toRet |= getval(key[i]); // or operation
  }
  toRet = toRet % GRAPH_SIZE;
  return toRet; // A T G C corresponds to 00, 01, 10, 11. 
  // We return a number of length key_len*2, to record each element. 
}

int setGraphSize(int k_len){
  // only have four element A,C,G,T; total possible sets are pow(4,k_len)
  int GRAPH_SIZE = 1;
  switch(k_len){
    case 1: GRAPH_SIZE = 4; break;
    case 2: GRAPH_SIZE = 16; break;
    case 3: GRAPH_SIZE = 64; break;
    case 4: GRAPH_SIZE = 256; break;
    case 5: GRAPH_SIZE = 1024; break;
    case 6: GRAPH_SIZE = 4096; break;
    case 7: GRAPH_SIZE = 16384; break;
    case 8: GRAPH_SIZE = 65536; break;
    case 9: GRAPH_SIZE = 262144; break;
    case 10: GRAPH_SIZE = 1048576; break;
    case 11: GRAPH_SIZE = 4194304; break;
    default: GRAPH_SIZE = 16777216; break;
  }
  return GRAPH_SIZE;
}

void AddEdges(GNode* from, GNode* to){
  if (to->inEdges[getval(from->key[0])] == NULL){
  from->outEdges[getval(to->key[to->key_len-1])] = to;
  to->inEdges[getval(from->key[0])] = from;
  }
}

int getLineLength(char* inputFilename){
  FILE* input;
  int len = 0;
  input = fopen(inputFilename, "r");
  
  if(!input){
    printf("Could not open input file!!\n");
    exit(1);
  }

  char* buff = malloc(1000000*sizeof(char));
  // get the first line of the file
  fscanf(input, "%s", buff);
   
  len = strlen(buff);
  
  printf("Content of the first line is >>> %s \n", buff);
  printf("Length of each line is >>> %d \n", len);
  
  fclose(input);
  free(buff);
  return len;
}

int InDegree(GNode* node){
    int count = 0;
    for(int j = 0; j < 4; ++j){
      if (node->inEdges[j]){
        count ++;
      }
    }
    return count;
}

int OutDegree(GNode* node){
    int count = 0;
    for(int j = 0; j < 4; ++j){
      if (node->outEdges[j]){
        count ++;
      }
    }
    return count;
}

#endif //__GENOME_H_