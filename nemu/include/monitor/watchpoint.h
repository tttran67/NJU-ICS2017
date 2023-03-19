#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  bool used;
  int value;
  char* expr;

} WP;
WP* new_wp();
void free_wp(int);
#endif
