#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct text_t {
	int        key;
    struct text_t  *left;
    struct text_t *right;
    int           height;
} text_t;


text_t *create_text() {
	text_t *tmp_node;
	tmp_node = (text_t *) malloc(sizeof(text_t));
	tmp_node->right = NULL;
	tmp_node->left = NULL;
	tmp_node->height = 0;
	tmp_node->key = 0;
	return( tmp_node );
}

int length_text(text_t *txt){
	if (txt == NULL)
		return -1;
	if (txt->left == NULL)
		return 0;
	return txt->key;
}

void left_rotation(text_t *n) {
	text_t *tmp_node;
    tmp_node = n->left;
    n->left  = n->right;
    n->right = n->left->right;
    n->left->right = n->left->left;
    n->left->left  = tmp_node;
    if(n->left->right == NULL)
    	n->left->key = 1;
    else
    	n->left->key = n->left->left->key + n->left->right->key;
    if (n->right->right == NULL)
    	n->right->key = 1;
    else
    	n->right->key = n->right->left->key + n->right->right->key;
}

void right_rotation(text_t *n){
	text_t *tmp_node;
    tmp_node = n->right;
    n->right = n->left;
    n->left  = n->right->left;
    n->right->left = n->right->right;
    n->right->right  = tmp_node;
    if(n->left->right == NULL)
    	n->left->key = 1;
    else
    	n->left->key = n->left->left->key + n->left->right->key;
    if (n->right->right == NULL)
    	n->right->key = 1;
    else
    	n->right->key = n->right->left->key + n->right->right->key;
}

char *get_line(text_t *tree, int index) {
	text_t *tmp_node;
	if( tree->left == NULL )
		return(NULL);
	else {
		tmp_node = tree;
		while( tmp_node->right != NULL ) {
			if( index <= tmp_node->left->key ){
				tmp_node = tmp_node->left;
			} else {
				index -= tmp_node->left->key;
				tmp_node = tmp_node->right;
			}
		}
		return( (char  *) tmp_node->left );
	}
}

char *set_line(text_t *tree, int index, char  *new_line){
	text_t *tmp_node;
	if(tree->left == NULL)
		return(NULL);
	else {
		tmp_node = tree;
		while( tmp_node->right != NULL ) {
			if( index <= tmp_node->left->key ){
				tmp_node = tmp_node->left;
			} else {
				index -= tmp_node->left->key;
				tmp_node = tmp_node->right;
			}
		}
		char *ret = strdup( (char  *) tmp_node->left);
		tmp_node->left = (text_t *) new_line;
		return ret;
	}
}

