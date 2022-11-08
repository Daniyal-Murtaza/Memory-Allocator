#include <stdio.h>
#include <stdlib.h>

struct dll_node
{
    struct dll_node *pfwd;
    struct dll_node *pbck;
    int size;
};

typedef struct dll_node dll_node;
// doubly linkedlist
struct dll
{
    struct dll_node *head;
    struct dll_node *tail;
    int available_memory;
};
typedef struct dll dll;

// function declarations

dll_node *get_new_node(int size);
void add_to_tail(dll *list, dll_node *new, int is_freelist);
dll_node *remove_from_head(dll *list);
void print_list(const dll *list);
int is_empty(const dll *list);

dll_node *get_new_node(int size)
{
    if (size == 0)
        return NULL;
    dll_node *new = malloc(sizeof(dll_node));
    if (!new)
    {
        fprintf(stderr, "Error: allocating memory");
        exit(1);
    }
    new->pfwd = new->pbck = NULL;
    new->size = size;
    return new;
}

void print_list(const dll *list)
{
    dll_node *n = list->head;
    if (n == NULL)
        return;
    while (n)
    {
        printf("( %d ),", n->size);
        n = n->pbck;
    }
}

int is_empty(const dll *list)
{
    if (list->head == NULL && list->tail == NULL)
        return 1;
    else if (list->head != NULL && list->tail != NULL)
        return 0;
    else
    {
        fprintf(stderr, "Error: List is inconsistent.");
        exit(1);
    }
}

void add_to_tail(dll *list, dll_node *new, int is_freelist)
{
    // adding the node to the tail of the list
    // if the list-> head and list->tail is NULL
    if (list->head == NULL && list->tail == NULL)
    {
        // setting the head and tail node pointers to the new node
        list->head = new;
        list->tail = new;
    }
    else
    {
        // at least 1 element or more than 1 element is there in the list
        // setting the next pointer of list->tail to be new pointer
        list->tail->pbck = new;
        // setting the current list->tail to be the prev pointer of new
        new->pfwd = list->tail;
        // setting the list->tail pointer to be the new pointer
        list->tail = new;
    }
    // if the list is a free list, just add the memory
    if (is_freelist == 1)
    {
        list->available_memory += new->size;
    }
    else
    {
        // subtract the memory along with header allocation memory from the list
        list->available_memory -= (new->size + 8);
    }
}

/**
 * This function removes the head node from the list
 * @param list - pointer representing the doubly linked list of nodes
 * @return - the node at the head of the list
 */
dll_node *remove_from_head(dll *const list)
{
    // if the list is not empty
    if (is_empty(list) != 1)
    {
        // getting the head node
        dll_node *tempNode = list->head;
        // setting the head node to be next node in the list
        list->head = tempNode->pbck;
        // updating the memory, by adding 8 more bytes as header allocation bytes
        list->available_memory += (tempNode->size + 8);
        // setting the pbck and pfwd pointers of the tempNode
        tempNode->pbck = tempNode->pfwd = NULL;
        // returning the tempnode
        return tempNode;
    }
    else
    {
        return NULL;
    }
}

int main()
{
    dll list, freelist;
    int available = 0;
    int num_allocs = 0;
    int num_deallocs = 0;

    list.head = NULL;
    list.tail = NULL;

    freelist.head = NULL;
    freelist.tail = NULL;

    freelist.available_memory = 0;
    scanf("%d", &available);
    list.available_memory = available - 8;

    scanf("%d", &num_allocs);
    for (int i = 0; i < num_allocs; i += 1)
    {
        int alloc = 0;
        scanf("%d", &alloc);
        add_to_tail(&list, get_new_node(alloc), 0);
    }

    scanf("%d", &num_deallocs);

    for (int i = 0; i < num_deallocs; i += 1)
    {
        add_to_tail(&freelist, remove_from_head(&list), 1);
    }

    puts("Allocator:");
    print_list(&list);
    printf("Available mem: %d", list.available_memory);
    puts("Freelist:");
    print_list(&freelist);
    printf("Available mem (freelist): %d", freelist.available_memory);

    return 0;
}