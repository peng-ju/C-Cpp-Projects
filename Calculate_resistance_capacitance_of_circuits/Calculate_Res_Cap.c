#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct _Node{
/* left and right wire lengths
assign -1.0 for leaf nodes */
float lwire_len;
float rwire_len;
/* capacitance at a node can be computed 
at creation and linking with parent nodes */
float cap_at_node;
float cap_at_node_r;
/* total resistance in the path from root to this node.
Assign 0.0 at node creation and later computed using a traversal. */
float res_at_node;
/* total capacitance in the subtree rooted at this node.
Assign 0.0 at node creation and later computed using a traversal. */
float cap_at_tree;
// left and right node pointers
struct _Node* left;
struct _Node* right;
// label of the leaf node. Set as zero if it is not a leaf node.
int label;
// sum of res times cap for the delay calculation
float sum;
}Node;

/*
//double the stack size if it is full.
void expand(){ 
	mystruct* myrealloced_array = realloc(myarray, myarray_size * sizeof(mystruct));
}
*/

typedef struct _Stack{
	struct _Stack* next;
	Node* node;
}Stack;

typedef struct _Stack_front{
	Stack *front;
}Stack_front;

void push(Stack_front * stack_front, Node* node){
	Stack* toInsert = (Stack *)malloc(sizeof(Stack));
	toInsert->node = node;
	toInsert->next = NULL;
	if(stack_front->front == NULL){
		stack_front->front = toInsert;
	}
	else{
		toInsert->next = stack_front->front;
		stack_front->front = toInsert;
	}
}

Node* pop(Stack_front *stack_front){
	if(stack_front->front == NULL)
		return NULL;
	Stack *toPop = stack_front->front;
	stack_front->front = stack_front->front->next;
	Node* toReturn = toPop ->node;
	free(toPop);
	return toReturn;
}

/*
// copy a node
struct _Node* Node_Copy(struct _Node* node){
	struct _Node* node_copy= (struct _Node *) malloc(sizeof(struct _Node));
	node_copy->lwire_len = node->lwire_len;
	node_copy->rwire_len = node->rwire_len;
	node_copy->cap_at_node = node->cap_at_node;
	node_copy->cap_at_node_r = node->cap_at_node_r;
	node_copy->cap_at_tree = node->cap_at_tree;
	node_copy->res_at_node = node->res_at_node;
    node_copy->sum = node->sum;

	node_copy->left = node->left;
	node_copy->right = node->right;
    node_copy->label = node->label;
    
	//free(node);
	return node_copy;
}*/

// pre_oder tranversal of a tree and write into file
void pre_order_tranversal(Node *node, FILE *file){
	if (node != NULL){
		if (node->label == 0){
            fprintf(file,"(%le %le)\n",node->lwire_len,node->rwire_len);
			printf("Write pre_order: (%f %f)\n",node->lwire_len,node->rwire_len);   
		}
		else{
            fprintf(file,"%d(%le)\n",node->label,node->cap_at_node_r);
			printf("Write pre_orde: %d(%f)\n",node->label,node->cap_at_node_r);  
		}
		pre_order_tranversal(node->left, file);
		pre_order_tranversal(node->right, file);
	}
	else{return;}
}

// pre-order tranverse to calculate res_at_node

void node_res_cal(Node *node,float res_unit){
	if (node==NULL) return;
	if (node->left){
		node->left->res_at_node += (node->lwire_len * res_unit + node->res_at_node);
		node->right->res_at_node += (node->rwire_len * res_unit + node->res_at_node);
		//printf("l res_at_node: %f \n",node->left->res_at_node);
		node_res_cal(node->left, res_unit);
		//printf("r res_at_node: %f \n",node->right->res_at_node);
		node_res_cal(node->right, res_unit);
	}	
}

// pre-order tranverse to calculate the sum
void node_sum(Node *node, FILE *file){
    if (node == NULL) return;
	if (node->left){
        node->left->sum += (node->sum +  (node->cap_at_tree - node->left->cap_at_tree) *pow(node->res_at_node,2.0));
		node->right->sum += (node->sum +  (node->cap_at_tree - node->right->cap_at_tree) *pow(node->res_at_node,2.0));

		node_sum(node->left, file);
		node_sum(node->right, file);
	}
	else{ // output delay
        node->sum += node->cap_at_tree * pow(node->res_at_node,2.0);
        double delay = node->sum / node->res_at_node;
        //fprintf(file,"%d(%le)\n",node->label ,delay);
		
		fwrite(&node->label, sizeof(int) ,1, file); 
        fwrite(&delay, sizeof(double) ,1, file);  
		printf("Write delay: %d(%le)\n",node->label ,delay); 
	}
}

// function to free the tree 
void delete(Node* root) {
    if (root != NULL) {
        delete(root->left);
        delete(root->right);
        free(root); // free root
    }
	else{
		return;
	}
}

/*
// function to free the stack
void delete_stack(struct _Node* stack, int size_stack) {
    for (int i=0; i<size_stack; i++){
		free(stack[i]);
	}

	if (root != NULL) {
        delete(root->left);
        delete(root->right);
        free(root); // free root
    }
}*/