void append_line(text_t *tree, char *new_line){
	text_t *tmp_node;
	int finished;
	if( tree->left == NULL ) {
		tree->left = (text_t *) new_line;
		tree->key  = 1;
		tree->height = 0;
		tree->right  = NULL;
	} else {
		text_t * path_stack[100]; int  stack_p = 0;
		tmp_node = tree;
		while( tmp_node->right != NULL ) {
			tmp_node->key += 1;
			path_stack[stack_p++] = tmp_node;
			tmp_node = tmp_node->right;
		}
		text_t *old_leaf, *new_leaf;
		old_leaf = create_text();
		old_leaf->left = tmp_node->left;
		old_leaf->key = tmp_node->key;
		old_leaf->right  = NULL;
		old_leaf->height = 0;
		new_leaf = create_text();
		new_leaf->left = (text_t *) new_line;
		new_leaf->key = 1;
		new_leaf->right  = NULL;
		new_leaf->height = 0;
		tmp_node->left  = old_leaf;
		tmp_node->right = new_leaf;
		tmp_node->key += 1;
		tmp_node->height = 1;

		/* rebalance */
		finished = 0;
		while( stack_p > 0 && !finished ){
			int tmp_height, old_height;
			tmp_node = path_stack[--stack_p];
			old_height= tmp_node->height;
			if( tmp_node->left->height - tmp_node->right->height == 2 ) {
				if( tmp_node->left->left->height - tmp_node->right->height == 1 ) {
					right_rotation( tmp_node );
					tmp_node->right->height = tmp_node->right->left->height + 1;
					tmp_node->height = tmp_node->right->height + 1;
				} else {
					left_rotation( tmp_node->left );
					right_rotation( tmp_node );
					tmp_height = tmp_node->left->left->height;
					tmp_node->left->height  = tmp_height + 1;
					tmp_node->right->height = tmp_height + 1;
					tmp_node->height = tmp_height + 2;
				}
			} else if ( tmp_node->left->height - tmp_node->right->height == -2 ) {
				if( tmp_node->right->right->height - tmp_node->left->height == 1 ) {
					left_rotation( tmp_node );
					tmp_node->left->height = tmp_node->left->right->height + 1;
					tmp_node->height = tmp_node->left->height + 1;
				} else {
					right_rotation( tmp_node->right );
					left_rotation( tmp_node );
					tmp_height = tmp_node->right->right->height;
					tmp_node->left->height  = tmp_height + 1;
					tmp_node->right->height = tmp_height + 1;
					tmp_node->height = tmp_height + 2;
				}
			} else {/* update height even if there was no rotation */
				if( tmp_node->left->height > tmp_node->right->height )
					tmp_node->height = tmp_node->left->height + 1;
				else
					tmp_node->height = tmp_node->right->height + 1;
			}
			if( tmp_node->height == old_height )
				finished = 1;
		}
   }
}

void insert_line(text_t *tree, int index, char  *new_object){
	text_t *tmp_node;
    int finished;
    if( tree->left == NULL ) {
	    tree->left = (text_t *) new_object;
	    tree->key  = 1;
	    tree->height = 0;
	    tree->right  = NULL;
    } else {
    	int i = index - 1;
	    text_t * path_stack[100]; int stack_p = 0;
        tmp_node = tree;
        while( tmp_node->right != NULL ) {
     	    path_stack[stack_p++] = tmp_node;
            if( i <= tmp_node->left->key ){
            	tmp_node->key += 1;
            	tmp_node = tmp_node->left;
            } else{
            	tmp_node->key += 1;
            	i -= tmp_node->left->key;
            	tmp_node = tmp_node->right;
            }
        }
        text_t *old_leaf, *new_leaf;
        old_leaf = create_text();
        old_leaf->left = tmp_node->left;
        old_leaf->key = tmp_node->key;
        old_leaf->right  = NULL;
        old_leaf->height = 0;
        new_leaf = create_text();
        new_leaf->left = (text_t *) new_object;
        new_leaf->key = 1;
        new_leaf->right  = NULL;
        new_leaf->height = 0;
        tmp_node->left  = old_leaf;
        tmp_node->right = new_leaf;
        tmp_node->key += 1;
        tmp_node->height = 1;

		finished = 0;
		while( stack_p > 0 && !finished ){
			int tmp_height, old_height;
			tmp_node = path_stack[--stack_p];
			old_height= tmp_node->height;
			if( tmp_node->left->height - tmp_node->right->height == 2 ) {
				if( tmp_node->left->left->height - tmp_node->right->height == 1 ) {
					right_rotation( tmp_node );
					tmp_node->right->height = tmp_node->right->left->height + 1;
					tmp_node->height = tmp_node->right->height + 1;
				} else {
					left_rotation( tmp_node->left );
					right_rotation( tmp_node );
					tmp_height = tmp_node->left->left->height;
					tmp_node->left->height  = tmp_height + 1;
					tmp_node->right->height = tmp_height + 1;
					tmp_node->height = tmp_height + 2;
				}
			} else if ( tmp_node->left->height - tmp_node->right->height == -2 ) {
				if( tmp_node->right->right->height - tmp_node->left->height == 1 ) {
					left_rotation( tmp_node );
					tmp_node->left->height = tmp_node->left->right->height + 1;
					tmp_node->height = tmp_node->left->height + 1;
				} else {
					right_rotation( tmp_node->right );
					left_rotation( tmp_node );
					tmp_height = tmp_node->right->right->height;
					tmp_node->left->height  = tmp_height + 1;
					tmp_node->right->height = tmp_height + 1;
					tmp_node->height = tmp_height + 2;
				}
			} else {/* update height even if there was no rotation */
				if( tmp_node->left->height > tmp_node->right->height )
					tmp_node->height = tmp_node->left->height + 1;
				else
					tmp_node->height = tmp_node->right->height + 1;
			}
			if( tmp_node->height == old_height )
				finished = 1;
		}
   }
}

