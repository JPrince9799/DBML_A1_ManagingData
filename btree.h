/*
 * 
 * You will need to write your B+Tree almost entirely from scratch. 
 * 
 * B+Trees are dynamically balanced tree structures that provides efficient support for insertion, deletion, equality, and range searches. 
 * The internal nodes of the tree direct the search and the leaf nodes hold the base data..
 * 
 * For a basic rundown on B+Trees, we will refer to parts of Chapter 10 of the textbook Ramikrishnan-Gehrke 
 * (all chapters and page numbers in this assignment prompt refer to the 3rd edition of the textbook).
 *
 * Read Chapter 10 which is on Tree Indexing in general. In particular, focus on Chapter 10.3 on B+Tree.
 */

#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "data_types.h"
#include "query.h"
#include "BP_Tree.h"
#define MAX_KEYS (1024)


/* 
Designing your C Structs for B+Tree nodes (Chapter 10.3.1)
How will you represent a B+Tree as a C Struct (or series of C structs that work together)? There are many valid ways to do this part of your design, and we leave it open to you to try and tune this as you progress through the project.
How will you account for a B+Tree node being an internal node or a leaf node? Will you have a single node type that can conditionally be either of the two types, or will you have two distinct struct types?
How many children does each internal node have? This is called the fanout of the B+Tree.
What is the maximum size for a leaf node? How about for an internal node?
What is the minimum threshold of content for a node, before it has to be part of a rebalancing?
*/

// TODO: here you will need to define a B+Tree node(s) struct(s)
typedef struct Node *bpTree;

struct Node{

    int isLeaf;                                                     //this is a variable to check if this is a leaf node
    
    int numKeys;                                                    //this is the variable to hold the number of keys in a node
    
    int keys[MAX_KEYS];
    
    struct Node *children[MAX_KEYS + 1];                            //the child nodes children[i]   

};

//Creating the B+ tree
bpTree bpCreate(void){

    bpTree b;
    
    b = malloc(sizeof(*b));
    
    assert(b);

    b -> isLeaf = 1;
    
    b -> numKeys = 0;

    return b;

}

//Method to delete the B+ tree
void bpDestroy(bpTree b){

    int i;

    if(!b -> isLeaf){

        for (i = 0; i < b -> numKeys + 1; i++){

            bpDestroy(b -> children[i]);

        }

    }

}

/* The following are methods that can be invoked on B+Tree node(s).
 * Hint: You may want to review different design patterns for passing structs into C functions.
 */




/* FIND (Chapter 10.4)
This is an equality search for an entry whose key matches the target key exactly.
How many nodes need to be accessed during an equality search for a key, within the B+Tree? 
*/
// TODO: here you will need to define FIND/SEARCH related method(s) of finding key-values in your B+Tree.

static int bpsearchKey(int m, const int *a, int key){

    int low = -1;
    
    int high = 0;

    int middle;

    while (low  + 1 < high){
        
        middle = (low + high)/2;

        if(a[middle] == key){

            return middle;    

        }
        else if(a[] < key){

            low = middle;

        }
        else{

           high = middle;

        }
        
    }
    return high;
}

//This method uses recursion to search the B+ tree for the 
int bpSearch(bpTree b, int key){

    int position;

    //when the node is empty
    if(!b -> numKeys == 0){

        return 0;

    }

    //look for the position  the searched key fits in
    position = bpsearchKey(b -> numKeys, b -> keys, key);

    if(position < b -> numKeys && b -> keys[position] == key){

        return 1;

    }
    else{

        return (!b -> isLeaf && bpSearch(b -> children, key));

    }
}


/* INSERT (Chapter 10.5)
How does inserting an entry into the tree differ from finding an entry in the tree?
When you insert a key-value pair into the tree, what happens if there is no space in the leaf node? What is the overflow handling algorithm?
For Splitting B+Tree Nodes (Chapter 10.8.3)
*/

static bpTree bpInternalInsert(bpTree b, int key, int *median){

    int position;

    int middle;

    bpTree bp;

    position = searchKey(b -> numKeys, b -> keys, key);

    if(position < b -> numKeys && b -> keys[position] == key){

        return 0;

    }
    //if the node is a leaf
    if(b -> isLeaf){
        
        //move all the keys above position by 1 space
        memmove(&b -> keys[position + 1], &b -> keys[position], sizeof(*(b -> keys)) * (b -> numKeys - position));

        b -> keys[position] = key;

        b -> numKeys++;

    }
    else{

        //insert into the child node
        bp = bpInternalInsert(&b -> children[position], key, &middle);
        //insert a new key if needed
        if(bp){
            
            //shift every key above position up a space
            memmove(&b -> keys[position + 1], &b -> keys[position], sizeof(*(b -> keys)) * (b -> numKeys - position));

            //the new child goes into the node position + 1
            memmove(&b -> keys[position + 2], &b -> keys[position + 1], sizeof(*(b -> keys)) * (b -> numKeys - position));

            b -> keys[position] = middle;

            b -> children[position + 1] = bp;

            b -> numKeys++;

        }

    }
    //instead of the next insertion
    //this algorithm is to help split the nodes when they are full
    //this may waste some space
    if(b -> numKeys >= MAX_KEYS){

        middle = b -> numKeys / 2;

        *median = b -> keys[middle];

        //splitting algorithm        
        bp = malloc(sizeof(*bp));

        bp -> numKeys = bp -> numKeys - middle - 1;

        bp -> isLeaf = b -> isLeaf;

        memmove(&bp ->keys, &b -> keys[middle + 1], sizeof(*(b -> keys)) * bp -> numKeys);

        if(!b -> isLeaf){

            memmove(bp -> children, &b -> children[middle + 1], sizeof(*(b -> children)) * (bp -> numKeys + 1));

        }
        b -> numKeys = middle;

        return bp;
        
    }
    else{

        return 0;

    }

}

void bpInsert(bpTree b, int key){

    bpTree b1;

    bpTree b2;

    int median;

    b2 = bpInternalInsert(b, key, &median);

    if(b2){

        b1 = malloc(sizeof(*b1));

        assert(b1);

        memmove(b1, b, sizeof(*b));

        b -> numKeys = 1;

        b -> isLeaf = 0;

        b -> keys[0] = median;

        b -> children[0] = b1;

        b -> children[0] = b2;
        
    }

}

void bpPrintKeys(bpTree t){

    

}

// TODO: here you will need to define INSERT related method(s) of adding key-values in your B+Tree.


/* BULK LOAD (Chapter 10.8.2)
Bulk Load is a special operation to build a B+Tree from scratch, from the bottom up, when beginning with an already known dataset.
Why might you use Bulk Load instead of a series of inserts for populating a B+Tree? Compare the cost of a Bulk Load of N data entries versus that of an insertion of N data entries? What are the tradeoffs?
*/

// TODO: here you will need to define BULK LOAD related method(s) of initially adding all at once some key-values to your B+Tree.
// BULK LOAD only can happen at the start of a workload


/*RANGE (GRADUATE CREDIT)
Scans are range searches for entries whose keys fall between a low key and high key.
Consider how many nodes need to be accessed during a range search for keys, within the B+Tree?
Can you describe two different methods to return the qualifying keys for a range search? 
(Hint: how does the algorithm of a range search compare to an equality search? What are their similarities, what is different?)
Can you describe a generic cost expression for Scan, measured in number of random accesses, with respect to the depth of the tree?
*/

// TODO GRADUATE: here you will need to define RANGE for finding qualifying keys and values that fall in a key range.



#endif