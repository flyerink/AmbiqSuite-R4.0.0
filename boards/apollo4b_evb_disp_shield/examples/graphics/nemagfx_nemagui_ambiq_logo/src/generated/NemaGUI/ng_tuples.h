#ifndef NG_TUPLES_H__
#define NG_TUPLES_H__

#include "ng_gitem.h"
#include "ng_animation.h"
#include "nema_core.h"
#include "nema_easing.h"
#include "nema_utils.h"


typedef struct _ng_point_t{
    int x; 
    int y;
} ng_point_t;

typedef struct _ng_git_uint32_t{
    gitem_base_t *git; 
    uint32_t val;
} ng_git_uint32_t;

typedef struct _ng_gitem_uint_uint_ez_t{
    gitem_base_t *git; 
    uint32_t val_0;
    uint32_t val_1;
    float (*easing)(float p);
} ng_git_uint32_uint32_ez_t;

typedef struct _ng_git_ptr_t{
    gitem_base_t *git; 
    void    *ptr;
} ng_git_ptr_t;

typedef struct _ng_git_float_t{
    gitem_base_t *git;
    float val;
} ng_git_float_t;

typedef struct _ng_gitem_float_float_ez_t{
    gitem_base_t *git; 
    float val_0;
    float val_1;
    float (*easing)(float p);
} ng_git_float_float_ez_t;


typedef struct _ng_git_int_int_t{
    gitem_base_t *git;
    int a;
    int b;
} ng_git_int_int_t;


typedef struct _ng_git_int_int_pair_ez_t{
    gitem_base_t *git; 
    int a0;
    int a1;
    int b0;
    int b1;
    float (*easing)(float p);
} ng_git_int_int_pair_ez_t;

typedef struct _ng_node_effect_direction_t {
    tree_node_t *node;
    nema_transition_t effect;
    ng_animation_direction_e direction; 
} ng_node_effect_direction_t;

typedef struct _ng_node_node_t {
    tree_node_t *node0;
    tree_node_t *node1;
} ng_node_node_t;

//Convenience types
//--------------------------------------------------
typedef struct _ng_gitptr_t {
    gitem_base_t *git;
} ng_gitptr_t;

typedef struct _ng_nodeptr_t {
    tree_node_t *node;
} ng_tree_node_ptr_t;
//---------------------------------------------------

#endif // NG_TUPLES_H__
