// Copyright (C) 2010. sparkling.liang@hotmail.com. All rights reserved.

#ifndef __CON_HASH_H_
#define __CON_HASH_H_

#include "configure.h"
#include <stdlib.h>

// node is initialized
#define NODE_FLAG_INIT  0x01
// node is added in the server
#define NODE_FLAG_IN    0x02

// nodes structure
struct node_s {
	char iden[64];   // node name or some thing identifies the node
  u_int replicas;  // number of replica virtual nodes
  u_int flag;
};

// callback function to calculate hash value
// @instr: input string
typedef long (*conhash_cb_hashfunc)(const char *instr);
struct conhash_s;

// initialize conhash library
// @pfhash : hash function, NULL to use default MD5 method
// return a conhash_s instance
struct conhash_s* conhash_init(conhash_cb_hashfunc pfhash);

// finalize lib
void conhash_fini(struct conhash_s *conhash);

// set node
void conhash_set_node(struct node_s *node, const char *iden, u_int replica);

// add a new node
// @node: the node to add
int conhash_add_node(struct conhash_s *conhash, struct node_s *node);

// remove a node
int conhash_del_node(struct conhash_s *conhash, struct node_s *node);

// update a node's virtual nodes
// @replica: new replica of server
// return 0 success, -1 failed
int conhash_update_node(struct conhash_s *conhash,
                        struct node_s *node, u_int replica);

// lookup a server which object belongs to
// @object: the input string which indicates an object
// return the server_s structure, do not modify the value,
// or it will cause a disaster
const struct node_s* conhash_lookup(const struct conhash_s *conhash,
                                    const char *object);

// some utility functions export
void  conhash_md5_digest(const u_char *instr, u_char digest[16]);
// get virtual node number in the hash
u_int conhash_get_vnodes_num(const struct conhash_s *conhash);

// get virtual nodes in ascending oder
// @values, pointer to an array, stores all the nodes's hash value
// @size, how many nodes to get, can't be less than the array size
void  conhash_get_vnodes(const struct conhash_s *conhash,
                         long *values, int size);

#endif // end __CON_HASH_H_
