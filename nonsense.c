#include<stdlib.h>
#include<stdio.h>//remove later
#include<string.h>
#include<time.h>
typedef struct node {//markov chain and binary tree
	char* word;
	struct node* left;
	struct node* right;
	struct node* parent;
	unsigned int arr_length;
	struct node** neighbours;
	unsigned int* weights;
	unsigned int sum;
} node;
int NODE_SIZE,POINTER_SIZE;
unsigned long int words;
node* new_node(char* word,node* ancestor){ 
	node* n=malloc(NODE_SIZE); 
	n->word=(char*)malloc(strlen(word)+1);
	strcpy(n->word,word);
	n->arr_length=0;
	n->sum=0;//sum of weights
	n->left=NULL;
	n->right=NULL;
	n->parent=ancestor;
	n->weights=(unsigned int*)malloc(0);
	n->neighbours=(node**)malloc(0);
	words+=1;
	return n;
}
int main(int argc,char** argv){
	NODE_SIZE=sizeof(node);
	POINTER_SIZE=sizeof(void*);
	words=0;
	//read from file
	FILE* file_pointer=fopen("data.txt","rb");
	fseek(file_pointer,0L,SEEK_END);
	int size=ftell(file_pointer);
	rewind(file_pointer);
	char* file_contents=(char*)malloc(size+1);
	file_contents[size]='\0';
	fread(file_contents,size,1,file_pointer);
	fclose(file_pointer);
	//create structure
	char* spacebar=(char*)malloc(1);
	*spacebar=' ';
	char* word=strtok(file_contents,spacebar);
	node* head=new_node(word,NULL);
	node* prev_node=head;
	word=strtok(NULL,spacebar);
	node* cur;node* last;int det;unsigned i,l;
	do{
		cur=head;
		//binary tree link
		for(;;){
			det=strcmp(word,cur->word);
			last=cur;
			if(det==0)
				break;
			else if(det>0){
				cur=cur->right;
				if(cur==NULL){
					cur=last->right=new_node(word,prev_node);
					break;
				}
			}
			else if(det<0){
				cur=cur->left;
				if(cur==NULL){
					cur=last->left=new_node(word,prev_node);
					break;
				}
			}
		}
		//markov link
		prev_node->sum++;
		for(i=0;i<prev_node->arr_length;i++)//linear search
			if(prev_node->neighbours[i]==cur){
				prev_node->weights[i]++;
				goto next_word;
			}
		//add neighbour
		l=i+1;
		prev_node->neighbours=(node**)realloc(prev_node->neighbours,l*POINTER_SIZE);
		prev_node->neighbours[i]=cur;
		prev_node->weights=(unsigned int*)realloc(prev_node->weights,4*l);
		prev_node->weights[i]=1;
		prev_node->arr_length=l;
		next_word:
		prev_node=cur;
		word=strtok(NULL,spacebar);
	}while(word!=NULL);
	//generate nonsense
	unsigned char no_left,no_right;unsigned int len;unsigned int* weights;
	for(;;){
		srand(time(0));
		//choose random node
		cur=head;
		for(l=rand()%words;l>0;l--){
			no_left=cur->left==NULL;
			no_right=cur->right==NULL;
			if(no_right && no_left)
				cur=cur->parent;
			else if(no_right)
				cur=cur->left;
			else if(no_left)
				cur=cur->right;
			else
				cur=rand()%2?cur->right:cur->left;
		}
		//write nonsense
		printf("Enter the number of words : ");
		scanf("%d",&l);
		file_pointer=fopen("nonsense.txt","w");
		for(;l>0;l--){
			fprintf(file_pointer,"%s ",cur->word);//write word
			//next word
			det=rand()%cur->sum;
			weights=cur->weights;
			len=cur->arr_length;
			for(i=0;i<len;i++){
				det-=weights[i];
				if(det<0)break;
			}
			cur=cur->neighbours[i];
		}
		fclose(file_pointer);
	}
	return 0;
}
