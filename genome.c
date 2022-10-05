#include <stdio.h>
#include <stdlib.h>

#include "genome.h"


void FreeHashGraph(GNode** hashGraph, int GRAPH_SIZE){
  ///loop through your hashGraph and free all existing nodes
  ///Then free the hashGraph
  /*
  if (*hashGraph != NULL){
    for(int j = 0; j < 4; ++j){
      if ((*hashGraph)->outEdges[j]){
        FreeHashGraph(&(*hashGraph)->outEdges[j], GRAPH_SIZE);
      }
    }
    free(*hashGraph);
  } */

  for(int i = 0; i < GRAPH_SIZE; ++i){
    
    if (hashGraph[i] != NULL){
        free(hashGraph[i]->key);
        free(hashGraph[i]);
    }
  }
  free(hashGraph);
}


void BuildGraph(char* inputFilename, char* nodeFilename, int k_len, GNode** hashGraph, int GRAPH_SIZE){
  int lineLen = getLineLength(inputFilename);
  FILE* ifptr = fopen(inputFilename, "r");
  if(!ifptr){
    printf("Could not open input file!!\n");
    FreeHashGraph(hashGraph, GRAPH_SIZE);
  }
  //char* buffer = (char*)malloc(sizeof(char)*lineLen + 2); 
  //fread(buffer, strlen(c)+1, 1, fp);
  
  // build the hash_array and initialize
  // 1 means exits, 0 means not exits
  //int* hash_array = (int*)malloc(sizeof(int)*GRAPH_SIZE);
  //for (int i=0; i<GRAPH_SIZE; i++){
  //    hash_array[i] = 0 ;
  //}
  while (!feof(ifptr)){
      char* buffer = (char*)malloc(sizeof(char)*lineLen + 2); 
      fscanf(ifptr,"%s\n", buffer);
      //fgets(buffer, lineLen, ifptr);
      //printf("New Line >>>>>> %s \n",buffer);
      GNode* prev=NULL, *curr=NULL;
  
      for (int i=0; i<lineLen-k_len+1; i++){
          // get i th substring
          //char str_new[k_len+1];
          //str_new[k_len] = '\0'; // make sure printf is good
          //strncpy(str_new,&buffer[i],k_len);
          char * str_new = (char *)malloc(sizeof(char)*(k_len+1));
          strncpy(str_new,&buffer[i],k_len);
          str_new[k_len] = '\0';
          //printf("New node >> %s \n",str_new);

          int nodeHash = hash(str_new, k_len, GRAPH_SIZE);///hash defined in genome.h
          
          
          if (hashGraph[nodeHash] == NULL){
              GNode* newNode = (GNode*)malloc(sizeof(GNode));
              InitGNode(newNode, str_new, k_len);
              //GNode* temp = newNode;
              hashGraph[nodeHash] = newNode;
              
              //printf("k_len >> %d \n",k_len);    
              //free(newNode);
          }
          prev = curr;
          curr = hashGraph[nodeHash];
          if (prev != NULL){
                  //printf("Before_IN >>> %d \n",InDegree(curr));
                  //printf("Before_OUT >>> %d \n",OutDegree(curr));
                  AddEdges(prev, curr);
                  //printf("After_IN >>> %d  \n",InDegree(curr));
                  //printf("After_OUT >>> %d  \n",OutDegree(curr));
          }
          free(str_new);
      }
      free(buffer);
  }
  fclose(ifptr);

  // for each line in the input file:
  //   prev=NULL;
  //   curr=NULL;
  //   for each k_len substring in the line:
  //     int nodeHash = hash(str, k_len, GRAPH_SIZE);///hash defined in genome.h
  //     check if node exists already(handle collisions). 
  //     If it doesn't exist, malloc and use InitGNode
  //     Code may be something like:
  //       GNode* newNode = (GNode*)malloc(sizeof(GNode));
  //       InitGNode(newNode, str, k_len);
  //     If there is an error at any point, make sure we free what we have malloc
  //       maybe use FreeHashGraph(hashGraph, GRAPH_SIZE); for ease
  //     set curr equal to the node, either the one we malloc or the one existing
  //     if(prev)
  //       AddEdges(prev, curr);///Definition in genome.h
  //     prev=curr;

  //free(hash_array);
  //free(buffer);
}

void TraverseHelper(GNode* node, FILE* outfile){
  fprintf(outfile, "%c", node->key[node->key_len-1]); // only need to output laser character of the key
  //printf("%c", node->key[node->key_len-1]);
  if(InDegree(node) == 1 && OutDegree(node) == 1){ // the hub node will be printed but not traversed.
    for(int j = 0; j < 4; ++j){
      if (node->outEdges[j]){
        TraverseHelper(node->outEdges[j], outfile);
        break; // only one outEdges exists
      }
    }
  }
}

