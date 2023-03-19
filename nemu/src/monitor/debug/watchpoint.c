#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(){
	if(free_ ==NULL){
		printf("No free space.\n");
		assert(0);
	}
	if(head == NULL){
		head = free_;
		free_ = free_ -> next;
		head->next = NULL;
		head->used = true;
		return head;
	}
	else{
		WP* current = head;
		while(current->next != NULL){
			current = current->next;
		}
		current->next = free_;
		free_ = free_->next;
		current->next->next = NULL;
		current->next->used = true;
		return current->next;
	}
}

void free_wp(int no){
  WP* curr_head = head;
  WP* curr_free = free_;
  if (0 > no || no >= 32) {
    printf("Error: %d is not an acceptable watchpoint.\n", no);
  }
  if (curr_head == NULL || (curr_head->next == NULL && curr_head->NO != no)) {
    printf("Error: try to free a nonexistent watchpoint.\n");
    assert(0);
  }
  if (curr_head->NO == no) {
    printf("%d\n", no);
    while(curr_free->next != NULL) {
      curr_free = curr_free->next;
    }
    curr_free->next = curr_head;
    if (curr_head->next != NULL) {
      head = curr_head->next;
    }
    else {
      head = NULL;
    }
    curr_free->next->used = false;
    curr_free->next->expr = NULL;
    curr_free->next->next = NULL;
    
    return;
  }
  if (curr_free == NULL) {
    while(curr_head != NULL && curr_head->next->NO != no) {
      curr_head = curr_head->next;
    }
    if (curr_head == NULL) {
      printf("Error: watchpoint %d is not found to be used.\n", no);
    }
    curr_head->next->used = false;
    curr_head->next->expr = NULL;
    curr_free = curr_head->next;
    curr_head->next = curr_head->next->next;
    curr_free->next = NULL;
  }
  else {
    while(curr_head != NULL && curr_head->next->NO != no) {
      curr_head = curr_head->next;
    }
    while(curr_free->next != NULL) {
      curr_free = curr_free->next;
    }
    if (curr_head == NULL) {
      printf("Error: watchpoint %d is not found to be used.\n", no);
    }
    curr_head->next->used = false;
    curr_head->next->expr = NULL;
    curr_free->next = curr_head->next;
    curr_head->next = curr_head->next->next;
    curr_free->next->next = NULL;
  }

}
