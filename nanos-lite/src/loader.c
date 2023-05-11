#include "common.h"

#define DEFAULT_ENTRY ((void *)0x4000000)
void _map(_Protect *p, void *va, void *pa);
void* new_page(void);

void ramdisk_read(void* buf, off_t offset, size_t len);
void ramdisk_write(const void* buf, off_t offset, size_t len);
size_t get_ramdisk_size();

int fs_open(const char *pathname, int flags, int mode);
size_t fs_filesz(int fd);
size_t fs_read(int fd, void *buf, size_t len);
int fs_close(int fd);



uintptr_t loader(_Protect *as, const char *filename) {
  // ramdisk_read(DEFAULT_ENTRY, 0, get_ramdisk_size());
  int fd = fs_open(filename, 0, 0);
  printf("fd to open = %d\n", fd);
  fs_read(fd, DEFAULT_ENTRY, fs_filesz(fd));
  fs_close(fd);
  return (uintptr_t)DEFAULT_ENTRY;
}