void TraverseGraph(GNode** hashGraph, int k_len, int GRAPH_SIZE, char* outputFilename){
  FILE* outfile = fopen(outputFilename, "w");
  /*Do some sort of loop that will start a traversal at every hnode
  * use TraverseHelper to traverse after we are at hnode to make this easier
  */
  // find the hub node
  for(int i = 0; i < GRAPH_SIZE; ++i){
    if (hashGraph[i] != NULL){
      
        //if ((InDegree(hashGraph[i]) != 1 || OutDegree(hashGraph[i]) != 1) && OutDegree(hashGraph[i]) >= 1){
        if (InDegree(hashGraph[i]) != 1 || OutDegree(hashGraph[i]) != 1){
        
            //printf("New traversal >>>>"); // new line
            //printf("\n Out Degree >>>>> %d \n",OutDegree(hashGraph[i]));
            //for (int j =0; j< (hashGraph[i]->key_len); j++){
            //    fprintf(outfile, "%c", hashGraph[i]->key[j]); // print the first len-1 character
            //    printf("%c", hashGraph[i]->key[j]);
            //}
            for(int j = 0; j < 4; ++j){
                if (hashGraph[i]->outEdges[j]){
                    fprintf(outfile, "%s", hashGraph[i]->key);
                    //printf( "%s", hashGraph[i]->key);
                    TraverseHelper(hashGraph[i]->outEdges[j], outfile);
                    fprintf(outfile, "\n");
                    //printf("\n");
                }
            }
            
            //fprintf(outfile, "\n"); // change line
            //printf("\n"); // change line
        }     
    }
  }
  

  /*
  if (*hashGraph != NULL){
    for(int j = 0; j < 4; ++j){
      if ((*hashGraph)->outEdges[j]){
        TraverseGraph(&(*hashGraph)->outEdges[j],k_len,  GRAPH_SIZE, outputFilename);
      }
    }
    
    if ((InDegree((*hashGraph)) != 1 || OutDegree((*hashGraph)) != 1) && OutDegree((*hashGraph)) >= 1){
        for (int i =0; i< ((*hashGraph)->key_len-1); i++){
            fprintf(outfile, "%c", (*hashGraph)->key[i]); // print the first len-1 character
        }
        TraverseHelper((*hashGraph), outfile);
        fprintf(outfile, "\n"); // change line
    }
  }*/

  fclose(outfile);
}

void PrintUniqueNodes(char* nodeFilename, GNode** hashGraph, int GRAPH_SIZE){
  FILE* nodeOut = fopen(nodeFilename, "w");
  /*do some sort of loop to print all unique nodes here*/
  /*if (*hashGraph != NULL){
    for(int j = 0; j < 4; ++j){
      if ((*hashGraph)->outEdges[j]){
        PrintUniqueNodes(nodeFilename,&(*hashGraph)->outEdges[j], GRAPH_SIZE);
      }
    }
  }*/

  printf("\n start to print out unique nodes >>>>>>> \n");
  for(int i = 0; i < GRAPH_SIZE; ++i){

    if (hashGraph[i] != NULL){
        //printf("Node %d >>> %s \n",i+1,(hashGraph[i])->key);
        //printf("Unique Node >>> %d \n",(hashGraph[i])->key_len); 
        fprintf(nodeOut, "%s\n",(hashGraph[i])->key);
        
    }
  }

  //fprintf(nodeOut, "%s",(*hashGraph)->key);
  fclose(nodeOut);
}

int main(int argc, char* argv[]){
  // argv[1] should be an integer of what our k length should be.
  // argv[2] will be the input filename that will contain the different reads. Each line in the
  // input file corresponds to one read.
  // argv[3] will be the filename of where you should output the strings from your traversal
  if(argc != 5){
    printf("Usage: ./pa3 <k_len> <inputFilename> <outputFilename> <nodeoutputFilename>\n");
    exit(1);
  }
  int k_len = atoi(argv[1]);
  if(k_len < 1){
    printf("k_len must be an integer greater than 0\n");
    exit(1);
  }
  char* inputFilename = argv[2];
  char* outputFilename = argv[3];
  char* nodeFilename = argv[4];

  // create hashGraph to store all possible unique nodes
  int GRAPH_SIZE = setGraphSize(k_len);///defined in genome.h
  GNode** hashGraph = (GNode**)malloc(sizeof(GNode*)*GRAPH_SIZE);
  for(int i = 0; i < GRAPH_SIZE; ++i){
    hashGraph[i] = NULL;//intialize just in case
  }

  ///Build the graph from the input file
  BuildGraph(inputFilename, nodeFilename, k_len, hashGraph, GRAPH_SIZE);
  printf("\n Finish >>>> Build the graph \n");

  ///Output the unique nodes
  PrintUniqueNodes(nodeFilename, hashGraph, GRAPH_SIZE);
  printf("\n Finish >>>> Output the unique nodes \n");

  ///output of the output strings
  TraverseGraph(hashGraph, k_len, GRAPH_SIZE, outputFilename);
  printf("\n Finish >>>> output of the output strings \n");

  ///Make sure we free the hashGraph correctly. 
  FreeHashGraph(hashGraph, GRAPH_SIZE);
  printf("\n Finish >>>> Free the hashGraph \n");

  return 0;
}