int main(int argc, char *argv[]){
	// create pointers for the files
	FILE *input, *output, *delay;
    if (argc==4){
        input = fopen(argv[1],"r");
		output = fopen(argv[2],"w");
		delay = fopen(argv[3],"wb");
	}
	else{
		printf("EXIT_FAILURE");
		return 0;
	}
	
	//input = fopen("3.txt", "r");
	//output = fopen("3.pre","w");
    //delay = fopen("3.delay","w");
	
	// important parameters
	float res_src ; // the output resistance at the source
	float res_unit; // per-unit-length resistance of a wire
	float cap_unit; // per-unit-length capacitance of a wire
	
  
	// create two stacks to store the leaf node info for BST
	Stack* stack = (Stack*) malloc(sizeof(Stack));
    Stack_front* stack_front = (Stack_front*) malloc(sizeof(Stack));

	stack->next = NULL;
	stack->node = NULL;
	stack_front->front = NULL;

	/*
	int size_stack = 2; // initial stack size
	int pos_stack = 0; // position of the stack top
    
	struct _Node *stack;
	stack = (struct _Node *) malloc(size_stack*sizeof(struct _Node));   // stack to store node 
    */
    // current line number for reading
	int r_line = 0;
	//int w_line_o = 0;
	//int w_line_d = 0;
	
	// read the first line of the input file
	fscanf(input,"%f %f %f", &res_src, &res_unit, &cap_unit); // store the value to the pointer of each variables
    printf("the output resistance at the source is:%f \n", res_src); 
	printf("the per-unit-length resistance of a wire is:%f \n", res_unit); 
	printf("the per-unit-length capacitance of a wire is:%f \n", cap_unit);
	
    // read the rest of the input
    int r_label,p_label = -1; // r_label is current label, p_label is the previous label
	float r_data1, r_data2; // at most two data is read from each line

    while (!feof(input)) {
		fscanf(input,"%d(%f)\n", &r_label, &r_data1); // if the next line is not leaf ndoe, it return the last label

		if (r_label != p_label){
			
			printf("Read line %d: %d(%f) \n",r_line++, r_label, r_data1);
			
			// create new leaf node
			//struct _Node *new_node = malloc(sizeof(struct _Node));
            Node *new_node = (Node *)malloc(sizeof(Node));

			new_node->label = r_label;
            new_node->lwire_len = 0.0;
			new_node->rwire_len = 0.0;
			new_node->cap_at_node_r = r_data1;
			new_node->cap_at_node = r_data1;
			new_node->res_at_node = 0.0;
			new_node->cap_at_tree = r_data1;
			new_node->right = NULL;
			new_node->left = NULL;
            new_node->sum = 0.0;
			// push stack
            push(stack_front,new_node);
			//stack[pos_stack] = *new_node;
			//pos_stack++;
            
			// expand stack if full
            //if (pos_stack == size_stack-1){
            //    size_stack *= 2;
			//	stack = (struct _Node*) realloc(stack ,size_stack*sizeof(struct _Node)); 
			//	printf("realloc memory for stack\n");
			//}

			// update p_label
			p_label = r_label;
		}
		else{
            fscanf(input,"(%f %f)\n", &r_data1, &r_data2);
			printf("Read line %d: (%f %f)\n", r_line++, r_data1, r_data2);
            
			// pop stack
			if (stack_front != NULL){
				//pos_stack--;
                Node *node_1 = pop(stack_front);
				//free(&stack[pos_stack]);
				//stack[pos_stack] = *NULL;
				//pos_stack--;
				Node *node_2 = pop(stack_front);;
				//free(&stack[pos_stack]);

				// create new leaf node
			    Node *new_node = (Node *) malloc(sizeof(Node));
			    new_node->label = 0; // =0 if not a leaf node
				new_node->cap_at_node_r = 0.0;
                new_node->lwire_len = r_data1; 
			    new_node->rwire_len = r_data2;
			    //new_node->cap_at_node = node_1->cap_at_node + node_2->cap_at_node + (new_node->lwire_len + new_node->rwire_len)*cap_unit/2;
                //new_node->cap_at_node = (new_node->lwire_len + new_node->rwire_len)*cap_unit/2;
                
			    new_node->res_at_node = 0.0; //node_1->res_at_node + node_2->res_at_node + (new_node->lwire_len + new_node->rwire_len)*res_unit;
			    
			    new_node->right = node_1; //node_1;
			    new_node->left = node_2; //node_2;
                new_node->sum = 0.0;

				// add wire cap the children
                new_node->left->cap_at_node += new_node->lwire_len*cap_unit/2;
				new_node->right->cap_at_node += new_node->rwire_len*cap_unit/2;

				new_node->left->cap_at_tree += new_node->lwire_len*cap_unit/2;
				new_node->right->cap_at_tree += new_node->rwire_len*cap_unit/2;
                
				new_node->cap_at_node = (new_node->lwire_len + new_node->rwire_len)*cap_unit/2;
				//printf("check %f \n",cap_unit/2);
				new_node->cap_at_tree = node_1->cap_at_tree + node_2->cap_at_tree + new_node->cap_at_node;
				// push new treenode 
				//stack[pos_stack] = *new_node;
			    //pos_stack++;
				push(stack_front,new_node);

			}
			else{printf("Error: Empty stack");} 
		}
    }
    
	printf("\n >>>>>>>>>>>>>>>>>> \n Reading is done \n");
	//struct _Node *root_node = &stack[pos_stack];
    Node *root_node = pop(stack_front);
	// write the pre-order printing of the RC tree
    pre_order_tranversal(root_node, output);

    /* calculate the delay at each leaf node
	two steps:
	step1: calculate the res between source and each node
	step2: calculate the sum part of the equation for each node, at each leaf node, output the delay
 	*/

	// step1: pre-order tranverse and calculate the res_at_node
	root_node->res_at_node = res_src; // source resisit
	node_res_cal(root_node,res_unit);
    
	// step2: calculate sum for each node
    node_sum(root_node,delay);

    printf("\n >>>>>>>>>>>>>>>> \n Finish \n Looks good! \n"); 

    // close file at the end
    fclose(input);
    fclose(output);
	fclose(delay);

	// free memory
	delete(root_node);
	free(stack_front);
	free(stack);
    //free(root_node);

    printf("\n >>>>>>>>>>>>>>>> \n Memory free \n");
	return 0;
}
