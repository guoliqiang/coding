#define tok_identifier 257
#define tok_literal 258
#define tok_doctext 259
#define tok_st_identifier 260
#define tok_int_constant 261
#define tok_dub_constant 262
#define tok_include 263
#define tok_namespace 264
#define tok_cpp_namespace 265
#define tok_cpp_include 266
#define tok_cpp_type 267
#define tok_php_namespace 268
#define tok_py_module 269
#define tok_perl_package 270
#define tok_java_package 271
#define tok_xsd_all 272
#define tok_xsd_optional 273
#define tok_xsd_nillable 274
#define tok_xsd_namespace 275
#define tok_xsd_attrs 276
#define tok_ruby_namespace 277
#define tok_smalltalk_category 278
#define tok_smalltalk_prefix 279
#define tok_cocoa_prefix 280
#define tok_csharp_namespace 281
#define tok_delphi_namespace 282
#define tok_void 283
#define tok_bool 284
#define tok_byte 285
#define tok_string 286
#define tok_binary 287
#define tok_slist 288
#define tok_senum 289
#define tok_i16 290
#define tok_i32 291
#define tok_i64 292
#define tok_double 293
#define tok_map 294
#define tok_list 295
#define tok_set 296
#define tok_oneway 297
#define tok_typedef 298
#define tok_struct 299
#define tok_xception 300
#define tok_throws 301
#define tok_extends 302
#define tok_service 303
#define tok_enum 304
#define tok_const 305
#define tok_required 306
#define tok_optional 307
#define tok_union 308
typedef union {
  char*          id;
  int64_t        iconst;
  double         dconst;
  bool           tbool;
  t_doc*         tdoc;
  t_type*        ttype;
  t_base_type*   tbase;
  t_typedef*     ttypedef;
  t_enum*        tenum;
  t_enum_value*  tenumv;
  t_const*       tconst;
  t_const_value* tconstv;
  t_struct*      tstruct;
  t_service*     tservice;
  t_function*    tfunction;
  t_field*       tfield;
  char*          dtext;
  t_field::e_req ereq;
  t_annotation*  tannot;
  t_field_id     tfieldid;
} YYSTYPE;
extern YYSTYPE yylval;
