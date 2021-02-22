#ifndef kode_tiny_graph_included
#define kode_tiny_graph_included
//----------------------------------------------------------------------

#include "base/kode.h"

//#include "s4k_data.h"
//#include "s4k_expr.h"

//----------------------------------------------------------------------

#define kode_tiny_graph_nodetype_empty      0
#define kode_tiny_graph_nodetype_noop       1
#define kode_tiny_graph_nodetype_proc       2
#define kode_tiny_graph_nodetype_expr       3
#define kode_tiny_graph_nodetype_cond       4
#define kode_tiny_graph_nodetype_repeat     5
#define kode_tiny_graph_nodetype_loc        6
#define kode_tiny_graph_nodetype_call       7
#define kode_tiny_graph_nodetype_cycle      8
#define kode_tiny_graph_nodetype_pattern    9
#define kode_tiny_graph_nodetype_wait       10
#define kode_tiny_graph_nodetype_poly       11
#define kode_tiny_graph_nodetype_voice      12

//----------------------------------------------------------------------

typedef void (*kode_tiny_graph_proc_t)(uint32 node);

uint32_t kode_tiny_graph_graph_loc[256];   // setup in graph_init()
uint8_t  kode_tiny_graph_node_state[256];  // 0

//----------

uint8_t  kode_tiny_graph_node_type[256];
uint8_t  kode_tiny_graph_node_arg[256];
uint8_t  kode_tiny_graph_node_bro[256];  // Sibling
uint8_t  kode_tiny_graph_node_son[256];  // FirstChild

kode_tiny_graph_proc_t kode_tiny_graph_graph_proc[256];
uint8_t* kode_tiny_graph_graph_list[256]; // todo: 256*256 uint8
//uint8_t* kode_tiny_graph_graph_patt[256]; // todo: 256*256 uint8

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void kode_tiny_graph_graph_init(void);
void kode_tiny_graph_graph_parse(uint32_t self);
void kode_tiny_graph_graph_parse_child(uint32_t self, uint32_t child);
void kode_tiny_graph_graph_parse_children(uint32_t self);

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void kode_tiny_graph_graph_init(void)
{
  uint32_t i;
  for (i=0; i<256; i++)
  {
    if (kode_tiny_graph_node_type[i] == kode_tiny_graph_nodetype_loc) { kode_tiny_graph_graph_loc[ kode_tiny_graph_node_arg[i] ] = i; }
    //kode_tiny_graph_node_state[i] = 0;
  }
}

//----------

void kode_tiny_graph_graph_parse_child(uint32_t self, uint32_t child)
{
  uint32_t i;
  uint32_t node;

  i = 0;
  node = kode_tiny_graph_node_son[self];
  while (node)
  {
    if (i==child) kode_tiny_graph_graph_parse(node);
    i++;
    node = kode_tiny_graph_node_bro[node];
  }
}

//----------

void kode_tiny_graph_graph_parse_children(uint32 self)
{
  uint32_t node;
  node = kode_tiny_graph_node_son[self];
  while (node)
  {
    kode_tiny_graph_graph_parse(node);
    node = kode_tiny_graph_node_bro[node];
  }
}

//----------

void kode_tiny_graph_graph_parse(uint32 self)
{
  uint32_t op;
  uint32_t val;

  //printf("graph parse: %i\n",self);
  op = kode_tiny_graph_node_type[self];
  val = kode_tiny_graph_node_arg[self];
  switch(op)
  {

    //case kode_tiny_graph_node_empty:
    //  break;

    case kode_tiny_graph_nodetype_noop:

      //printf("yo!\n");
      kode_tiny_graph_graph_parse_children(self);
      break;

    case kode_tiny_graph_nodetype_proc:

      kode_tiny_graph_graph_proc[val](self);
      kode_tiny_graph_graph_parse_children(self);
      break;

    case kode_tiny_graph_nodetype_expr:

      //kode_tiny_graph_expr_evaluate( &kode_tiny_graph_list[val*256], false );
      //kode_tiny_graph_graph_parse_children(self);
      break;

    case kode_tiny_graph_nodetype_cond:

      //if (kode_tiny_graph_expr_evaluate( &kode_tiny_graph_list[val*256], true ) == true ) kode_tiny_graph_graph_parse_children(self);
      break;

    case kode_tiny_graph_nodetype_repeat:

      while (val-- > 0) kode_tiny_graph_graph_parse_children(self);
      break;

    case kode_tiny_graph_nodetype_loc:

      //graph_loc[val] = self;
      kode_tiny_graph_graph_parse_children(self);
      break;

    case kode_tiny_graph_nodetype_call:

      kode_tiny_graph_graph_parse( kode_tiny_graph_graph_loc[val] );
      kode_tiny_graph_graph_parse_children(self);
      break;

    case kode_tiny_graph_nodetype_wait:

      if ( kode_tiny_graph_node_state[self] == 0 ) kode_tiny_graph_graph_parse_children(self);
      kode_tiny_graph_node_state[self]++;
      if ( kode_tiny_graph_node_state[self] >= val ) kode_tiny_graph_node_state[self] = 0;
      break;

    case kode_tiny_graph_nodetype_cycle:

      kode_tiny_graph_graph_parse_child( self, kode_tiny_graph_node_state[self] );
      kode_tiny_graph_node_state[self]++;
      if ( kode_tiny_graph_node_state[self] >= val ) kode_tiny_graph_node_state[self] = 0;
      break;

    case kode_tiny_graph_nodetype_pattern:

      //kode_tiny_graph_graph_parse_child( self, kode_tiny_graph_graph_list[ kode_tiny_graph_node_state[self] ] );
      //kode_tiny_graph_node_state[self]++;
      //if ( kode_tiny_graph_node_state[self] >= val ) kode_tiny_graph_node_state[self] = 0;
      break;

  } // switch op
}

//----------------------------------------------------------------------
#endif