char  *delete_line(text_t *tree, int index){
	text_t *tmp_node, *upper_node, *other_node;
	char  *deleted_object; int finished;
	if( tree->left == NULL )
		return( NULL );
	else if( tree->right == NULL ){
		deleted_object = (char  *) tree->left;
		tree->left = NULL;
		return( deleted_object );
	} else {
		text_t * path_stack[100]; int stack_p = 0;
		tmp_node = tree;
		while( tmp_node->right != NULL ) {
			path_stack[stack_p++] = tmp_node;
			upper_node = tmp_node;
			if(index <= tmp_node->left->key){
				tmp_node->key -= 1;
				tmp_node   = upper_node->left;
				other_node = upper_node->right;
			} else {
				index -= tmp_node->left->key;
				tmp_node->key -= 1;
				tmp_node   = upper_node->right;
				other_node = upper_node->left;
			}
		}
		upper_node->key   = other_node->key;
        upper_node->left  = other_node->left;
        upper_node->right = other_node->right;
        upper_node->height = other_node->height;
        deleted_object = (char *) tmp_node->left;

        free(tmp_node);
        other_node->left = NULL;
        other_node->right = NULL;
        free(other_node);

        /*start rebalance*/
        finished = 0; stack_p -= 1;
        while( stack_p > 0 && !finished ){
        	int tmp_height, old_height;
        	tmp_node = path_stack[--stack_p];
        	old_height= tmp_node->height;
        	if( tmp_node->left->height - tmp_node->right->height == 2 ) {
        		if( tmp_node->left->left->height - tmp_node->right->height == 1 ) {
        			right_rotation( tmp_node );
        			tmp_node->right->height = tmp_node->right->left->height + 1;
        			tmp_node->height = tmp_node->right->height + 1;
        		} else {
        			left_rotation( tmp_node->left );
        			right_rotation( tmp_node );
        			tmp_height = tmp_node->left->left->height;
        			tmp_node->left->height  = tmp_height + 1;
        			tmp_node->right->height = tmp_height + 1;
        			tmp_node->height = tmp_height + 2;
        		}
        	} else if ( tmp_node->left->height - tmp_node->right->height == -2 ) {
        		if( tmp_node->right->right->height - tmp_node->left->height == 1 ) {
        			left_rotation( tmp_node );
        			tmp_node->left->height = tmp_node->left->right->height + 1;
        			tmp_node->height = tmp_node->left->height + 1;
        		} else {
        			right_rotation( tmp_node->right );
        			left_rotation( tmp_node );
        			tmp_height = tmp_node->right->right->height;
        			tmp_node->left->height  = tmp_height + 1;
        			tmp_node->right->height = tmp_height + 1;
        			tmp_node->height = tmp_height + 2;
        		}
        	} else {/* update height even if there was no rotation */
        		if( tmp_node->left->height > tmp_node->right->height )
        			tmp_node->height = tmp_node->left->height + 1;
        		else
        			tmp_node->height = tmp_node->right->height + 1;
        	}
        	if( tmp_node->height == old_height )
        		finished = 1;
        }
        /*end rebalance*/
        return( deleted_object );
	}
}
