/**
** @file mruby/internal.h - Functions only called from within the library
**
** See Copyright Notice in mruby.h
*/

#ifndef MRUBY_INTERNAL_H
#define MRUBY_INTERNAL_H

#ifdef MRUBY_ARRAY_H
void mrb_ary_decref(mrb_state*, mrb_shared_array*);
mrb_value mrb_ary_subseq(mrb_state *mrb, mrb_value ary, mrb_int beg, mrb_int len);
#endif

#ifdef MRUBY_CLASS_H
struct RClass *mrb_vm_define_class(mrb_state*, mrb_value, mrb_value, mrb_sym);
struct RClass *mrb_vm_define_module(mrb_state*, mrb_value, mrb_sym);
mrb_value mrb_instance_new(mrb_state *mrb, mrb_value cv);
void mrb_class_name_class(mrb_state*, struct RClass*, struct RClass*, mrb_sym);
mrb_bool mrb_const_name_p(mrb_state*, const char*, mrb_int);
mrb_value mrb_class_find_path(mrb_state*, struct RClass*);
mrb_value mrb_mod_to_s(mrb_state *, mrb_value);
void mrb_method_added(mrb_state *mrb, struct RClass *c, mrb_sym mid);
mrb_noreturn void mrb_method_missing(mrb_state *mrb, mrb_sym name, mrb_value self, mrb_value args);
mrb_method_t mrb_vm_find_method(mrb_state *mrb, struct RClass *c, struct RClass **cp, mrb_sym mid);
mrb_value mrb_mod_const_missing(mrb_state *mrb, mrb_value mod);
mrb_value mrb_const_missing(mrb_state *mrb, mrb_value mod, mrb_sym sym);
size_t mrb_class_mt_memsize(mrb_state*, struct RClass*);
mrb_value mrb_obj_extend(mrb_state*, mrb_value obj);
#endif

mrb_value mrb_obj_equal_m(mrb_state *mrb, mrb_value);

/* debug */
size_t mrb_packed_int_len(uint32_t num);
size_t mrb_packed_int_encode(uint32_t num, uint8_t *p);
uint32_t mrb_packed_int_decode(const uint8_t *p, const uint8_t **newpos);

/* dump */
#ifdef MRUBY_IREP_H
int mrb_dump_irep(mrb_state *mrb, const mrb_irep *irep, uint8_t flags, uint8_t **bin, size_t *bin_size);
#ifndef MRB_NO_STDIO
int mrb_dump_irep_cfunc(mrb_state *mrb, const mrb_irep*, uint8_t flags, FILE *f, const char *initname);
int mrb_dump_irep_cstruct(mrb_state *mrb, const mrb_irep*, uint8_t flags, FILE *f, const char *initname);
#endif
#endif

/* codedump */
void mrb_codedump_all(mrb_state *mrb, struct RProc *proc);
#ifndef MRB_NO_STDIO
void mrb_codedump_all_file(mrb_state *mrb, struct RProc *proc, FILE *out);
#endif

/* error */
mrb_value mrb_exc_inspect(mrb_state *mrb, mrb_value exc);
mrb_value mrb_exc_backtrace(mrb_state *mrb, mrb_value exc);
mrb_value mrb_get_backtrace(mrb_state *mrb);
void mrb_exc_mesg_set(mrb_state *mrb, struct RException *exc, mrb_value mesg);
mrb_value mrb_exc_mesg_get(mrb_state *mrb, struct RException *exc);
mrb_value mrb_f_raise(mrb_state*, mrb_value);
mrb_value mrb_make_exception(mrb_state *mrb, mrb_value exc, mrb_value mesg);
mrb_value mrb_exc_get_output(mrb_state *mrb, struct RObject *exc);

struct RBacktrace {
  MRB_OBJECT_HEADER;
  size_t len;
  struct mrb_backtrace_location *locations;
};

struct mrb_backtrace_location {
  mrb_sym method_id;
  int32_t idx;
  const mrb_irep *irep;
};

/* gc */
size_t mrb_gc_mark_mt(mrb_state*, struct RClass*);
void mrb_gc_free_mt(mrb_state*, struct RClass*);

/* hash */
size_t mrb_hash_memsize(mrb_value obj);
size_t mrb_gc_mark_hash(mrb_state*, struct RHash*);
void mrb_gc_free_hash(mrb_state*, struct RHash*);
mrb_value mrb_hash_first_key(mrb_state*, mrb_value);
uint32_t mrb_obj_hash_code(mrb_state *mrb, mrb_value key);

/* irep */
struct mrb_insn_data mrb_decode_insn(const mrb_code *pc);
#ifdef MRUBY_IREP_H
void mrb_irep_free(mrb_state*, struct mrb_irep*);

static inline const struct mrb_irep_catch_handler *
mrb_irep_catch_handler_table(const struct mrb_irep *irep)
{
  if (irep->clen > 0) {
    return (const struct mrb_irep_catch_handler*)(irep->iseq + irep->ilen);
  }
  else {
    return (const struct mrb_irep_catch_handler*)NULL;
  }
}
#endif

