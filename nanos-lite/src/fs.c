#include "fs.h"
bool debug4fs = 1;
typedef struct {
  char *name;
  size_t size;
  off_t disk_offset;
  off_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_EVENTS, FD_DISPINFO, FD_NORMAL};

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin (note that this is not the actual stdin)", 0, 0},
  {"stdout (note that this is not the actual stdout)", 0, 0},
  {"stderr (note that this is not the actual stderr)", 0, 0},
  [FD_FB] = {"/dev/fb", 0, 0},
  [FD_EVENTS] = {"/dev/events", 0, 0},
  [FD_DISPINFO] = {"/proc/dispinfo", 128, 0},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))
extern void ramdisk_read(void *buf, off_t offset, size_t len);
extern void ramdisk_write(const void *buf, off_t offset, size_t len);
extern _Screen _screen;
void dispinfo_read(void *buf, off_t offset, size_t len);
void fb_write(const void *buf, off_t offset, size_t len);
extern size_t events_read(void *buf, size_t len);

void init_fs() {
  // TODO: initialize the size of /dev/fb
  if(debug4fs) printf("init_fs\n");
  file_table[FD_FB].size = _screen.height * _screen.width * 4;
}

size_t fs_filesz(int fd){
  if(debug4fs) printf("fs_filesz\n");
  return file_table[fd].size;
}

int fs_open(const char* pathname, int flags, int mode) {
  if(debug4fs) printf("fs_open\n");
  printf("NR_FILES = %d\n", NR_FILES);
  printf("pathname = %s\n", pathname);
  for(int i = 0; i < NR_FILES; i++) {
    printf("file_table[i].name = %s\n", file_table[i].name);
    printf("pathname = %s\n", pathname);
    if (strcmp(pathname, file_table[i].name) == 0){
      // TODO-ADD
      // file_table[i].open_offset = 0;//【pyt】
      return i;
    }
  }
  assert(0);
  return -1;
}

int fs_close(int fd) {
  if(debug4fs) printf("fs_close\n");
  return 0;
}

size_t fs_read(int fd, void* buf, size_t len) {
  if(debug4fs) printf("fs_read\n");
  // size? offset? <=> len
  // int offset = file_table[fd].disk_offset + file_table[fd].open_offset;
  // switch(fd){
  //   case FD_STDIN:
  //     return 0;
  //   case FD_STDOUT:
  //     return 0;
  //   case FD_STDERR:
  //     return 0;
  //   case FD_DISPINFO: {
  //     if(fs_filesz(fd) <= len + file_table[fd].open_offset) {
  //       len = fs_filesz(fd) - file_table[fd].open_offset;
  //     }
  //     dispinfo_read(buf, file_table[fd].open_offset, len);
  //     file_table[fd].open_offset += len;
  //     break;
  //   }
  //   case FD_EVENTS: {
  //     len = events_read(buf, len);
  //     break;
  //   }
  //   default: {
  //     if(fs_filesz(fd) <= len + file_table[fd].open_offset) {
  //       len = fs_filesz(fd) - file_table[fd].open_offset;
  //     }
  //     ramdisk_read(buf, offset, len);
  //     file_table[fd].open_offset += len;
  //     break;
  //   }
  // }
  // //Log("Read: file %s, open_off %d, disk_off %d, len %d.\n", file_table[fd].name, file_table[fd].open_offset, file_table[fd].disk_offset, len);
  // return len;
  //【pyt】
  ssize_t fs_size = fs_filesz(fd);
  switch(fd)
  {
    case FD_STDOUT:
    case FD_FB:
        break;
    case FD_EVENTS:
        len = events_read((void*)buf, len);
        break;
    case FD_DISPINFO:
        if(file_table[fd].open_offset >= file_table[fd].size)
            return 0;
        if(file_table[fd].open_offset + len > file_table[fd].size)
        {
            len = file_table[fd].size - file_table[fd].open_offset;
        }
        dispinfo_read(buf, file_table[fd].open_offset, len);
        file_table[fd].open_offset += len;
        break;
    default:
        if(file_table[fd].open_offset >= fs_size || len == 0)
        {
          return 0;
        }
        if(file_table[fd].open_offset + len > fs_size)
        {
          len = fs_size - file_table[fd].open_offset;
        }
        ramdisk_read(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
        file_table[fd].open_offset += len;
        break;

  }
  return len;
}

size_t fs_write(int fd, const void* buf, size_t len) {
  //Log("fwrite");
  // TODO-ADD
  // int offset = file_table[fd].open_offset + file_table[fd].disk_offset;
  // switch(fd) {
  //   case FD_STDIN:
  //     return 0;
  //   case FD_STDOUT: {
  //     for(int i = 0; i < len; i++) {
  //       _putc(((char *)buf)[i]);
  //     }
  //     break;
  //   }
  //   case FD_STDERR: {
  //     for(int i = 0; i < len; i++) {
  //       _putc(((char *)buf)[i]);
  //     }
  //     break;
  //   }
  //   case FD_FB: {
  //     if(fs_filesz(fd) <= len + file_table[fd].open_offset) {
  //       len = fs_filesz(fd) - file_table[fd].open_offset;
  //     }
  //     fb_write(buf, file_table[fd].open_offset, len);
  //     file_table[fd].open_offset += len;
  //     break;
  //   }
  //   default: {
  //     if(fs_filesz(fd) <= len + file_table[fd].open_offset) {
  //       len = fs_filesz(fd) - file_table[fd].open_offset;
  //     }
  //     ramdisk_write(buf, offset, len);
  //     file_table[fd].open_offset += len;
  //   }
  // }
  
  // //Log("Write: file %s, open_off %d, disk_off %d, len %d.\n", file_table[fd].name, file_table[fd].open_offset, file_table[fd].disk_offset, len);
  // return len;//【pyt】
  ssize_t fs_size = fs_filesz(fd);
  switch(fd)
  {
    case FD_STDOUT:
    case FD_STDERR:
        for(int i = 0; i < len; i ++)
        {
          _putc(((char*)buf)[i]);
        }
        break;
    case FD_FB:
        fb_write(buf, file_table[fd].open_offset, len);
        file_table[fd].open_offset += len;
        break;
    default:
        if(file_table[fd].open_offset >= fs_size)return 0;
        if(file_table[fd].open_offset + len > fs_size)
        {
          len = fs_size - file_table[fd].open_offset;
        }
        ramdisk_write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
        file_table[fd].open_offset += len;
        break;
  }
  return len;
}