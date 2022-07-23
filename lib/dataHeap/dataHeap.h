#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

/**
 * @brief contains data about robots whereabouts on change to movement
*/
struct DrawInstruction
{
    uint16_t ins; // instruction
    int16_t x0, x1, x2; // x cooridnate sets
    int16_t y0, y1, y2; // y cooridnate sets
    bool color;
};

/**
 * @brief node inside of the heap
*/
struct Heap_node_s
{
    struct Heap_node_s* next;
    DrawInstruction data;
    struct Heap_node_s* prev;
};

/**
 * @brief node heap struct contains all data from heap
*/
struct Node_heap_s
{
    Heap_node_s* first;
    Heap_node_s* last;
};

/**
 * @brief push position data onto heap
 * 
 * @param heap pointer to heap
 * 
 * @param data position data to push onto heap
*/
void push(Node_heap_s* heap, DrawInstruction data)
{
    if(heap->first == NULL)
    {
        heap->first = (Heap_node_s*)malloc(sizeof(Heap_node_s));
        heap->first->next = NULL;
        heap->first->prev = NULL;
        heap->first->data = data;
        heap->last = heap->first;
        return;
    }

    Heap_node_s* newNode = (Heap_node_s*)malloc(sizeof(Heap_node_s));
    Heap_node_s* currentNode = heap->first;
    newNode->next = NULL;
    newNode->prev = currentNode;
    currentNode->next = newNode;
    newNode->data = data;

    heap->first = newNode;
}

/**
 * @brief pop position data from top of heap
 * 
 * @param heap pointer to heap
*/
DrawInstruction pop(Node_heap_s* heap)
{
    DrawInstruction data = {0, 0, 0, 0, 0, 0, 0};
    if(heap->first == NULL)
        return data;
    data = heap->first->data;

    if(heap->first == heap->last)
    {
        free(heap->first);
        heap->first = NULL;
        heap->last = NULL;
        return data;
    }

    heap->first = heap->first->prev;
    free(heap->first->next);
    heap->first->next = NULL;
    return data;
}

/**
 * @brief frees the heap
 * 
 * @param heap pointer to heap
*/
void free_heap(Node_heap_s* heap)
{
    Heap_node_s* current = heap->first;
    if(heap->first != NULL)
    {
        heap->first = NULL;
        heap->last = NULL;

        while(current->prev != NULL)
        {
            Heap_node_s* plch = current;
            current->next = NULL;
            current = current->prev;
            plch->prev = NULL;
            free(plch);
        }
    }
}

unsigned char is_empty(Node_heap_s* heap)
{
    return heap->first == NULL;
}