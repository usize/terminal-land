#include "logging.h"

static FILE *__logfile = NULL;

void OPEN_LOG(const char* path) {
  if (__logfile == NULL) {
    __logfile= fopen(path, "w+");
  }
}

void LOG(const char* s) {
  if (__logfile) fputs(s, __logfile);
}

void CLOSE_LOG() {
  if (__logfile) fclose(__logfile);
}
