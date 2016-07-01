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

# if 0
#include <fuse/fuse.h>
# endif

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include <syslog.h>

#include "mruby.h"
#include "mruby/data.h"
#include "mruby/error.h"
#include "mrb_fuse.h"
#include "mruby/variable.h"
#include "mruby/string.h"
#include "mruby/array.h"
#include "mruby/hash.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";

typedef struct {
} mrb_fuse_data;

static const struct mrb_data_type mrb_fuse_data_type = {
  "mrb_fuse_data", mrb_free,
};

struct fuse_operations fuse_mount_op;
int fuse_mount_argc;
char **fuse_mount_argv3 ;
mrb_state *gMrb;
mrb_value gSelf;

static int internal_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi
			 )
{
	(void) offset;
	(void) fi;

	/* if (strcmp(path, "/") != 0)
		return -ENOENT;
        */

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	filler(buf, hello_path + 1, NULL, 0);

	return 0;
}

static int internal_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;

# if 0
    syslog(LOG_DAEMON,"func=%s,path=%s","internal_getattr",path); 
# endif

mrb_state *mrb = &gMrb;
mrb_value self = gSelf;

    mrb_value entries = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@entries"));

    memset(stbuf, 0, sizeof(struct stat));
    if(strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return res;
    }

    if (mrb_hash_p(entries)) {
      mrb_value keys = mrb_hash_keys(mrb, entries);
      int klen = RARRAY_LEN(keys);

      for (int ki = 0; ki < klen; ++ki) {
        mrb_value rkey = mrb_ary_ref(mrb, keys, ki);
        mrb_value rval = mrb_hash_get(mrb, entries, rkey);
        char *ent_path = mrb_str_to_cstr(mrb, rkey);
      }
    }


    if(strcmp(path, hello_path) == 0) {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(hello_str);
    }
    else
        res = -ENOENT;

    return res;
}

static mrb_value mrb_fuse_mount(mrb_state *mrb, mrb_value self)
{

  mrb_value mountpoint;


  mountpoint = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@mountpoint"));

  /* *gMrb = *mrb; */
  /* gSelf = self; */

  printf("mountpoint=%s\n",mrb_str_to_cstr(mrb, mountpoint));

  memset(&fuse_mount_op, 0, sizeof(struct fuse_operations));
  fuse_mount_op.readdir = internal_readdir;
  fuse_mount_op.getattr = internal_getattr;

  

  int argc = 2;
  char  **argv = malloc(sizeof(char*)*argc);

  char *p = strdup(mrb_str_to_cstr(mrb, mountpoint));

  int i = 0;


  *(argv + i++) = "mruby-fuse";
  *(argv + i++) = p;

  fuse_main(argc, argv, &fuse_mount_op, NULL);
  return self;
}

# if 0
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

printf("is %d\n",__LINE__);
  /* path_len = strlen(path); */
  option_len = strlen(option);

#if 1

printf("is %d\n",__LINE__);
  memset(&fuse_mount_op, 0, sizeof(struct fuse_operations));

printf("is %d\n",__LINE__);

  fuse_mount_argc = 2;
  if (option && option_len > 0) {
    fuse_mount_argc += 2;
  }

printf("is %d\n",__LINE__);
char  **fuse_mount_argv = malloc(sizeof(char*)*fuse_mount_argc - 1);

printf("is %d\n",__LINE__);
  char *p = strdup(path);
  char *q = strdup(option);

printf("is %d\n",__LINE__);
  int i = 0;
  *(fuse_mount_argv + i++) = "mruby-fuse";
  if (option && option_len > 0) {
    *(fuse_mount_argv + i++) = "-o";
    *(fuse_mount_argv + i++) = q;
  }

printf("is %d\n",__LINE__);
  *(fuse_mount_argv + i++) = p;

printf("is %d\n",__LINE__);
fuse_mount_argv3 = fuse_mount_argv;
printf("is %d\n",__LINE__);

#endif

  return self;
}

# endif

void mrb_mruby_fuse_gem_init(mrb_state *mrb)
{
  struct RClass *fuse;
  fuse = mrb_define_class(mrb, "Fuse", mrb->object_class);

/*  mrb_define_method(mrb, fuse, "initialize", mrb_fuse_init, MRB_ARGS_REQ(1)|MRB_ARGS_OPT(1)); */
  mrb_define_method(mrb, fuse, "mount", mrb_fuse_mount, MRB_ARGS_NONE());

  DONE;
}

void mrb_mruby_fuse_gem_final(mrb_state *mrb)
{
}
