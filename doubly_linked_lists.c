#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h> 

#define MAX_THREADS 10000

typedef struct node
{
	int data;
	struct node* prev;
	struct node* next;
    pthread_mutex_t lock;
    	// takes care of the locking mechanism for nodes
}node;

node* head = NULL;
pthread_mutex_t head_lock;

// NOTE: all node are locked before accesing or editing its contents
//		 locks are released after editing.

node* getNewNode(int ele)
{
	// returns address of a new node.
	node* tmp = (node*)malloc(sizeof(node));
	tmp->data = ele;
	tmp->prev = NULL;
	tmp->next = NULL;
    pthread_mutex_init(&(tmp->lock), NULL);
	return tmp;
}

void* insertAtTail(void* x)
{
	int ele = *((int*)x);
	node* newNode = getNewNode(ele);
	
	// lock the head before accesing its value.
	pthread_mutex_lock(&head_lock);
	node* tmp = head;
	
	// CASE: Inserting root node
	if(tmp == NULL)
	{
		head = newNode;
		pthread_mutex_unlock(&head_lock);
		return NULL;
	}
	
	node* pre_tmp = NULL;
	// lock the first node, i.e, tmp.
	pthread_mutex_lock(&(tmp->lock));
	pthread_mutex_unlock(&head_lock);
	
	while(tmp->next)
	{
		pre_tmp = tmp;
		// lock the next node for usage.
		if(tmp->next) pthread_mutex_lock(&(tmp->next->lock));
		tmp = tmp->next;
		// unlock the previous node.
		pthread_mutex_unlock(&(pre_tmp->lock));
	}
	
	
	// update the new node and its previous node.
	tmp->next = newNode;
	newNode->prev = tmp;
	pthread_mutex_unlock(&(tmp->lock));
}

void* deleteNode(void* x)
{
	int ele = *((int*)x);
	
	// lock the head before accesing its value.
	pthread_mutex_lock(&head_lock);
	node* tmp = head;
	if(!tmp)
	{
		// element not present in the list.
		printf("\nDelete: %d not found : -1",ele);
		pthread_mutex_unlock(&head_lock);
		return NULL;
	}
	
	// lock the first node, i.e, tmp
	pthread_mutex_lock(&(tmp->lock));
	pthread_mutex_unlock(&head_lock);
	node* prev_tmp = NULL;
	
	while(tmp && tmp->data!=ele)
	{
		prev_tmp = tmp;
		// lock the next node for usage.
		if(tmp->next) pthread_mutex_lock(&((tmp->next)->lock));
		tmp = tmp->next;
		// unlock the previous node.
		pthread_mutex_unlock(&(prev_tmp->lock));
	}
	if(!tmp)
	{
		// element not present in the list.
		printf("\nDelete: %d not found : -1",ele);
		return NULL;
	}
	else
	{
		// tmp is a pointer the node to be delete
		// tmp node is already locked
		pthread_mutex_lock(&head_lock);
		
		// CASE: Deleting root element
		if(tmp == head)
		{
			// lock next node if present, update next node and 'head'
			if(tmp->next) pthread_mutex_lock(&(tmp->next->lock));
			head = tmp->next;
			if(head) head->prev = NULL;
			
			// unlock all locked nodes
			pthread_mutex_unlock(&(tmp->lock));
			if(head) pthread_mutex_unlock(&(head->lock));
			pthread_mutex_unlock(&head_lock);
			free(tmp);
		}
		// CASE: Deleting intermediate (or last) element
		else
		{
			// lock previous node and next node if present
			pthread_mutex_lock(&(tmp->prev->lock));
			if(tmp->next) pthread_mutex_lock(&(tmp->next->lock));
			pthread_mutex_unlock(&head_lock);
			
			// update the nodes
			(tmp->prev)->next = tmp->next;
			if(tmp->next) (tmp->next)->prev = tmp->prev;
			
			// unlock all locked nodes.
			pthread_mutex_unlock(&(tmp->lock));
			pthread_mutex_unlock(&(tmp->prev->lock));
			if(tmp->next) pthread_mutex_unlock(&(tmp->next->lock));
			free(tmp);		
		}
	}
}

void* searchNode(void* x)
{
	int ele = *((int*)x);
	
	// lock head before accesing its value
	pthread_mutex_lock(&head_lock);
	node* tmp = head;
	if(!tmp)
	{
		printf("\nSearch: %d not found: -1",ele);
		pthread_mutex_unlock(&head_lock);
		return NULL;
	}
	
	// lock the first node, i.e, tmp
	pthread_mutex_lock(&(tmp->lock));
	pthread_mutex_unlock(&head_lock);
	node* prev_tmp = NULL;
	int index = 0; 
		// stores position of search element
	while(tmp && tmp->data!=ele)
	{
		prev_tmp = tmp;
		if(tmp->next) pthread_mutex_lock(&((tmp->next)->lock));
		tmp = tmp->next;
		pthread_mutex_unlock(&(prev_tmp->lock));
		++index;
	}
	if(tmp) pthread_mutex_unlock(&(tmp->lock));
	if(!tmp) printf("\nSearch: %d not found: -1",ele);
	else printf("\nSearch: %d found at : %d",ele,index);	
}

void* print(void* args)
{
	// locking head before accesing its value.
	pthread_mutex_lock(&head_lock);
	node* tmp = head;
	if(!tmp)
	{
		printf("\nEmtpy list");
		pthread_mutex_unlock(&head_lock);
		return NULL;
	}
	
	// lock the first node, i.e, tmp
	pthread_mutex_lock(&(tmp->lock));
	pthread_mutex_unlock(&head_lock);
	node* prev_tmp = NULL;
	printf("\nList: ");
	while(tmp)
	{
		prev_tmp = tmp;
		printf("%d ",tmp->data);
		tmp = tmp->next;
		
		//if next node is present, lock the next node
		if(tmp) pthread_mutex_lock(&(tmp->lock));
		//unlock the previous node
		pthread_mutex_unlock(&(prev_tmp->lock));		
	}	
}

int main()
{
	pthread_mutex_init(&head_lock, NULL);
	pthread_t thread[MAX_THREADS];
	int values[MAX_THREADS];
	int count = 0;
	int choice = -1;
	
	printf("1.Insert   2.Delete  3.Search  4.Print\n");
	do
	{
		//printf("Enter your choice: ");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1: scanf("%d",&values[count]);
					pthread_create(&(thread[count]), NULL, insertAtTail, (void*)&values[count]);
					++count; 
					break;
			case 2: scanf("%d",&values[count]);
					pthread_create(&(thread[count]), NULL, deleteNode, (void*)&values[count]);
					++count; 
					break;
			case 3: scanf("%d",&values[count]);
					pthread_create(&(thread[count]), NULL, searchNode, (void*)&values[count]);
					++count; 
					break;
			case 4: pthread_create(&(thread[count++]), NULL, print, NULL); 
					break;
			default:choice = -1;
		}
	}
	while(choice != -1);
	while(count--)
	{
		pthread_join(thread[count],NULL);
	}
	printf("\n");
}
