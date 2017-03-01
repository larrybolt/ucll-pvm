#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct linked_list
{
    int value;
    linked_list* next;
};

unsigned length(linked_list*);
bool has_cycle(linked_list*);

#endif
