#include "common.h"

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};
int current_game = 0;
size_t events_read(void *buf, size_t len) {
  return 0;
  // int key = _read_key();
  // if (key == _KEY_NONE) {
  //   sprintf(buf, "t %d\n", _uptime());
  // }
  // else {
  //   bool keydown = false;
  //   if (key & 0x8000) {
  //     key ^= 0x8000;
  //     keydown = true; 
  //   }
  //   sprintf(buf, "%s %s\n", keydown ? "kd":"ku", keyname[key]);
  //   if (key == 13 && keydown) {
  //     current_game = (current_game == 0 ? 1 : 0);
  //   }
  // }
  // if (strlen(buf) > len) {
  //   char *tmp;
  //   for(int i = 0; i < len; i++) {
  //     tmp[i] = ((char*)buf)[i];
  //   }
  //   buf = (void*)tmp;
  //   return len;
  // }
  // return strlen(buf);
}

static char dispinfo[128] __attribute__((used));

void dispinfo_read(void *buf, off_t offset, size_t len) {
  // memcpy(buf, dispinfo+offset, len);
}

void fb_write(const void *buf, off_t offset, size_t len) {
}

void init_device() {
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
