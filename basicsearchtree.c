#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct text_t {
	int leftChildren;
	int rightChildren;
    struct text_t   *left;
    struct text_t  *right;
    int height;
    char * text;
} text_t;

int numLines = 0;

void left_rotation(text_t *n) {
	text_t *tmp_node;
	int left_key, right_key;
	tmp_node = n->left;
	left_key = n->leftChildren;
	right_key = n->rightChildren;
	n->left = n->right;
	n->leftChildren	= n->right->leftChildren;
	n->rightChildren	= n->right->rightChildren;
	n->right = n->left->right;
	n->left->right = n->left->left;
	n->left->left = tmp_node;
	n->left->leftChildren = left_key;
	n->left->rightChildren = right_key;
}

void right_rotation(text_t *n) {
	text_t *tmp_node;
	int left_key, right_key;
	tmp_node = n->right;
	left_key = n->leftChildren;
	right_key = n->rightChildren;
	n->right = n->left;
	n->leftChildren	= n->left->leftChildren;
	n->rightChildren	= n->left->rightChildren;
	n->left = n->right->left;
	n->right->left = n->right->right;
	n->right->right = tmp_node;
	n->right->leftChildren = left_key;
	n->right->rightChildren = right_key;
}

text_t *get_node() {
	text_t *tmp;
	tmp = (text_t *) malloc(sizeof(text_t));
	tmp->left = NULL;
	tmp->right = NULL;
	tmp->text = NULL;
	tmp->leftChildren = 0;
	tmp->rightChildren = 0;
	tmp->height = 0;
	return tmp;
}

text_t * create_text(){
	return get_node();
}

int length_text( text_t *txt){
	return txt->leftChildren + txt->rightChildren + 1;
}

char * get_line( text_t *txt, int index) {
	int i = index;
	text_t *tmp_node;
	if(txt == NULL || i > numLines || i < 1)
		return(NULL);
	else {
		tmp_node = txt;
		while(tmp_node->leftChildren + 1 != i){
			if(i <= tmp_node->leftChildren){
				tmp_node = tmp_node->left;
			} else {
				i = i - tmp_node->leftChildren - 1;
				tmp_node = tmp_node->right;
			}
		}
		return tmp_node->text;
	}
}

void append_line( text_t *txt, char * new_line) {
	text_t *tmp_node;
	if( txt == NULL ) {
		txt = get_node();
		txt->text = new_line;
	} else {
		tmp_node = txt;
		while( tmp_node->right != NULL ) {
			tmp_node->rightChildren += 1;
			tmp_node = tmp_node->right;
		}
	    text_t *new_leaf;
		new_leaf = get_node();
		new_leaf->text = new_line;
		tmp_node->right = new_leaf;
		tmp_node->rightChildren += 1;
	}
	numLines += 1;
	return;
}

char * set_line( text_t *txt, int index, char * new_line){
	int i = index;
	text_t *tmp_node;
	if(txt == NULL || i > numLines || i <= 0)
		return(NULL);
	else {
		tmp_node = txt;
		while(tmp_node->leftChildren + 1 != i){
			if(i <= tmp_node->leftChildren){
				tmp_node = tmp_node->left;
			} else {
				i = i - tmp_node->leftChildren - 1;
				tmp_node = tmp_node->right;
			}
		}
		char *prev = strdup(tmp_node->text)
		tmp_node->text = new_line;
		return prev;
	}
}

void insert_line( text_t *txt, int index, char * new_line) {
	if(txt == NULL || index > numLines || index < 1)
		append_line(txt, new_line);
	else {
		int i = index;
		text_t *tmp_node;
		tmp_node = txt;
		while(tmp_node->leftChildren + 1 != i){
			if(i <= tmp_node->leftChildren){
				tmp_node->leftChildren += 1;
				tmp_node = tmp_node->left;
			} else {
				i = i - tmp_node->leftChildren - 1;
				tmp_node->rightChildren += 1;
				tmp_node = tmp_node->right;
			}
		}
		text_t *new = get_node();
		new->text = new_line;
		if(tmp_node->left == NULL){
			tmp_node->left = new;
			tmp_node->leftChildren += 1;
		} else {
			tmp_node->leftChildren += 1;
			tmp_node = tmp_node->left;
			tmp_node->rightChildren += 1;
			while(tmp_node->right != NULL){
				tmp_node = tmp_node->right;
				tmp_node->rightChildren += 1;
			}
			tmp_node->right = new;
		}
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
			free(txt);
			return(deleted_line);
		}
		else
			return( NULL );
	} else {
		char *nxt, *c;
		int i = index;
		c = get_line(txt, i + 1);
		c = set_line(txt, i, c);
		i++;
		while(i < numLines){
			nxt = get_line(txt, i + 1);
			nxt = set_line(txt, i, nxt);
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
		  free(tmp_node);
		  return(deleted_line);
	  }
   }
}

int main() {
	return 0;
}

