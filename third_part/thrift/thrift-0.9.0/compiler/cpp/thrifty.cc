#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#include <stdlib.h>
#include <string.h>

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20090221

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
#ifdef YYPARSE_PARAM_TYPE
#define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
#else
#define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
#endif
#else
#define YYPARSE_DECL() yyparse(void)
#endif /* YYPARSE_PARAM */

extern int YYPARSE_DECL();

static int yygrowstack(void);
#define YYPREFIX "yy"
#line 2 "thrifty.yy"
/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/**
 * Thrift parser.
 *
 * This parser is used on a thrift definition file.
 *
 */

#define __STDC_LIMIT_MACROS
#define __STDC_FORMAT_MACROS
#include <stdio.h>
#ifndef _MSC_VER
#include <inttypes.h>
#else
#include <stdint.h>
#endif
#include <limits.h>
#include "main.h"
#include "globals.h"
#include "parse/t_program.h"
#include "parse/t_scope.h"

/**
 * This global variable is used for automatic numbering of field indices etc.
 * when parsing the members of a struct. Field values are automatically
 * assigned starting from -1 and working their way down.
 */
int y_field_val = -1;
int g_arglist = 0;
const int struct_is_struct = 0;
const int struct_is_union = 1;

#line 58 "thrifty.yy"
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
#line 108 "thrifty.cc"
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
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
    0,   47,   50,   48,   48,   51,   51,   51,   51,   51,
   51,   51,   51,   51,   51,   51,   51,   51,   51,   51,
   51,   52,   49,   49,    8,    8,    8,    9,    9,    9,
    9,    9,   10,   53,   53,   53,   20,   21,   21,   22,
   22,   23,   24,   24,   25,   26,   27,   27,   27,   27,
   27,   27,   28,   29,   29,   30,   31,   31,   32,   32,
   33,   42,   42,   43,   43,   44,   44,   45,   45,   34,
   35,   54,   55,   40,   40,   38,   38,   36,   41,   41,
   39,   39,   19,   19,   14,   15,   15,   16,   16,   16,
   18,   18,   37,   37,   17,   17,   17,    1,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    3,    4,    4,
    4,    5,    6,    7,   46,   46,   11,   11,   12,   12,
   13,
};
static const short yylen[] = {                            2,
    2,    0,    0,    3,    0,    1,    3,    3,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    3,    0,    1,    1,    1,    1,    1,    1,
    1,    1,    4,    1,    1,    0,    6,    2,    0,    6,
    4,    6,    2,    0,    2,    6,    1,    1,    1,    1,
    1,    1,    3,    3,    0,    3,    5,    0,    1,    1,
    7,    1,    0,    1,    0,    1,    0,    4,    0,    6,
    9,    0,    0,    2,    0,    2,    0,   10,    1,    0,
    4,    0,    2,    0,   11,    2,    0,    1,    1,    0,
    2,    0,    1,    1,    1,    1,    1,    2,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    2,    1,    1,
    1,    7,    5,    5,    2,    0,    3,    0,    2,    0,
    4,
};
static const short yydefred[] = {                         5,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    4,    6,    0,    0,   59,    0,    0,    0,    0,   60,
   23,   26,   28,   29,   30,   25,    0,   31,   32,   27,
   22,    0,    0,    9,   10,   11,   12,   13,   17,   19,
   14,   15,   16,   18,   20,   21,    0,   95,  102,  103,
   99,  100,  101,  104,  105,  106,  107,    0,    0,    0,
   96,    0,   97,    0,  109,  110,  111,    0,    0,    0,
    0,    0,    0,    7,    8,   44,    0,    0,    0,    0,
  120,   98,  108,    0,   84,    0,    0,   39,    0,   62,
    0,    0,  115,    0,    0,    0,    0,   33,    0,   74,
   72,    0,    0,   84,    0,    0,   43,    0,    0,    0,
    0,  117,  119,    0,   83,    0,   77,    0,   38,    0,
   50,   49,   47,   48,   58,   55,    0,   51,   52,    0,
   34,   35,   45,   42,    0,  114,  113,    0,   70,    0,
    0,    0,   37,    0,    0,    0,   46,    0,    0,    0,
   86,   88,   89,    0,   76,    0,    0,    0,    0,   56,
    0,   53,    0,   61,  112,  121,    0,   79,    0,    0,
    0,   41,    0,   54,    0,   94,   93,    0,   71,    0,
    0,    0,    0,    0,   40,   57,   91,   64,    0,   84,
   66,    0,    0,    0,    0,    0,   84,    0,    0,    0,
    0,   85,   84,    0,   68,    0,   78,   81,
};
static const short yydgoto[] = {                          1,
   71,   72,   73,   74,   75,   76,   77,   31,   32,   33,
   92,  107,  123,  125,  151,  164,   78,  193,  109,   34,
  112,  129,   35,  102,  117,   36,  137,  138,  156,  139,
  155,   37,   38,   39,   40,  165,  188,  152,  210,   97,
  179,  101,  199,  202,  205,   88,  126,    2,    3,    4,
   21,   22,  143,  127,  167,
};
static const short yysindex[] = {                         0,
    0,    0,    0,  421, -247, -239,  -37, -234, -231, -228,
 -226, -223, -217, -215, -212, -206, -202, -190, -187, -184,
    0,    0, -183,  372,    0, -180, -179, -178,  372,    0,
    0,    0,    0,    0,    0,    0, -177,    0,    0,    0,
    0, -175, -174,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -36,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -181,   24, -181,
    0,   48,    0,   48,    0,    0,    0, -168,  -29, -207,
  -27, -160, -173,    0,    0,    0, -158,   47,  372,   49,
    0,    0,    0,   48,    0, -149,  -11,    0,   55,    0,
   -6, -111,    0,  372,   56,  372,  -39,    0,   -4,    0,
    0,   -3,  -25,    0,   -9,   48,    0,   79, -181,   62,
   64,    0,    0,   48,    0, -125,    0,   48,    0, -112,
    0,    0,    0,    0,    0,    0,   -9,    0,    0,   21,
    0,    0,    0,    0,  372,    0,    0, -105,    0,   98,
 -259,    0,    0,  -22,  -61,  -55,    0,   48,   96,   -9,
    0,    0,    0,  372,    0, -138,   35, -100,   -9,    0,
  104,    0,   -9,    0,    0,    0,  -93,    0, -153,   48,
   48,    0,  -25,    0,  102,    0,    0,  -92,    0,   -9,
   -9,  -25, -107,  127,    0,    0,    0,    0, -106,    0,
    0,  -91,  135,   63,   48, -114,    0,   -9,  148,   48,
   67,    0,    0,   -9,    0,  152,    0,    0,
};
static const short yyrindex[] = {                         0,
    0,    1,   13,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  129,    0,  129,
    0,  -34,    0,  -34,    0,    0,    0,    0,    0,   75,
    0,    0,   76,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   25,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  308,    0,
    0,  -52,    0,    0, -110,   25,    0,    0,  -40,    0,
    0,    0,    0,   25,    0,  268,    0,   25,    0,    0,
    0,    0,    0,    0,    0,    0,   37,    0,    0,  308,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  387, -113,    0,  -35,    0,    0,    0,   25,    0,  -38,
    0,    0,    0,    0,    0,  355,    0,    0, -108,    0,
    0,    0,  -47,    0,    0,    0,    0,    0,    0,   25,
  -35,    0,    0,    0,  -33,    0,    0,    0,    0, -108,
  -32,    0,   70,    0,    0,    0,    0,    0,  110,    0,
    0,  150,  308,    0,  190,  294,    0,  243,    0,  340,
  308,    0,    0,  222,    0,  308,    0,    0,
};
static const short yygindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -53,    0,    0,    0,    0,    0,  323,    0,  -94,    0,
    0,    0,    0,    0,    0,    0, -123,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  -54,    3,    0,    0,    0,
    0,    0,  -88,    0,    0,
};
#define YYTABLESIZE 703
static const short yytable[] = {                        116,
   24,  122,   36,  116,   43,    5,   92,   92,  118,  118,
   92,   73,    1,  116,   36,   90,   36,   91,   41,  140,
   93,  116,   44,  118,  118,   92,   45,  118,   46,  136,
   47,  171,  173,   48,  141,  136,   36,  172,  168,   49,
  108,   23,   50,   36,   51,   36,  162,  163,  157,  142,
   24,   25,   26,   52,   53,   27,   28,   29,   36,  191,
   30,  135,  144,  170,  146,  136,   54,  135,  197,   55,
  149,  176,   56,   57,  153,   36,   79,   80,   81,   83,
  182,   84,   85,   89,  184,   87,   86,   91,   94,  118,
   36,   92,   36,   95,   96,   98,   99,  135,  100,  103,
  169,  195,  196,   58,  174,  203,  104,  110,  106,   65,
   65,  111,  211,   65,  130,  113,  114,  119,  216,  212,
  124,  128,  145,  147,  148,  217,  189,  190,   65,  186,
   59,   60,   61,   62,   63,  150,   64,   65,   66,   67,
   68,   69,   70,    2,  154,  158,  115,   36,   36,   67,
   67,  208,  160,   67,  166,  161,  214,  175,  178,  180,
  181,  183,  192,  185,  194,  198,  200,  201,   67,    2,
    2,    2,    2,    2,    2,  206,    2,    2,    2,    2,
    2,    2,    2,    2,  204,  207,  209,  213,  116,   69,
   69,  215,  218,   69,   65,  131,  132,   75,   63,  133,
  134,  131,  132,    0,    2,  133,  134,    0,   69,   36,
   36,    0,    0,   36,   36,    0,  116,  121,   36,   42,
    0,  118,  118,   92,   36,   36,    0,   92,   36,   36,
  118,  131,  132,  118,   67,  133,  134,    0,    0,   92,
   92,    0,   92,    0,    0,    0,    0,    0,  118,    0,
   92,   92,   92,   92,   92,    0,   92,   92,   92,   92,
   92,   92,   92,    3,    3,    3,    3,    0,    3,    3,
    3,    3,   92,   92,   69,    3,    0,    3,    3,    3,
    3,    3,    3,   36,    0,    0,    0,    0,    0,   24,
    0,    0,    0,    0,    0,    0,    0,    0,   24,   24,
   24,    2,    0,   24,   24,   24,    0,    0,   24,    0,
    2,    2,    2,  118,  118,    2,    2,    2,    0,    0,
    2,    0,  118,  118,  118,   36,   65,  118,  118,  118,
   65,    0,  118,   82,   36,   36,   36,   82,    0,   36,
   36,   36,    0,   65,   36,   65,   36,    0,    0,    0,
    0,   82,   82,   65,   65,   65,   65,   65,    0,   65,
   65,   65,   65,   65,   65,   65,   67,   36,    0,    0,
   67,    0,    0,    0,    0,   65,   65,    0,    0,    0,
    0,    0,    0,  118,    0,   67,    0,    0,    0,    0,
    0,    0,    0,   67,   67,   67,   67,   67,  118,   67,
   67,   67,   67,   67,   67,   67,   69,    0,    0,    0,
   69,  105,    0,    0,    0,   67,   67,    0,   82,    0,
    0,    0,    0,    0,    0,    0,  118,    0,  120,    0,
    0,    0,    0,   69,   69,   69,   69,   69,    0,   69,
   69,   69,   69,   69,   69,   69,  118,    0,    0,    0,
  118,    0,    0,    0,    0,   69,   69,    0,    0,    0,
    0,    0,    0,    0,  118,    0,    0,  159,    0,    0,
    0,    0,    0,  118,  118,  118,  118,  118,   36,  118,
  118,  118,  118,  118,  118,  118,  177,    0,    0,    0,
    0,    0,    0,    0,    0,  118,  118,    0,    0,   36,
    0,  187,    0,   36,   36,   36,   36,   36,   36,   36,
    0,   36,   36,   36,   36,   36,   36,   36,   36,    0,
    0,    0,    0,    0,   87,    0,   36,   36,   36,   36,
   36,    0,   36,   36,   36,   36,   36,   36,   36,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   36,   36,
   82,   87,   87,   87,   87,   87,    0,   87,   87,   87,
   87,   87,   87,   87,    2,    0,    0,    0,    2,    0,
    0,    0,    0,   87,   87,    0,   82,   82,   82,   82,
   82,   82,    0,   82,   82,   82,   82,   82,   82,   82,
   82,    2,    2,    2,    2,    2,  118,    2,    2,    2,
    2,    2,    2,    2,    0,    0,    0,    0,    0,    0,
    0,   80,    0,    2,    2,    0,    0,    0,    0,    0,
    0,    0,  118,  118,  118,  118,  118,  118,   58,  118,
  118,  118,  118,  118,  118,  118,  118,   80,   80,   80,
   80,   80,   80,   90,   80,   80,   80,   80,   80,   80,
   80,    0,    0,    0,    0,   59,   60,   61,   62,   63,
    0,   64,   65,   66,   67,   68,   69,   70,    0,    0,
   90,   90,   90,   90,   90,    0,   90,   90,   90,   90,
   90,   90,   90,    6,    7,    8,    9,    0,   10,   11,
   12,   13,    0,    0,    0,   14,    0,   15,   16,   17,
   18,   19,   20,
};
static const short yycheck[] = {                         40,
    0,   41,   41,   44,   42,    3,   40,   41,   44,   44,
   44,  125,    0,  125,  125,   70,  125,   40,  258,  114,
   74,   62,  257,   59,    0,   59,  258,   62,  257,   91,
  257,  155,  156,  257,   44,   91,    0,   93,   61,  257,
   94,  289,  258,   91,  257,   93,  306,  307,  137,   59,
  298,  299,  300,  260,  257,  303,  304,  305,   91,  183,
  308,  123,  116,  125,  119,   91,  257,  123,  192,  257,
  124,  160,  257,  257,  128,  123,  257,  257,  257,  257,
  169,  257,  257,   60,  173,  267,  123,   40,  257,  125,
  123,  125,  125,  123,  302,  123,  257,  123,  272,  258,
  154,  190,  191,  257,  158,  200,   60,  257,   60,   40,
   41,  123,  207,   44,  112,   61,  123,   62,  213,  208,
  125,  125,   44,   62,   61,  214,  180,  181,   59,  283,
  284,  285,  286,  287,  288,  261,  290,  291,  292,  293,
  294,  295,  296,  257,  257,  125,  258,  258,  257,   40,
   41,  205,  258,   44,  152,   58,  210,   62,  297,  125,
  261,   58,   61,  257,  257,  273,   40,  274,   59,  283,
  284,  285,  286,  287,  288,   41,  290,  291,  292,  293,
  294,  295,  296,  297,  276,  123,  301,   40,   60,   40,
   41,  125,   41,   44,  125,  257,  258,  123,  123,  261,
  262,  257,  258,   -1,  257,  261,  262,   -1,   59,  257,
  258,   -1,   -1,  261,  262,   -1,  257,  257,  257,  257,
   -1,  257,  257,  257,  257,  258,   -1,  261,  261,  262,
   41,  257,  258,   44,  125,  261,  262,   -1,   -1,  273,
  274,   -1,  276,   -1,   -1,   -1,   -1,   -1,   59,   -1,
  284,  285,  286,  287,  288,   -1,  290,  291,  292,  293,
  294,  295,  296,  263,  264,  265,  266,   -1,  268,  269,
  270,  271,  306,  307,  125,  275,   -1,  277,  278,  279,
  280,  281,  282,   41,   -1,   -1,   -1,   -1,   -1,  289,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  298,  299,
  300,  289,   -1,  303,  304,  305,   -1,   -1,  308,   -1,
  298,  299,  300,  289,  125,  303,  304,  305,   -1,   -1,
  308,   -1,  298,  299,  300,  289,  257,  303,  304,  305,
  261,   -1,  308,   40,  298,  299,  300,   44,   -1,  303,
  304,  305,   -1,  274,  308,  276,  125,   -1,   -1,   -1,
   -1,   29,   59,  284,  285,  286,  287,  288,   -1,  290,
  291,  292,  293,  294,  295,  296,  257,  125,   -1,   -1,
  261,   -1,   -1,   -1,   -1,  306,  307,   -1,   -1,   -1,
   -1,   -1,   -1,   44,   -1,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  284,  285,  286,  287,  288,   59,  290,
  291,  292,  293,  294,  295,  296,  257,   -1,   -1,   -1,
  261,   89,   -1,   -1,   -1,  306,  307,   -1,  125,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  104,   -1,  106,   -1,
   -1,   -1,   -1,  284,  285,  286,  287,  288,   -1,  290,
  291,  292,  293,  294,  295,  296,  257,   -1,   -1,   -1,
  261,   -1,   -1,   -1,   -1,  306,  307,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  125,   -1,   -1,  145,   -1,   -1,
   -1,   -1,   -1,  284,  285,  286,  287,  288,  257,  290,
  291,  292,  293,  294,  295,  296,  164,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  306,  307,   -1,   -1,  257,
   -1,  179,   -1,  261,  283,  284,  285,  286,  287,  288,
   -1,  290,  291,  292,  293,  294,  295,  296,  297,   -1,
   -1,   -1,   -1,   -1,  257,   -1,  284,  285,  286,  287,
  288,   -1,  290,  291,  292,  293,  294,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  306,  307,
  257,  284,  285,  286,  287,  288,   -1,  290,  291,  292,
  293,  294,  295,  296,  257,   -1,   -1,   -1,  261,   -1,
   -1,   -1,   -1,  306,  307,   -1,  283,  284,  285,  286,
  287,  288,   -1,  290,  291,  292,  293,  294,  295,  296,
  297,  284,  285,  286,  287,  288,  257,  290,  291,  292,
  293,  294,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  257,   -1,  306,  307,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  283,  284,  285,  286,  287,  288,  257,  290,
  291,  292,  293,  294,  295,  296,  297,  283,  284,  285,
  286,  287,  288,  257,  290,  291,  292,  293,  294,  295,
  296,   -1,   -1,   -1,   -1,  284,  285,  286,  287,  288,
   -1,  290,  291,  292,  293,  294,  295,  296,   -1,   -1,
  284,  285,  286,  287,  288,   -1,  290,  291,  292,  293,
  294,  295,  296,  263,  264,  265,  266,   -1,  268,  269,
  270,  271,   -1,   -1,   -1,  275,   -1,  277,  278,  279,
  280,  281,  282,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 308
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'","'*'",0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,"':'","';'",
"'<'","'='","'>'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'['",
0,"']'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,"tok_identifier","tok_literal","tok_doctext",
"tok_st_identifier","tok_int_constant","tok_dub_constant","tok_include",
"tok_namespace","tok_cpp_namespace","tok_cpp_include","tok_cpp_type",
"tok_php_namespace","tok_py_module","tok_perl_package","tok_java_package",
"tok_xsd_all","tok_xsd_optional","tok_xsd_nillable","tok_xsd_namespace",
"tok_xsd_attrs","tok_ruby_namespace","tok_smalltalk_category",
"tok_smalltalk_prefix","tok_cocoa_prefix","tok_csharp_namespace",
"tok_delphi_namespace","tok_void","tok_bool","tok_byte","tok_string",
"tok_binary","tok_slist","tok_senum","tok_i16","tok_i32","tok_i64","tok_double",
"tok_map","tok_list","tok_set","tok_oneway","tok_typedef","tok_struct",
"tok_xception","tok_throws","tok_extends","tok_service","tok_enum","tok_const",
"tok_required","tok_optional","tok_union",
};
static const char *yyrule[] = {
"$accept : Program",
"Program : HeaderList DefinitionList",
"CaptureDocText :",
"DestroyDocText :",
"HeaderList : HeaderList DestroyDocText Header",
"HeaderList :",
"Header : Include",
"Header : tok_namespace tok_identifier tok_identifier",
"Header : tok_namespace '*' tok_identifier",
"Header : tok_cpp_namespace tok_identifier",
"Header : tok_cpp_include tok_literal",
"Header : tok_php_namespace tok_identifier",
"Header : tok_py_module tok_identifier",
"Header : tok_perl_package tok_identifier",
"Header : tok_ruby_namespace tok_identifier",
"Header : tok_smalltalk_category tok_st_identifier",
"Header : tok_smalltalk_prefix tok_identifier",
"Header : tok_java_package tok_identifier",
"Header : tok_cocoa_prefix tok_identifier",
"Header : tok_xsd_namespace tok_literal",
"Header : tok_csharp_namespace tok_identifier",
"Header : tok_delphi_namespace tok_identifier",
"Include : tok_include tok_literal",
"DefinitionList : DefinitionList CaptureDocText Definition",
"DefinitionList :",
"Definition : Const",
"Definition : TypeDefinition",
"Definition : Service",
"TypeDefinition : Typedef",
"TypeDefinition : Enum",
"TypeDefinition : Senum",
"TypeDefinition : Struct",
"TypeDefinition : Xception",
"Typedef : tok_typedef FieldType tok_identifier TypeAnnotations",
"CommaOrSemicolonOptional : ','",
"CommaOrSemicolonOptional : ';'",
"CommaOrSemicolonOptional :",
"Enum : tok_enum tok_identifier '{' EnumDefList '}' TypeAnnotations",
"EnumDefList : EnumDefList EnumDef",
"EnumDefList :",
"EnumDef : CaptureDocText tok_identifier '=' tok_int_constant TypeAnnotations CommaOrSemicolonOptional",
"EnumDef : CaptureDocText tok_identifier TypeAnnotations CommaOrSemicolonOptional",
"Senum : tok_senum tok_identifier '{' SenumDefList '}' TypeAnnotations",
"SenumDefList : SenumDefList SenumDef",
"SenumDefList :",
"SenumDef : tok_literal CommaOrSemicolonOptional",
"Const : tok_const FieldType tok_identifier '=' ConstValue CommaOrSemicolonOptional",
"ConstValue : tok_int_constant",
"ConstValue : tok_dub_constant",
"ConstValue : tok_literal",
"ConstValue : tok_identifier",
"ConstValue : ConstList",
"ConstValue : ConstMap",
"ConstList : '[' ConstListContents ']'",
"ConstListContents : ConstListContents ConstValue CommaOrSemicolonOptional",
"ConstListContents :",
"ConstMap : '{' ConstMapContents '}'",
"ConstMapContents : ConstMapContents ConstValue ':' ConstValue CommaOrSemicolonOptional",
"ConstMapContents :",
"StructHead : tok_struct",
"StructHead : tok_union",
"Struct : StructHead tok_identifier XsdAll '{' FieldList '}' TypeAnnotations",
"XsdAll : tok_xsd_all",
"XsdAll :",
"XsdOptional : tok_xsd_optional",
"XsdOptional :",
"XsdNillable : tok_xsd_nillable",
"XsdNillable :",
"XsdAttributes : tok_xsd_attrs '{' FieldList '}'",
"XsdAttributes :",
"Xception : tok_xception tok_identifier '{' FieldList '}' TypeAnnotations",
"Service : tok_service tok_identifier Extends '{' FlagArgs FunctionList UnflagArgs '}' TypeAnnotations",
"FlagArgs :",
"UnflagArgs :",
"Extends : tok_extends tok_identifier",
"Extends :",
"FunctionList : FunctionList Function",
"FunctionList :",
"Function : CaptureDocText Oneway FunctionType tok_identifier '(' FieldList ')' Throws TypeAnnotations CommaOrSemicolonOptional",
"Oneway : tok_oneway",
"Oneway :",
"Throws : tok_throws '(' FieldList ')'",
"Throws :",
"FieldList : FieldList Field",
"FieldList :",
"Field : CaptureDocText FieldIdentifier FieldRequiredness FieldType tok_identifier FieldValue XsdOptional XsdNillable XsdAttributes TypeAnnotations CommaOrSemicolonOptional",
"FieldIdentifier : tok_int_constant ':'",
"FieldIdentifier :",
"FieldRequiredness : tok_required",
"FieldRequiredness : tok_optional",
"FieldRequiredness :",
"FieldValue : '=' ConstValue",
"FieldValue :",
"FunctionType : FieldType",
"FunctionType : tok_void",
"FieldType : tok_identifier",
"FieldType : BaseType",
"FieldType : ContainerType",
"BaseType : SimpleBaseType TypeAnnotations",
"SimpleBaseType : tok_string",
"SimpleBaseType : tok_binary",
"SimpleBaseType : tok_slist",
"SimpleBaseType : tok_bool",
"SimpleBaseType : tok_byte",
"SimpleBaseType : tok_i16",
"SimpleBaseType : tok_i32",
"SimpleBaseType : tok_i64",
"SimpleBaseType : tok_double",
"ContainerType : SimpleContainerType TypeAnnotations",
"SimpleContainerType : MapType",
"SimpleContainerType : SetType",
"SimpleContainerType : ListType",
"MapType : tok_map CppType '<' FieldType ',' FieldType '>'",
"SetType : tok_set CppType '<' FieldType '>'",
"ListType : tok_list '<' FieldType '>' CppType",
"CppType : tok_cpp_type tok_literal",
"CppType :",
"TypeAnnotations : '(' TypeAnnotationList ')'",
"TypeAnnotations :",
"TypeAnnotationList : TypeAnnotationList TypeAnnotation",
"TypeAnnotationList :",
"TypeAnnotation : tok_identifier '=' tok_literal CommaOrSemicolonOptional",

};
#endif
#if YYDEBUG
#include <stdio.h>
#endif

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH  500
#endif
#endif

