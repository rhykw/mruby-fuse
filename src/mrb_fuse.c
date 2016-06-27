/*
** mrb_fuse.c
**
** Copyright (c) 2016 Russel Hunter Yukawa
**
** See Copyright Notice in LICENSE
*/

#define _GNU_SOURCE 1

#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

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
  mrb_fuse_data *data;

  data = (mrb_fuse_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_fuse_data_type;
  DATA_PTR(self) = NULL;

  return self;
}

void mrb_mruby_fuse_gem_init(mrb_state *mrb)
{
  struct RClass *fuse;
  fuse = mrb_define_class(mrb, "Fuse", mrb->object_class);

  mrb_define_method(mrb, fuse, "initialize", mrb_fuse_init, MRB_ARGS_NONE());

  DONE;
}

void mrb_mruby_fuse_gem_final(mrb_state *mrb)
{
}
