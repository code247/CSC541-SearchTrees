#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "API.h"

typedef int index;
typedef struct text_t {
	index      index;
    struct text_t   *left;
    struct text_t  *right;
    int length;
} text_t;


#define BLOCKSIZE 256

int numLines = 0;
text_t *currentblock = NULL;
int    size_left;
text_t *free_list = NULL;
int nodes_taken = 0;
int nodes_returned = 0;

text_t *get_node() {
  return (text_t *) malloc(BLOCKSIZE * sizeof(text_t));
}

text_t * create_text(){
	text_t *tmp_node;
	tmp_node = get_node();
	tmp_node->left = NULL;
	return( tmp_node );
}

int length_text( text_t *txt){
	return txt->length;
}

char * get_line( text_t *txt, int index) {
	text_t *tmp_node;
	if(txt->left == NULL)
		return(NULL);
	else {
		tmp_node = txt;
		while( tmp_node->right != NULL ){
			if(index < tmp_node->index )
				tmp_node = tmp_node->left;
			else
			   tmp_node = tmp_node->right;
		}
		if( tmp_node->index == index )
			return( (char *) tmp_node->left );
		else
			return( NULL );
	}
}

void append_line( text_t *txt, char * new_line) {
	text_t *tmp_node;
	if( txt->left == NULL ) {
		txt->left = (text_t *) new_line;
		txt->index  = 1;
		txt->right  = NULL;
		txt->length = strlen(new_line);
	} else {
		tmp_node = txt;
		while( tmp_node->right != NULL ) {
			tmp_node = tmp_node->right;
		}
	    text_t *old_leaf, *new_leaf;
		old_leaf = get_node();
		old_leaf->left = tmp_node->left;
		old_leaf->index = tmp_node->index;
		old_leaf->right  = NULL;
		old_leaf->length = tmp_node->length;
		new_leaf = get_node();
		new_leaf->left = (text_t *) new_line;
		new_leaf->index = tmp_node->index + 1;
		new_leaf->right  = NULL;
		new_leaf->length = strlen(new_line);

		tmp_node->left  = old_leaf;
		tmp_node->right = new_leaf;
		tmp_node->index += 1;
		tmp_node->length += tmp_node->length + strlen(new_line);
	}
	numLines += 1;
	return;
}

char * set_line( text_t *txt, int index, char * new_line){
	text_t *tmp_node;
	if(txt->left == NULL)
		return(NULL);
	else {
		tmp_node = txt;
		while( tmp_node->right != NULL ){
			if(index < tmp_node->index )
				tmp_node = tmp_node->left;
			else
			   tmp_node = tmp_node->right;
		}
		if( tmp_node->index == index ){
			char * prev = (char *) tmp_node->left;
			tmp_node->left = (text_t *) new_line;
			return prev;
		} else
			return NULL;
	}
}

void insert_line( text_t *txt, int index, char * new_line) {
	text_t *tmp_node;
	if(txt->left == NULL || numLines < index)
		append_line(txt, new_line);
	else {
		int i = numLines;
		char *tmp = set_line(txt, i, new_line);
		i++;
		while(i <= numLines){
			char *tmp = set_line(txt, i, tmp);
		}
		append_line(txt, tmp);
//		tmp_node = txt;
//		while( tmp_node->right != NULL ){
//			if(index < tmp_node->index )
//				tmp_node = tmp_node->left;
//			else
//			   tmp_node = tmp_node->right;
//		}
//		if( tmp_node->index == index ){
//			text_t *old_leaf, *new_leaf;
//			old_leaf = get_node();
//			old_leaf->left = tmp_node->left;
//			old_leaf->index = index + 1;
//			old_leaf->right = NULL;
//			old_leaf->length = tmp_node->length;
//			new_leaf = get_node();
//			new_leaf->left = (text_t *) new_line;
//			new_leaf->index = index;
//			new_leaf->right = NULL;
//			new_leaf->length = strlen(new_line);
//
//			tmp_node->left = new_leaf;
//			tmp_node->right = old_leaf;
//			tmp_node->index = index + 1;
//			tmp_node->length += tmp_node->length + strlen(new_line);
//		}
	}
}

char * delete_line( text_t *txt, int index) {
	text_t *tmp_node, *upper_node, *other_node;
	char *deleted_line;
	if(txt->left == NULL)
		return( NULL );
	else if(txt->right == NULL) {
		if(txt->index == index) {
			deleted_line = (char *) txt->left;
			txt->left = NULL;
			return(deleted_line);
		}
		else
			return( NULL );
	} else {
		int i = index;
		char *c = get_line(txt, i + 1);
		char *t = set_line(txt, i, c);
		i++;
		while(i < numLines){
			char *nxt = get_line(txt, i + 1);
			char *tmp = set_line(txt, i);
			i++;
		}
		tmp_node = txt;
		while(tmp_node->right != NULL) {
			upper_node = tmp_node;
		    if(numLines < tmp_node->index){
		    	tmp_node   = upper_node->left;
		    	other_node = upper_node->right;
		    } else {
		    	tmp_node   = upper_node->right;
		    	other_node = upper_node->left;
		    }
	  }
	  if(tmp_node->index != numLines)
		  return( NULL );
	  else {
		  upper_node->index   = other_node->index;
		  upper_node->left  = other_node->left;
		  upper_node->right = other_node->right;
		  deleted_line = (char *) tmp_node->left;
		  numLines -= 1;
		  return(deleted_line);
	  }
   }
}