#define YYINITSTACKSIZE 500

int      yydebug;
int      yynerrs;
int      yyerrflag;
int      yychar;
short   *yyssp;
YYSTYPE *yyvsp;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* variables for the parser stack */
static short   *yyss;
static short   *yysslim;
static YYSTYPE *yyvs;
static unsigned yystacksize;
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(void)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = yyssp - yyss;
    newss = (yyss != 0)
          ? (short *)realloc(yyss, newsize * sizeof(*newss))
          : (short *)malloc(newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    yyss  = newss;
    yyssp = newss + i;
    newvs = (yyvs != 0)
          ? (YYSTYPE *)realloc(yyvs, newsize * sizeof(*newvs))
          : (YYSTYPE *)malloc(newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    yystate = 0;
    *yyssp = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yyssp = yytable[yyn];
        *++yyvsp = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yyssp = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yyvsp[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 236 "thrifty.yy"
	{
      pdebug("Program -> Headers DefinitionList");
      /*
      TODO(dreiss): Decide whether full-program doctext is worth the trouble.
      if ($1 != NULL) {
        g_program->set_doc($1);
      }
      */
      clear_doctext();
    }
break;
case 2:
#line 248 "thrifty.yy"
	{
      if (g_parse_mode == PROGRAM) {
        yyval.dtext = g_doctext;
        g_doctext = NULL;
      } else {
        yyval.dtext = NULL;
      }
    }
break;
case 3:
#line 259 "thrifty.yy"
	{
      if (g_parse_mode == PROGRAM) {
        clear_doctext();
      }
    }
break;
case 4:
#line 269 "thrifty.yy"
	{
      pdebug("HeaderList -> HeaderList Header");
    }
break;
case 5:
#line 273 "thrifty.yy"
	{
      pdebug("HeaderList -> ");
    }
break;
case 6:
#line 279 "thrifty.yy"
	{
      pdebug("Header -> Include");
    }
break;
case 7:
#line 283 "thrifty.yy"
	{
      pdebug("Header -> tok_namespace tok_identifier tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace(yyvsp[-1].id, yyvsp[0].id);
      }
    }
break;
case 8:
#line 290 "thrifty.yy"
	{
      pdebug("Header -> tok_namespace * tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("*", yyvsp[0].id);
      }
    }
break;
case 9:
#line 298 "thrifty.yy"
	{
      pwarning(1, "'cpp_namespace' is deprecated. Use 'namespace cpp' instead");
      pdebug("Header -> tok_cpp_namespace tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("cpp", yyvsp[0].id);
      }
    }
break;
case 10:
#line 306 "thrifty.yy"
	{
      pdebug("Header -> tok_cpp_include tok_literal");
      if (g_parse_mode == PROGRAM) {
        g_program->add_cpp_include(yyvsp[0].id);
      }
    }
break;
case 11:
#line 313 "thrifty.yy"
	{
      pwarning(1, "'php_namespace' is deprecated. Use 'namespace php' instead");
      pdebug("Header -> tok_php_namespace tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("php", yyvsp[0].id);
      }
    }
break;
case 12:
#line 322 "thrifty.yy"
	{
      pwarning(1, "'py_module' is deprecated. Use 'namespace py' instead");
      pdebug("Header -> tok_py_module tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("py", yyvsp[0].id);
      }
    }
break;
case 13:
#line 331 "thrifty.yy"
	{
      pwarning(1, "'perl_package' is deprecated. Use 'namespace perl' instead");
      pdebug("Header -> tok_perl_namespace tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("perl", yyvsp[0].id);
      }
    }
break;
case 14:
#line 340 "thrifty.yy"
	{
      pwarning(1, "'ruby_namespace' is deprecated. Use 'namespace rb' instead");
      pdebug("Header -> tok_ruby_namespace tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("rb", yyvsp[0].id);
      }
    }
break;
case 15:
#line 349 "thrifty.yy"
	{
      pwarning(1, "'smalltalk_category' is deprecated. Use 'namespace smalltalk.category' instead");
      pdebug("Header -> tok_smalltalk_category tok_st_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("smalltalk.category", yyvsp[0].id);
      }
    }
break;
case 16:
#line 358 "thrifty.yy"
	{
      pwarning(1, "'smalltalk_prefix' is deprecated. Use 'namespace smalltalk.prefix' instead");
      pdebug("Header -> tok_smalltalk_prefix tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("smalltalk.prefix", yyvsp[0].id);
      }
    }
break;
case 17:
#line 367 "thrifty.yy"
	{
      pwarning(1, "'java_package' is deprecated. Use 'namespace java' instead");
      pdebug("Header -> tok_java_package tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("java", yyvsp[0].id);
      }
    }
break;
case 18:
#line 376 "thrifty.yy"
	{
      pwarning(1, "'cocoa_prefix' is deprecated. Use 'namespace cocoa' instead");
      pdebug("Header -> tok_cocoa_prefix tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("cocoa", yyvsp[0].id);
      }
    }
break;
case 19:
#line 385 "thrifty.yy"
	{
      pwarning(1, "'xsd_namespace' is deprecated. Use 'namespace xsd' instead");
      pdebug("Header -> tok_xsd_namespace tok_literal");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("cocoa", yyvsp[0].id);
      }
    }
break;
case 20:
#line 394 "thrifty.yy"
	{
     pwarning(1, "'csharp_namespace' is deprecated. Use 'namespace csharp' instead");
     pdebug("Header -> tok_csharp_namespace tok_identifier");
     if (g_parse_mode == PROGRAM) {
       g_program->set_namespace("csharp", yyvsp[0].id);
     }
   }
break;
case 21:
#line 403 "thrifty.yy"
	{
     pwarning(1, "'delphi_namespace' is deprecated. Use 'namespace delphi' instead");
     pdebug("Header -> tok_delphi_namespace tok_identifier");
     if (g_parse_mode == PROGRAM) {
       g_program->set_namespace("delphi", yyvsp[0].id);
     }
   }
break;
case 22:
#line 413 "thrifty.yy"
	{
      pdebug("Include -> tok_include tok_literal");
      if (g_parse_mode == INCLUDES) {
        std::string path = include_file(std::string(yyvsp[0].id));
        if (!path.empty()) {
          g_program->add_include(path, std::string(yyvsp[0].id));
        }
      }
    }
break;
case 23:
#line 425 "thrifty.yy"
	{
      pdebug("DefinitionList -> DefinitionList Definition");
      if (yyvsp[-1].dtext != NULL && yyvsp[0].tdoc != NULL) {
        yyvsp[0].tdoc->set_doc(yyvsp[-1].dtext);
      }
    }
break;
case 24:
#line 432 "thrifty.yy"
	{
      pdebug("DefinitionList -> ");
    }
break;
case 25:
#line 438 "thrifty.yy"
	{
      pdebug("Definition -> Const");
      if (g_parse_mode == PROGRAM) {
        g_program->add_const(yyvsp[0].tconst);
      }
      yyval.tdoc = yyvsp[0].tconst;
    }
break;
case 26:
#line 446 "thrifty.yy"
	{
      pdebug("Definition -> TypeDefinition");
      if (g_parse_mode == PROGRAM) {
        g_scope->add_type(yyvsp[0].ttype->get_name(), yyvsp[0].ttype);
        if (g_parent_scope != NULL) {
          g_parent_scope->add_type(g_parent_prefix + yyvsp[0].ttype->get_name(), yyvsp[0].ttype);
        }
      }
      yyval.tdoc = yyvsp[0].ttype;
    }
break;
case 27:
#line 457 "thrifty.yy"
	{
      pdebug("Definition -> Service");
      if (g_parse_mode == PROGRAM) {
        g_scope->add_service(yyvsp[0].tservice->get_name(), yyvsp[0].tservice);
        if (g_parent_scope != NULL) {
          g_parent_scope->add_service(g_parent_prefix + yyvsp[0].tservice->get_name(), yyvsp[0].tservice);
        }
        g_program->add_service(yyvsp[0].tservice);
      }
      yyval.tdoc = yyvsp[0].tservice;
    }
break;
case 28:
#line 471 "thrifty.yy"
	{
      pdebug("TypeDefinition -> Typedef");
      if (g_parse_mode == PROGRAM) {
        g_program->add_typedef(yyvsp[0].ttypedef);
      }
    }
break;
case 29:
#line 478 "thrifty.yy"
	{
      pdebug("TypeDefinition -> Enum");
      if (g_parse_mode == PROGRAM) {
        g_program->add_enum(yyvsp[0].tenum);
      }
    }
break;
case 30:
#line 485 "thrifty.yy"
	{
      pdebug("TypeDefinition -> Senum");
      if (g_parse_mode == PROGRAM) {
        g_program->add_typedef(yyvsp[0].ttypedef);
      }
    }
break;
case 31:
#line 492 "thrifty.yy"
	{
      pdebug("TypeDefinition -> Struct");
      if (g_parse_mode == PROGRAM) {
        g_program->add_struct(yyvsp[0].tstruct);
      }
    }
break;
case 32:
#line 499 "thrifty.yy"
	{
      pdebug("TypeDefinition -> Xception");
      if (g_parse_mode == PROGRAM) {
        g_program->add_xception(yyvsp[0].tstruct);
      }
    }
break;
case 33:
#line 508 "thrifty.yy"
	{
      pdebug("TypeDef -> tok_typedef FieldType tok_identifier");
      t_typedef *td = new t_typedef(g_program, yyvsp[-2].ttype, yyvsp[-1].id);
      yyval.ttypedef = td;
      if (yyvsp[0].ttype != NULL) {
        yyval.ttypedef->annotations_ = yyvsp[0].ttype->annotations_;
        delete yyvsp[0].ttype;
      }
    }
break;
case 34:
#line 520 "thrifty.yy"
	{}
break;
case 35:
#line 522 "thrifty.yy"
	{}
break;
case 36:
#line 524 "thrifty.yy"
	{}
break;
case 37:
#line 528 "thrifty.yy"
	{
      pdebug("Enum -> tok_enum tok_identifier { EnumDefList }");
      yyval.tenum = yyvsp[-2].tenum;
      yyval.tenum->set_name(yyvsp[-4].id);
      if (yyvsp[0].ttype != NULL) {
        yyval.tenum->annotations_ = yyvsp[0].ttype->annotations_;
        delete yyvsp[0].ttype;
      }
      yyval.tenum->resolve_values();
      /* make constants for all the enum values*/
      if (g_parse_mode == PROGRAM) {
        const std::vector<t_enum_value*>& enum_values = yyval.tenum->get_constants();
        std::vector<t_enum_value*>::const_iterator c_iter;
        for (c_iter = enum_values.begin(); c_iter != enum_values.end(); ++c_iter) {
          std::string const_name = yyval.tenum->get_name() + "." + (*c_iter)->get_name();
          t_const_value* const_val = new t_const_value((*c_iter)->get_value());
          const_val->set_enum(yyval.tenum);
          g_scope->add_constant(const_name, new t_const(g_type_i32, (*c_iter)->get_name(), const_val));
          if (g_parent_scope != NULL) {
            g_parent_scope->add_constant(g_parent_prefix + const_name, new t_const(g_type_i32, (*c_iter)->get_name(), const_val));
          }
        }
      }
    }
break;
case 38:
#line 555 "thrifty.yy"
	{
      pdebug("EnumDefList -> EnumDefList EnumDef");
      yyval.tenum = yyvsp[-1].tenum;
      yyval.tenum->append(yyvsp[0].tenumv);
    }
break;
case 39:
#line 561 "thrifty.yy"
	{
      pdebug("EnumDefList -> ");
      yyval.tenum = new t_enum(g_program);
    }
break;
case 40:
#line 568 "thrifty.yy"
	{
      pdebug("EnumDef -> tok_identifier = tok_int_constant");
      if (yyvsp[-2].iconst < 0) {
        pwarning(1, "Negative value supplied for enum %s.\n", yyvsp[-4].id);
      }
      if (yyvsp[-2].iconst > INT_MAX) {
        pwarning(1, "64-bit value supplied for enum %s.\n", yyvsp[-4].id);
      }
      yyval.tenumv = new t_enum_value(yyvsp[-4].id, yyvsp[-2].iconst);
      if (yyvsp[-5].dtext != NULL) {
        yyval.tenumv->set_doc(yyvsp[-5].dtext);
      }
      if (yyvsp[-1].ttype != NULL) {
        yyval.tenumv->annotations_ = yyvsp[-1].ttype->annotations_;
        delete yyvsp[-1].ttype;
      }
    }
break;
case 41:
#line 587 "thrifty.yy"
	{
      pdebug("EnumDef -> tok_identifier");
      yyval.tenumv = new t_enum_value(yyvsp[-2].id);
      if (yyvsp[-3].dtext != NULL) {
        yyval.tenumv->set_doc(yyvsp[-3].dtext);
      }
      if (yyvsp[-1].ttype != NULL) {
        yyval.tenumv->annotations_ = yyvsp[-1].ttype->annotations_;
        delete yyvsp[-1].ttype;
      }
    }
break;
case 42:
#line 601 "thrifty.yy"
	{
      pdebug("Senum -> tok_senum tok_identifier { SenumDefList }");
      yyval.ttypedef = new t_typedef(g_program, yyvsp[-2].tbase, yyvsp[-4].id);
      if (yyvsp[0].ttype != NULL) {
        yyval.ttypedef->annotations_ = yyvsp[0].ttype->annotations_;
        delete yyvsp[0].ttype;
      }
    }
break;
case 43:
#line 612 "thrifty.yy"
	{
      pdebug("SenumDefList -> SenumDefList SenumDef");
      yyval.tbase = yyvsp[-1].tbase;
      yyval.tbase->add_string_enum_val(yyvsp[0].id);
    }
break;
case 44:
#line 618 "thrifty.yy"
	{
      pdebug("SenumDefList -> ");
      yyval.tbase = new t_base_type("string", t_base_type::TYPE_STRING);
      yyval.tbase->set_string_enum(true);
    }
break;
case 45:
#line 626 "thrifty.yy"
	{
      pdebug("SenumDef -> tok_literal");
      yyval.id = yyvsp[-1].id;
    }
break;
case 46:
#line 633 "thrifty.yy"
	{
      pdebug("Const -> tok_const FieldType tok_identifier = ConstValue");
      if (g_parse_mode == PROGRAM) {
        g_scope->resolve_const_value(yyvsp[-1].tconstv, yyvsp[-4].ttype);
        yyval.tconst = new t_const(yyvsp[-4].ttype, yyvsp[-3].id, yyvsp[-1].tconstv);
        validate_const_type(yyval.tconst);

        g_scope->add_constant(yyvsp[-3].id, yyval.tconst);
        if (g_parent_scope != NULL) {
          g_parent_scope->add_constant(g_parent_prefix + yyvsp[-3].id, yyval.tconst);
        }
      } else {
        yyval.tconst = NULL;
      }
    }
break;
case 47:
#line 651 "thrifty.yy"
	{
      pdebug("ConstValue => tok_int_constant");
      yyval.tconstv = new t_const_value();
      yyval.tconstv->set_integer(yyvsp[0].iconst);
      if (!g_allow_64bit_consts && (yyvsp[0].iconst < INT32_MIN || yyvsp[0].iconst > INT32_MAX)) {
        pwarning(1, "64-bit constant \"%"PRIi64"\" may not work in all languages.\n", yyvsp[0].iconst);
      }
    }
break;
case 48:
#line 660 "thrifty.yy"
	{
      pdebug("ConstValue => tok_dub_constant");
      yyval.tconstv = new t_const_value();
      yyval.tconstv->set_double(yyvsp[0].dconst);
    }
break;
case 49:
#line 666 "thrifty.yy"
	{
      pdebug("ConstValue => tok_literal");
      yyval.tconstv = new t_const_value(yyvsp[0].id);
    }
break;
case 50:
#line 671 "thrifty.yy"
	{
      pdebug("ConstValue => tok_identifier");
      yyval.tconstv = new t_const_value();
      yyval.tconstv->set_identifier(yyvsp[0].id);
    }
break;
case 51:
#line 677 "thrifty.yy"
	{
      pdebug("ConstValue => ConstList");
      yyval.tconstv = yyvsp[0].tconstv;
    }
break;
case 52:
#line 682 "thrifty.yy"
	{
      pdebug("ConstValue => ConstMap");
      yyval.tconstv = yyvsp[0].tconstv;
    }
break;
case 53:
#line 689 "thrifty.yy"
	{
      pdebug("ConstList => [ ConstListContents ]");
      yyval.tconstv = yyvsp[-1].tconstv;
    }
break;
case 54:
#line 696 "thrifty.yy"
	{
      pdebug("ConstListContents => ConstListContents ConstValue CommaOrSemicolonOptional");
      yyval.tconstv = yyvsp[-2].tconstv;
      yyval.tconstv->add_list(yyvsp[-1].tconstv);
    }
break;
case 55:
#line 702 "thrifty.yy"
	{
      pdebug("ConstListContents =>");
      yyval.tconstv = new t_const_value();
      yyval.tconstv->set_list();
    }
break;
case 56:
#line 710 "thrifty.yy"
	{
      pdebug("ConstMap => { ConstMapContents }");
      yyval.tconstv = yyvsp[-1].tconstv;
    }
break;
case 57:
#line 717 "thrifty.yy"
	{
      pdebug("ConstMapContents => ConstMapContents ConstValue CommaOrSemicolonOptional");
      yyval.tconstv = yyvsp[-4].tconstv;
      yyval.tconstv->add_map(yyvsp[-3].tconstv, yyvsp[-1].tconstv);
    }
break;
case 58:
#line 723 "thrifty.yy"
	{
      pdebug("ConstMapContents =>");
      yyval.tconstv = new t_const_value();
      yyval.tconstv->set_map();
    }
break;
case 59:
#line 731 "thrifty.yy"
	{
      yyval.iconst = struct_is_struct;
    }
break;
case 60:
#line 735 "thrifty.yy"
	{
      yyval.iconst = struct_is_union;
    }
break;
case 61:
#line 741 "thrifty.yy"
	{
      pdebug("Struct -> tok_struct tok_identifier { FieldList }");
      yyvsp[-2].tstruct->set_xsd_all(yyvsp[-4].tbool);
      yyvsp[-2].tstruct->set_union(yyvsp[-6].iconst == struct_is_union);
      yyval.tstruct = yyvsp[-2].tstruct;
      yyval.tstruct->set_name(yyvsp[-5].id);
      if (yyvsp[0].ttype != NULL) {
        yyval.tstruct->annotations_ = yyvsp[0].ttype->annotations_;
        delete yyvsp[0].ttype;
      }
    }
break;
case 62:
#line 755 "thrifty.yy"
	{
      yyval.tbool = true;
    }
break;
case 63:
#line 759 "thrifty.yy"
	{
      yyval.tbool = false;
    }
break;
case 64:
#line 765 "thrifty.yy"
	{
      yyval.tbool = true;
    }
break;
case 65:
#line 769 "thrifty.yy"
	{
      yyval.tbool = false;
    }
break;
case 66:
#line 775 "thrifty.yy"
	{
      yyval.tbool = true;
    }
break;
case 67:
#line 779 "thrifty.yy"
	{
      yyval.tbool = false;
    }
break;
case 68:
#line 785 "thrifty.yy"
	{
      yyval.tstruct = yyvsp[-1].tstruct;
    }
break;
case 69:
#line 789 "thrifty.yy"
	{
      yyval.tstruct = NULL;
    }
break;
case 70:
#line 795 "thrifty.yy"
	{
      pdebug("Xception -> tok_xception tok_identifier { FieldList }");
      yyvsp[-2].tstruct->set_name(yyvsp[-4].id);
      yyvsp[-2].tstruct->set_xception(true);
      yyval.tstruct = yyvsp[-2].tstruct;
      if (yyvsp[0].ttype != NULL) {
        yyval.tstruct->annotations_ = yyvsp[0].ttype->annotations_;
        delete yyvsp[0].ttype;
      }
    }
break;
case 71:
#line 808 "thrifty.yy"
	{
      pdebug("Service -> tok_service tok_identifier { FunctionList }");
      yyval.tservice = yyvsp[-3].tservice;
      yyval.tservice->set_name(yyvsp[-7].id);
      yyval.tservice->set_extends(yyvsp[-6].tservice);
      if (yyvsp[0].ttype != NULL) {
        yyval.tservice->annotations_ = yyvsp[0].ttype->annotations_;
        delete yyvsp[0].ttype;
      }
    }
break;
case 72:
#line 820 "thrifty.yy"
	{
       g_arglist = 1;
    }
break;
case 73:
#line 825 "thrifty.yy"
	{
       g_arglist = 0;
    }
break;
case 74:
#line 831 "thrifty.yy"
	{
      pdebug("Extends -> tok_extends tok_identifier");
      yyval.tservice = NULL;
      if (g_parse_mode == PROGRAM) {
        yyval.tservice = g_scope->get_service(yyvsp[0].id);
        if (yyval.tservice == NULL) {
          yyerror("Service \"%s\" has not been defined.", yyvsp[0].id);
          exit(1);
        }
      }
    }
break;
case 75:
#line 843 "thrifty.yy"
	{
      yyval.tservice = NULL;
    }
break;
case 76:
#line 849 "thrifty.yy"
	{
      pdebug("FunctionList -> FunctionList Function");
      yyval.tservice = yyvsp[-1].tservice;
      yyvsp[-1].tservice->add_function(yyvsp[0].tfunction);
    }
break;
case 77:
#line 855 "thrifty.yy"
	{
      pdebug("FunctionList -> ");
      yyval.tservice = new t_service(g_program);
    }
break;
case 78:
#line 862 "thrifty.yy"
	{
      yyvsp[-4].tstruct->set_name(std::string(yyvsp[-6].id) + "_args");
      yyval.tfunction = new t_function(yyvsp[-7].ttype, yyvsp[-6].id, yyvsp[-4].tstruct, yyvsp[-2].tstruct, yyvsp[-8].tbool);
      if (yyvsp[-9].dtext != NULL) {
        yyval.tfunction->set_doc(yyvsp[-9].dtext);
      }
      if (yyvsp[-1].ttype != NULL) {
        yyval.tfunction->annotations_ = yyvsp[-1].ttype->annotations_;
        delete yyvsp[-1].ttype;
      }
    }
break;
case 79:
#line 876 "thrifty.yy"
	{
      yyval.tbool = true;
    }
break;
case 80:
#line 880 "thrifty.yy"
	{
      yyval.tbool = false;
    }
break;
case 81:
#line 886 "thrifty.yy"
	{
      pdebug("Throws -> tok_throws ( FieldList )");
      yyval.tstruct = yyvsp[-1].tstruct;
      if (g_parse_mode == PROGRAM && !validate_throws(yyval.tstruct)) {
        yyerror("Throws clause may not contain non-exception types");
        exit(1);
      }
    }
break;
case 82:
#line 895 "thrifty.yy"
	{
      yyval.tstruct = new t_struct(g_program);
    }
break;
case 83:
#line 901 "thrifty.yy"
	{
      pdebug("FieldList -> FieldList , Field");
      yyval.tstruct = yyvsp[-1].tstruct;
      if (!(yyval.tstruct->append(yyvsp[0].tfield))) {
        yyerror("Field identifier %d for \"%s\" has already been used", yyvsp[0].tfield->get_key(), yyvsp[0].tfield->get_name().c_str());
        exit(1);
      }
    }
break;
case 84:
#line 910 "thrifty.yy"
	{
      pdebug("FieldList -> ");
      y_field_val = -1;
      yyval.tstruct = new t_struct(g_program);
    }
break;
case 85:
#line 918 "thrifty.yy"
	{
      pdebug("tok_int_constant : Field -> FieldType tok_identifier");
      if (yyvsp[-9].tfieldid.auto_assigned) {
        pwarning(1, "No field key specified for %s, resulting protocol may have conflicts or not be backwards compatible!\n", yyvsp[-6].id);
        if (g_strict >= 192) {
          yyerror("Implicit field keys are deprecated and not allowed with -strict");
          exit(1);
        }
      }
      yyval.tfield = new t_field(yyvsp[-7].ttype, yyvsp[-6].id, yyvsp[-9].tfieldid.value);
      yyval.tfield->set_req(yyvsp[-8].ereq);
      if (yyvsp[-5].tconstv != NULL) {
        g_scope->resolve_const_value(yyvsp[-5].tconstv, yyvsp[-7].ttype);
        validate_field_value(yyval.tfield, yyvsp[-5].tconstv);
        yyval.tfield->set_value(yyvsp[-5].tconstv);
      }
      yyval.tfield->set_xsd_optional(yyvsp[-4].tbool);
      yyval.tfield->set_xsd_nillable(yyvsp[-3].tbool);
      if (yyvsp[-10].dtext != NULL) {
        yyval.tfield->set_doc(yyvsp[-10].dtext);
      }
      if (yyvsp[-2].tstruct != NULL) {
        yyval.tfield->set_xsd_attrs(yyvsp[-2].tstruct);
      }
      if (yyvsp[-1].ttype != NULL) {
        yyval.tfield->annotations_ = yyvsp[-1].ttype->annotations_;
        delete yyvsp[-1].ttype;
      }
    }
break;
case 86:
#line 950 "thrifty.yy"
	{
      if (yyvsp[-1].iconst <= 0) {
        if (g_allow_neg_field_keys) {
          /*
           * g_allow_neg_field_keys exists to allow users to add explicitly
           * specified key values to old .thrift files without breaking
           * protocol compatibility.
           */
          if (yyvsp[-1].iconst != y_field_val) {
            /*
             * warn if the user-specified negative value isn't what
             * thrift would have auto-assigned.
             */
            pwarning(1, "Negative field key (%d) differs from what would be "
                     "auto-assigned by thrift (%d).\n", yyvsp[-1].iconst, y_field_val);
          }
          /*
           * Leave $1 as-is, and update y_field_val to be one less than $1.
           * The FieldList parsing will catch any duplicate key values.
           */
          y_field_val = yyvsp[-1].iconst - 1;
          yyval.tfieldid.value = yyvsp[-1].iconst;
          yyval.tfieldid.auto_assigned = false;
        } else {
          pwarning(1, "Nonpositive value (%d) not allowed as a field key.\n",
                   yyvsp[-1].iconst);
          yyval.tfieldid.value = y_field_val--;
          yyval.tfieldid.auto_assigned = true;
        }
      } else {
        yyval.tfieldid.value = yyvsp[-1].iconst;
        yyval.tfieldid.auto_assigned = false;
      }
    }
break;
case 87:
#line 985 "thrifty.yy"
	{
      yyval.tfieldid.value = y_field_val--;
      yyval.tfieldid.auto_assigned = true;
    }
break;
case 88:
#line 992 "thrifty.yy"
	{
      yyval.ereq = t_field::T_REQUIRED;
    }
break;
case 89:
#line 996 "thrifty.yy"
	{
      if (g_arglist) {
        if (g_parse_mode == PROGRAM) {
          pwarning(1, "optional keyword is ignored in argument lists.\n");
        }
        yyval.ereq = t_field::T_OPT_IN_REQ_OUT;
      } else {
        yyval.ereq = t_field::T_OPTIONAL;
      }
    }
break;
case 90:
#line 1007 "thrifty.yy"
	{
      yyval.ereq = t_field::T_OPT_IN_REQ_OUT;
    }
break;
case 91:
#line 1013 "thrifty.yy"
	{
      if (g_parse_mode == PROGRAM) {
        yyval.tconstv = yyvsp[0].tconstv;
      } else {
        yyval.tconstv = NULL;
      }
    }
break;
case 92:
#line 1021 "thrifty.yy"
	{
      yyval.tconstv = NULL;
    }
break;
case 93:
#line 1027 "thrifty.yy"
	{
      pdebug("FunctionType -> FieldType");
      yyval.ttype = yyvsp[0].ttype;
    }
break;
case 94:
#line 1032 "thrifty.yy"
	{
      pdebug("FunctionType -> tok_void");
      yyval.ttype = g_type_void;
    }
break;
case 95:
#line 1039 "thrifty.yy"
	{
      pdebug("FieldType -> tok_identifier");
      if (g_parse_mode == INCLUDES) {
        /* Ignore identifiers in include mode*/
        yyval.ttype = NULL;
      } else {
        /* Lookup the identifier in the current scope*/
        yyval.ttype = g_scope->get_type(yyvsp[0].id);
        if (yyval.ttype == NULL) {
          yyerror("Type \"%s\" has not been defined.", yyvsp[0].id);
          exit(1);
        }
      }
    }
break;
case 96:
#line 1054 "thrifty.yy"
	{
      pdebug("FieldType -> BaseType");
      yyval.ttype = yyvsp[0].ttype;
    }
break;
case 97:
#line 1059 "thrifty.yy"
	{
      pdebug("FieldType -> ContainerType");
      yyval.ttype = yyvsp[0].ttype;
    }
break;
case 98:
#line 1065 "thrifty.yy"
	{
      pdebug("BaseType -> SimpleBaseType TypeAnnotations");
      if (yyvsp[0].ttype != NULL) {
        yyval.ttype = new t_base_type(*static_cast<t_base_type*>(yyvsp[-1].ttype));
        yyval.ttype->annotations_ = yyvsp[0].ttype->annotations_;
        delete yyvsp[0].ttype;
      } else {
        yyval.ttype = yyvsp[-1].ttype;
      }
    }
break;
case 99:
#line 1078 "thrifty.yy"
	{
      pdebug("BaseType -> tok_string");
      yyval.ttype = g_type_string;
    }
break;
case 100:
#line 1083 "thrifty.yy"
	{
      pdebug("BaseType -> tok_binary");
      yyval.ttype = g_type_binary;
    }
break;
case 101:
#line 1088 "thrifty.yy"
	{
      pdebug("BaseType -> tok_slist");
      yyval.ttype = g_type_slist;
    }
break;
case 102:
#line 1093 "thrifty.yy"
	{
      pdebug("BaseType -> tok_bool");
      yyval.ttype = g_type_bool;
    }
break;
case 103:
#line 1098 "thrifty.yy"
	{
      pdebug("BaseType -> tok_byte");
      yyval.ttype = g_type_byte;
    }
break;
case 104:
#line 1103 "thrifty.yy"
	{
      pdebug("BaseType -> tok_i16");
      yyval.ttype = g_type_i16;
    }
break;
case 105:
#line 1108 "thrifty.yy"
	{
      pdebug("BaseType -> tok_i32");
      yyval.ttype = g_type_i32;
    }
break;
case 106:
#line 1113 "thrifty.yy"
	{
      pdebug("BaseType -> tok_i64");
      yyval.ttype = g_type_i64;
    }
break;
case 107:
#line 1118 "thrifty.yy"
	{
      pdebug("BaseType -> tok_double");
      yyval.ttype = g_type_double;
    }
break;
case 108:
#line 1124 "thrifty.yy"
	{
      pdebug("ContainerType -> SimpleContainerType TypeAnnotations");
      yyval.ttype = yyvsp[-1].ttype;
      if (yyvsp[0].ttype != NULL) {
        yyval.ttype->annotations_ = yyvsp[0].ttype->annotations_;
        delete yyvsp[0].ttype;
      }
    }
break;
case 109:
#line 1135 "thrifty.yy"
	{
      pdebug("SimpleContainerType -> MapType");
      yyval.ttype = yyvsp[0].ttype;
    }
break;
case 110:
#line 1140 "thrifty.yy"
	{
      pdebug("SimpleContainerType -> SetType");
      yyval.ttype = yyvsp[0].ttype;
    }
break;
case 111:
#line 1145 "thrifty.yy"
	{
      pdebug("SimpleContainerType -> ListType");
      yyval.ttype = yyvsp[0].ttype;
    }
break;
case 112:
#line 1152 "thrifty.yy"
	{
      pdebug("MapType -> tok_map <FieldType, FieldType>");
      yyval.ttype = new t_map(yyvsp[-3].ttype, yyvsp[-1].ttype);
      if (yyvsp[-5].id != NULL) {
        ((t_container*)yyval.ttype)->set_cpp_name(std::string(yyvsp[-5].id));
      }
    }
break;
case 113:
#line 1162 "thrifty.yy"
	{
      pdebug("SetType -> tok_set<FieldType>");
      yyval.ttype = new t_set(yyvsp[-1].ttype);
      if (yyvsp[-3].id != NULL) {
        ((t_container*)yyval.ttype)->set_cpp_name(std::string(yyvsp[-3].id));
      }
    }
break;
case 114:
#line 1172 "thrifty.yy"
	{
      pdebug("ListType -> tok_list<FieldType>");
      yyval.ttype = new t_list(yyvsp[-2].ttype);
      if (yyvsp[0].id != NULL) {
        ((t_container*)yyval.ttype)->set_cpp_name(std::string(yyvsp[0].id));
      }
    }
break;
case 115:
#line 1182 "thrifty.yy"
	{
      yyval.id = yyvsp[0].id;
    }
break;
case 116:
#line 1186 "thrifty.yy"
	{
      yyval.id = NULL;
    }
break;
case 117:
#line 1192 "thrifty.yy"
	{
      pdebug("TypeAnnotations -> ( TypeAnnotationList )");
      yyval.ttype = yyvsp[-1].ttype;
    }
break;
case 118:
#line 1197 "thrifty.yy"
	{
      yyval.ttype = NULL;
    }
break;
case 119:
#line 1203 "thrifty.yy"
	{
      pdebug("TypeAnnotationList -> TypeAnnotationList , TypeAnnotation");
      yyval.ttype = yyvsp[-1].ttype;
      yyval.ttype->annotations_[yyvsp[0].tannot->key] = yyvsp[0].tannot->val;
      delete yyvsp[0].tannot;
    }
break;
case 120:
#line 1210 "thrifty.yy"
	{
      /* Just use a dummy structure to hold the annotations. */
      yyval.ttype = new t_struct(g_program);
    }
break;
case 121:
#line 1217 "thrifty.yy"
	{
      pdebug("TypeAnnotation -> tok_identifier = tok_literal");
      yyval.tannot = new t_annotation;
      yyval.tannot->key = yyvsp[-3].id;
      yyval.tannot->val = yyvsp[-1].id;
    }
break;
#line 1909 "thrifty.cc"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = (short) yystate;
    *++yyvsp = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    return (1);

yyaccept:
    return (0);
}