/* numeric */
mrb_value mrb_div_int_value(mrb_state *mrb, mrb_int x, mrb_int y);
mrb_int mrb_div_int(mrb_int x, mrb_int y);
mrb_value mrb_int_add(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_int_sub(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_int_mul(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_noreturn void mrb_int_zerodiv(mrb_state *mrb);
mrb_noreturn void mrb_int_overflow(mrb_state *mrb, const char *reason);
#ifndef MRB_NO_FLOAT
void mrb_check_num_exact(mrb_state *mrb, mrb_float num);
#endif

#ifdef MRB_USE_COMPLEX
mrb_value mrb_complex_new(mrb_state *mrb, mrb_float x, mrb_float y);
mrb_value mrb_complex_add(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_complex_sub(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_complex_mul(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_complex_div(mrb_state *mrb, mrb_value x, mrb_value y);
void mrb_complex_copy(mrb_state *mrb, mrb_value x, mrb_value y);
#endif
#ifdef MRB_USE_RATIONAL
mrb_value mrb_rational_new(mrb_state *mrb, mrb_int x, mrb_int y);
mrb_value mrb_rational_add(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_rational_sub(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_rational_mul(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_rational_div(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_as_rational(mrb_state *mrb, mrb_value x);
void mrb_rational_copy(mrb_state *mrb, mrb_value x, mrb_value y);
int mrb_rational_mark(mrb_state *mrb, struct RBasic *rat);
#endif
#ifdef MRB_USE_SET
size_t mrb_gc_mark_set(mrb_state *mrb, struct RBasic *set);
void mrb_gc_free_set(mrb_state *mrb, struct RBasic *set);
size_t mrb_set_memsize(mrb_value);
#endif

#ifdef MRUBY_PROC_H
struct RProc *mrb_closure_new(mrb_state*, const mrb_irep*);
void mrb_proc_copy(mrb_state *mrb, struct RProc *a, const struct RProc *b);
mrb_int mrb_proc_arity(const struct RProc *p);
struct REnv *mrb_env_new(mrb_state *mrb, struct mrb_context *c, mrb_callinfo *ci, int nstacks, mrb_value *stack, struct RClass *tc);
void mrb_proc_merge_lvar(mrb_state *mrb, mrb_irep *irep, struct REnv *env, int num, const mrb_sym *lv, const mrb_value *stack);
mrb_value mrb_proc_local_variables(mrb_state *mrb, const struct RProc *proc);
const struct RProc *mrb_proc_get_caller(mrb_state *mrb, struct REnv **env);
mrb_value mrb_proc_get_self(mrb_state *mrb, const struct RProc *p, struct RClass **target_class_p);
mrb_bool mrb_proc_eql(mrb_state *mrb, mrb_value self, mrb_value other);
#endif

/* range */
#ifdef MRUBY_RANGE_H
mrb_value mrb_get_values_at(mrb_state *mrb, mrb_value obj, mrb_int olen, mrb_int argc, const mrb_value *argv, mrb_value (*func)(mrb_state*, mrb_value, mrb_int));
size_t mrb_gc_mark_range(mrb_state *mrb, struct RRange *r);
#endif

/* string */
void mrb_gc_free_str(mrb_state*, struct RString*);
uint32_t mrb_str_hash(mrb_state *mrb, mrb_value str);
mrb_value mrb_str_dump(mrb_state *mrb, mrb_value str);
mrb_value mrb_str_inspect(mrb_state *mrb, mrb_value str);
mrb_bool mrb_str_beg_len(mrb_int str_len, mrb_int *begp, mrb_int *lenp);
mrb_value mrb_str_byte_subseq(mrb_state *mrb, mrb_value str, mrb_int beg, mrb_int len);
mrb_value mrb_str_aref(mrb_state *mrb, mrb_value str, mrb_value idx, mrb_value len);
uint32_t mrb_byte_hash(const uint8_t*, mrb_int);
uint32_t mrb_byte_hash_step(const uint8_t*, mrb_int, uint32_t);

#ifdef MRB_UTF8_STRING
mrb_int mrb_utf8len(const char *str, const char *end);
mrb_int mrb_utf8_strlen(const char *str, mrb_int byte_len);
#endif

/* variable */
mrb_value mrb_vm_special_get(mrb_state*, mrb_sym);
void mrb_vm_special_set(mrb_state*, mrb_sym, mrb_value);
mrb_value mrb_vm_cv_get(mrb_state*, mrb_sym);
void mrb_vm_cv_set(mrb_state*, mrb_sym, mrb_value);
mrb_value mrb_vm_const_get(mrb_state*, mrb_sym);
size_t mrb_obj_iv_tbl_memsize(mrb_value);
void mrb_obj_iv_set_force(mrb_state *mrb, struct RObject *obj, mrb_sym sym, mrb_value v);
mrb_value mrb_mod_constants(mrb_state *mrb, mrb_value mod);
mrb_value mrb_mod_const_at(mrb_state *mrb, struct RClass *c, mrb_value ary);
mrb_value mrb_f_global_variables(mrb_state *mrb, mrb_value self);
mrb_value mrb_obj_instance_variables(mrb_state*, mrb_value);
mrb_value mrb_mod_class_variables(mrb_state*, mrb_value);
mrb_value mrb_mod_cv_get(mrb_state *mrb, struct RClass *c, mrb_sym sym);
mrb_bool mrb_mod_cv_defined(mrb_state *mrb, struct RClass *c, mrb_sym sym);
mrb_bool mrb_ident_p(const char *s, mrb_int len);
mrb_value mrb_exc_const_get(mrb_state *mrb, mrb_sym sym);

/* GC functions */
void mrb_gc_mark_gv(mrb_state*);
void mrb_gc_free_gv(mrb_state*);
size_t mrb_gc_mark_iv(mrb_state*, struct RObject*);
void mrb_gc_free_iv(mrb_state*, struct RObject*);

/* VM */
#define MRB_CI_VISIBILITY(ci) MRB_FLAGS_GET((ci)->vis, 0, 2)
#define MRB_CI_SET_VISIBILITY(ci, visi) MRB_FLAGS_SET((ci)->vis, 0, 2, visi)
#define MRB_CI_VISIBILITY_BREAK_P(ci) MRB_FLAG_CHECK((ci)->vis, 2)
#define MRB_CI_SET_VISIBILITY_BREAK(ci) MRB_FLAG_ON((ci)->vis, 2)
mrb_int mrb_ci_bidx(mrb_callinfo *ci);
mrb_int mrb_ci_nregs(mrb_callinfo *ci);
mrb_value mrb_exec_irep(mrb_state *mrb, mrb_value self, const struct RProc *p);
mrb_value mrb_obj_instance_eval(mrb_state*, mrb_value);
mrb_value mrb_object_exec(mrb_state *mrb, mrb_value self, struct RClass *target_class);
mrb_value mrb_mod_module_eval(mrb_state*, mrb_value);
mrb_value mrb_f_send(mrb_state *mrb, mrb_value self);
mrb_value mrb_f_public_send(mrb_state *mrb, mrb_value self);

#ifdef MRB_USE_BIGINT
mrb_value mrb_bint_new_int(mrb_state *mrb, mrb_int x);
#ifdef MRB_INT64
#define mrb_bint_new_int64(mrb,x) mrb_bint_new_int((mrb),(mrb_int)(x))
#else
mrb_value mrb_bint_new_int64(mrb_state *mrb, int64_t x);
#endif
mrb_value mrb_bint_new_uint64(mrb_state *mrb, uint64_t x);
mrb_value mrb_bint_new_str(mrb_state *mrb, const char *x, mrb_int len, mrb_int base);
mrb_value mrb_as_bint(mrb_state *mrb, mrb_value x);
mrb_value mrb_bint_add(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_bint_sub(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_bint_add_n(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_bint_sub_n(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_bint_mul(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_bint_div(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_bint_divmod(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_bint_add_ii(mrb_state *mrb, mrb_int x, mrb_int y);
mrb_value mrb_bint_sub_ii(mrb_state *mrb, mrb_int x, mrb_int y);
mrb_value mrb_bint_mul_ii(mrb_state *mrb, mrb_int x, mrb_int y);
mrb_value mrb_bint_mod(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_bint_rem(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_bint_pow(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_bint_powm(mrb_state *mrb, mrb_value x, mrb_value y, mrb_value z);
mrb_value mrb_bint_and(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_bint_or(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_bint_neg(mrb_state *mrb, mrb_value x);
mrb_value mrb_bint_xor(mrb_state *mrb, mrb_value x, mrb_value y);
mrb_value mrb_bint_rev(mrb_state *mrb, mrb_value x);
mrb_value mrb_bint_lshift(mrb_state *mrb, mrb_value x, mrb_int width);
mrb_value mrb_bint_rshift(mrb_state *mrb, mrb_value x, mrb_int width);
mrb_value mrb_bint_to_s(mrb_state *mrb, mrb_value x, mrb_int base);
#ifndef MRB_NO_FLOAT
mrb_value mrb_bint_new_float(mrb_state *mrb, mrb_float x);
mrb_float mrb_bint_as_float(mrb_state *mrb, mrb_value x);
#endif
mrb_int mrb_bint_as_int(mrb_state *mrb, mrb_value x);
#ifdef MRB_INT64
#define mrb_bint_as_int64(mrb, x) mrb_bint_as_int((mrb), (x))
#else
int64_t mrb_bint_as_int64(mrb_state *mrb, mrb_value x);
#endif
uint64_t mrb_bint_as_uint64(mrb_state *mrb, mrb_value x);
mrb_int mrb_bint_cmp(mrb_state *mrb, mrb_value x, mrb_value y);
void mrb_gc_free_bint(mrb_state *mrb, struct RBasic *x);
void mrb_bint_copy(mrb_state *mrb, mrb_value x, mrb_value y);
size_t mrb_bint_memsize(mrb_value x);
mrb_value mrb_bint_hash(mrb_state *mrb, mrb_value x);
mrb_value mrb_bint_sqrt(mrb_state *mrb, mrb_value x);
#endif

#endif  /* MRUBY_INTERNAL_H */
