/*
** mrb_fuse.c
**
** Copyright (c) 2016 Russel Hunter Yukawa
**
** See Copyright Notice in LICENSE
*/

#define _GNU_SOURCE 1
#define FUSE_USE_VERSION 26

#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fuse/fuse.h>

#include "mruby.h"
#include "mruby/data.h"
#include "mruby/error.h"
#include "mrb_fuse.h"

#define DONE mrb_gc_arena_restore(mrb, 0);


typedef struct {
} mrb_fuse_data;

static const struct mrb_data_type mrb_fuse_data_type = {
  "mrb_fuse_data", mrb_free,
};

static mrb_value mrb_fuse_init(mrb_state *mrb, mrb_value self)
{
  const char *path = NULL;
  int path_len = 0;

  int r;
  mrb_fuse_data *data;

  data = (mrb_fuse_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_fuse_data_type;
  DATA_PTR(self) = NULL;

  const char *option = NULL;
  int option_len = 0;

  mrb_get_args(mrb, "z|z", &path, &option);

  /* path_len = strlen(path); */
  option_len = strlen(option);

#if 1

  struct fuse_operations op;
  memset(&op, 0, sizeof(struct fuse_operations));

  int argc = 2;
  if (option && option_len > 0) {
    argc += 2;
  }

  char **argv = malloc(sizeof(char*)*argc);

  char *p = strdup(path);
  char *q = strdup(option);

  int i = 0;
  *(argv + i++) = "mruby-fuse";
  if (option && option_len > 0) {
    *(argv + i++) = "-o";
    *(argv + i++) = q;
  }
  *(argv + i++) = p;

  r = fuse_main(argc, argv, &op, NULL);

  printf("r=%d\n", r);

#endif

  return self;
}

void mrb_mruby_fuse_gem_init(mrb_state *mrb)
{
  struct RClass *fuse;
  fuse = mrb_define_class(mrb, "Fuse", mrb->object_class);

  mrb_define_method(mrb, fuse, "initialize", mrb_fuse_init, MRB_ARGS_REQ(1)|MRB_ARGS_OPT(1));

  DONE;
}

void mrb_mruby_fuse_gem_final(mrb_state *mrb)
{
}
