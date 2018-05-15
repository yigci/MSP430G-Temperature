int Q_LENGTH;					// variable to determine queue length.
void enq(int,int);				// function def.
int qcount=0;					// counter for queue.
int denominator;				// denominator is used to make the division operation.

struct node						// structure of queue.
{
    float info;					// info.
    struct node *ptr;			// pointer to hold next node.
}*temp,*first,*last;			// necessary node pointers.

void enq(int data,int MAX)
{
    if (qcount==0)				// if qcount(queue counter) is zero first node is created.
    {
        first = (struct node *)malloc(1*sizeof(struct node));//create a new node.
        first->ptr = NULL;		// pointer of the new node is NULL.
        first->info = data;		// node holds data as its info.
        last = first;			// first and last nodes are same as there is only one node.
        denominator=1;			// set denominator to 1. Denominator is going to be used...
        						//...to obtain average.
    }
    else
    {
    	if(qcount<MAX){			// check if queue reached maximum number of elements.
        	temp=(struct node *)malloc(1*sizeof(struct node));// if not create new node.
        	temp->info = data;	// new node holds new data input...
       		temp->ptr = NULL;	//...and it addresses NULL.
       		last->ptr = temp;	// newly added node is named as last.
        	last = last->ptr;
        	denominator++;		// increase denominator by 1.
    	}
    }
    if(qcount==MAX){			// while we have 10 nodes so far we don't need any new nodes.
    	temp=first;				// temp hold address of first.
    	first=first->ptr;		// first node is now address the next node.
		temp->info = data;		// temp is now our new node to hold data.
        temp->ptr = NULL;
		last->ptr = temp;		// new node now added to end of the queue.
        last = last->ptr;		// last node is updated.
		denominator=MAX;		// set denominator to maximum value possible.
		qcount=9;				// to keep queue running properly we set qcount to 9...
								//...and increase it in the next line.
	}
	qcount++;					// increment of qcount by 1.
}
