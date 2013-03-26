

/*
Copyright 2003 by Steven S. Skiena; all rights reserved. 

Permission is granted for use in non-commerical applications
provided this copyright notice remains intact and unchanged.

This program appears in my book:

"Programming Challenges: The Programming Contest Training Manual"
by Steven Skiena and Miguel Revilla, Springer-Verlag, New York 2003.

See our website www.programming-challenges.com for additional information.

This book can be ordered from Amazon.com at

http://www.amazon.com/exec/obidos/ASIN/0387001638/thealgorithmrepo/

*/

void init_queue(Queue *q)
{
        q->first = 0;
        q->last = QUEUE_SIZE-1;
        q->count = 0;
}

void enqueue(Queue *q, int x)
{
        if (q->count >= QUEUE_SIZE) {
		printf("Warning: queue overflow enqueue x=%d\n",x); // TODO - handle doubling
                exit( EXIT_FAILURE );
                
        } else {
                q->last = (q->last+1) % QUEUE_SIZE;
                q->q[ q->last ] = x;    
                q->count = q->count + 1;
        }
}

int dequeue(Queue *q)
{
        int x;

        if (q->count <= 0) printf("Warning: empty queue dequeue.\n");
        else {
                x = q->q[ q->first ];
                q->first = (q->first+1) % QUEUE_SIZE;
                q->count = q->count - 1;
        }

        return(x);
}

int peek(Queue *q )
{
        int x;

        if (q->count <= 0) printf("Warning: empty queue peek.\n");
        else {
                x = q->q[ q->first ];
        }

        return(x);
}

int empty(Queue *q)
{
        if (q->count <= 0) return (TRUE);
        else return (FALSE);
}

void print_queue(Queue *q)
{
        int i,j;

        i=q->first; 
        
        while (i != q->last) {
                printf("%c ",q->q[i]);
                i = (i+1) % QUEUE_SIZE;
        }

        printf("%2d ",q->q[i]);
        printf("\n");
}
