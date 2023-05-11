#include "common.h"
#include "syscall.h"
#include "am.h"
extern ssize_t fs_write(int fd, const void* buf, size_t len);
int mm_brk(uint32_t new_brk);

static inline _RegSet* sys_none(_RegSet *r) {
  SYSCALL_ARG1(r) = 1;
  return NULL;
}

static inline _RegSet* sys_exit(_RegSet *r) {
  _halt(SYSCALL_ARG2(r));
  return NULL;
}

static inline _RegSet* sys_fwrite(_RegSet* r) {
  int fd = (int)SYSCALL_ARG2(r);
  void* buf = (void*)SYSCALL_ARG3(r);
  size_t len = (size_t)SYSCALL_ARG4(r);
  size_t res = fs_write(fd, buf, len);
  SYSCALL_ARG1(r) = res;
  return NULL;
}
_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  uintptr_t res = -1;
  a[0] = SYSCALL_ARG1(r);
  a[1] = SYSCALL_ARG2(r);
  a[2] = SYSCALL_ARG3(r);
  a[3] = SYSCALL_ARG4(r);

  switch (a[0]) {
    case SYS_none: 
        res = 1;
        break;
    case SYS_exit:
        _halt(a[1]);
        break;
    case SYS_brk:
      res = 0;
      break;
    case SYS_write: 
        return sys_fwrite(r);
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
  
  SYSCALL_ARG1(r) = res;

  return NULL;
}
