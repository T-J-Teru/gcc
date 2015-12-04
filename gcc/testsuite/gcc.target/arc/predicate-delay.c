/* { dg-do compile } */
/* { dg-options "-O2 -fno-builtin" } */

/* This caused an ICE in arc_predicate_delay_insns.  */

typedef int (*initcall_t)(void);
struct dentry {
  struct inode *d_inode;
  struct {
    int d_alias;
  };
} trace_instance_dir, tracer_init_debugfs___trans_tmp_28;
struct inode {
  struct inode_operations *i_op;
  void *i_private;
} file_inode_f_0;
struct file {
  void *private_data;
};
struct file_operations {
  long llseek;
  int (*read)(int);
  int (*write)(void);
  unsigned poll;
  int (*open)(void);
  int (*release)(struct inode *, struct file *);
  int (*splice_read)(struct file *, int);
};
struct inode_operations {
  struct dentry *(*lookup)(struct inode *, struct dentry *, unsigned);
  int (*mkdir)(void);
  int (*rmdir)(void);
};
struct partial_page {
  int offset;
  int len;
  long private;
};
struct splice_pipe_desc {
  int **pages;
  struct partial_page *partial;
};
struct trace_iterator {
  int list;
  int trace_buffer;
  int buffer_disabled;
  int current_trace;
  struct dentry *options;
  struct dentry percpu_dir;
  int tracing_cpumask;
} trace_create_file(struct file_operations *), trace_options_init_dentry_tr,
  new_instance_create_tr, *instance_delete_tr;
char mask_str[4096 + 1];
int tracing_cpumask_read_len, tracing_cpumask_write_err,
    tracing_buffers_open_ret, tracing_buffers_splice_read_ret,
    tracing_buffers_splice_read_page, tracing_buffers_splice_read_entries,
    create_trace_options_dir_i, new_instance_create_ret, instance_mkdir_ret,
    instance_mkdir___trans_tmp_30, instance_rmdir_ret, instance_rmdir___mptr;
struct ftrace_buffer_info {
  struct trace_iterator iter;
};
int snprintf(char *, ...);
struct dentry *simple_lookup(struct inode *, struct dentry *, unsigned);
long ring_buffer_entries_cpu(void);
int wait_on_pipe(struct trace_iterator *);
int tracing_cpumask_read(int p3) {
  int *tr = (&file_inode_f_0)->i_private;
  tracing_cpumask_read_len = snprintf(mask_str, tr);
  return p3;
}

int tracing_cpumask_write(void) { return tracing_cpumask_write_err; }
struct file_operations tracing_cpumask_fops = { .read = tracing_cpumask_read,
                                                tracing_cpumask_write },
                       tracing_readme_fops, tracing_saved_cmdlines_fops,
                       tracing_saved_cmdlines_size_fops, rb_simple_fops;
int tracing_buffers_open(void) { return tracing_buffers_open_ret; }

int tracing_buffers_release(struct inode *, struct file *);
static int tracing_buffers_splice_read(struct file *p1, int p4) {
  struct ftrace_buffer_info *info = p1->private_data;
  struct trace_iterator iter = info->iter;
  struct partial_page partial_def[16];
  int *pages_def[16];
  struct splice_pipe_desc spd = { pages_def, partial_def };
  int i;
again:
  for (i = 0; p4 && tracing_buffers_splice_read_entries; i++, p4 = 3) {
    spd.pages[i] = &tracing_buffers_splice_read_page;
    spd.partial[i].offset = ring_buffer_entries_cpu();
  }
  tracing_buffers_splice_read_ret = wait_on_pipe(&iter);
  if (tracing_buffers_splice_read_ret)
    goto again;
  return tracing_buffers_splice_read_ret;
}
static struct file_operations tracing_buffers_fops = {
  .open = tracing_buffers_open,
  tracing_buffers_release,
  tracing_buffers_splice_read
};
void trace_create_cpu_file(struct file_operations *);
int instance_mkdir(void) {
  for (; new_instance_create_tr.list;) {
    goto out_unlock;
    goto out_free_tr;
  }
  trace_create_file(&rb_simple_fops);
  trace_create_cpu_file(&tracing_buffers_fops);
out_free_tr:
out_unlock:
  instance_mkdir___trans_tmp_30 = new_instance_create_ret;
  return instance_mkdir_ret;
}

int instance_rmdir(void) {
  for (instance_delete_tr =
           ({ (typeof(*instance_delete_tr) *)instance_rmdir___mptr; });
       ;)
    return instance_rmdir_ret;
}
struct inode_operations instance_dir_inode_operations = { simple_lookup,
                                                          instance_mkdir,
                                                          instance_rmdir };
int tracer_init_debugfs(void) {
  trace_create_file(&tracing_readme_fops);
  trace_create_file(&tracing_saved_cmdlines_fops);
  trace_create_file(&tracing_saved_cmdlines_size_fops);
  trace_instance_dir.d_inode->i_op = &instance_dir_inode_operations;
  tracer_init_debugfs___trans_tmp_28 = *trace_options_init_dentry_tr.options;
  for (create_trace_options_dir_i = 0;;)
    ;
}

initcall_t __initcall_tracer_init_debugfs5 = tracer_init_debugfs;

