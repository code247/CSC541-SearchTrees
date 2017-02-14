#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct text_t {
	int leftChildren;
	int rightChildren;
    struct text_t   *left;
    struct text_t  *right;
    int height;
    char * text;
} text_t;

int numLines = 0;

void left_rotation(text_t *n){
	text_t *tmp_node;
	char * tmp_key;
	int tmp_left = n->leftChildren;
	int tmp_right = n->rightChildren;
	tmp_node = n->left;
	tmp_key  = strdup(n->text);
	n->left  = n->right;
	n->text   = strdup(n->right->text);
	n->right = n->left->right;
	n->left->right = n->left->left;
	n->left->left  = tmp_node;
	n->left->text   = tmp_key;
	n->leftChildren = n->left->leftChildren + 1;
	n->rightChildren = n->left->rightChildren;
	n->left->leftChildren = tmp_left;
	n->left->rightChildren = tmp_right - n->left->rightChildren - 1;
}

void right_rotation(text_t *n){
	text_t *tmp_node;
	char * tmp_key;
	int tmp_left = n->leftChildren;
	int tmp_right = n->rightChildren;
	tmp_node = n->right;
	tmp_key  = strdup(n->text);
	n->right = n->left;
	n->text   = strdup(n->left->text);
	n->left  = n->right->left;
	n->right->left = n->right->right;
	n->right->right  = tmp_node;
	n->right->text   = tmp_key;
	n->rightChildren = n->right->rightChildren + 1;
	n->leftChildren = n->right->leftChildren;
	n->right->rightChildren = tmp_right;
	n->right->leftChildren = tmp_left - n->right->leftChildren - 1;
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
		text_t * stack[100];
		int stack_p = 0;
		while( tmp_node->right != NULL ) {
			tmp_node->rightChildren += 1;
			stack[stack_p++] = tmp_node;
			tmp_node = tmp_node->right;
		}
	    text_t *new_leaf;
		new_leaf = get_node();
		new_leaf->text = new_line;
		tmp_node->right = new_leaf;
		tmp_node->rightChildren += 1;
		stack[stack_p++] = tmp_node;
		int tmp_ptr = stack_p;
		while(tmp_ptr > 0){
			text_t *tmp = stack[--tmp_ptr];
			if(tmp->left != NULL && tmp->right != NULL){
				if (tmp->left->height > tmp->right->height)
					tmp->height = tmp->left->height + 1;
				else
					tmp->height = tmp->right->height + 1;
			} else if (tmp->right == NULL){
				tmp->height = tmp->left->height + 1;
			} else {
				tmp->height = tmp->right->height + 1;
			}
		}
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
		char *prev = strdup(tmp_node->text);
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
		text_t * stack[100];
		int stack_p = 0;
		while(tmp_node->leftChildren + 1 != i){
			if(i <= tmp_node->leftChildren){
				tmp_node->leftChildren += 1;
				stack[stack_p++] = tmp_node;
				tmp_node = tmp_node->left;
			} else {
				i = i - tmp_node->leftChildren - 1;
				tmp_node->rightChildren += 1;
				stack[stack_p++] = tmp_node;
				tmp_node = tmp_node->right;
			}
		}
		text_t *new = get_node();
		new->text = new_line;

		if(tmp_node->left == NULL){
			tmp_node->left = new;
			tmp_node->leftChildren += 1;
			stack[stack_p++] = tmp_node;
		} else {
			stack[stack_p++] = tmp_node;
			tmp_node->leftChildren += 1;
			tmp_node = tmp_node->left;
			tmp_node->rightChildren += 1;
			while(tmp_node->right != NULL){
				stack[stack_p++] = tmp_node;
				tmp_node = tmp_node->right;
				tmp_node->rightChildren += 1;
			}
			stack[stack_p++] = tmp_node;
			tmp_node->right = new;
		}
		int tmp_ptr = stack_p;
		while(tmp_ptr > 0){
			text_t *tmp = stack[--tmp_ptr];
			if(tmp->left != NULL && tmp->right != NULL){
				if (tmp->left->height > tmp->right->height)
					tmp->height = tmp->left->height + 1;
				else
					tmp->height = tmp->right->height + 1;
			} else if (tmp->right == NULL){
				tmp->height = tmp->left->height + 1;
			} else {
				tmp->height = tmp->right->height + 1;
			}
		}
		int finished = 0;
		while(stack_p > 0 && !finished){
			int tmp_height, old_height;
			tmp_node = stack[--stack_p];
			old_height= tmp_node->height;
			if(tmp_node->left->height - tmp_node->right->height == 2 ){
				if( tmp_node->left->left->height - tmp_node->right->height == 1 ) {
					right_rotation( tmp_node );
					tmp_node->right->height = tmp_node->right->left->height + 1;
					tmp_node->height = tmp_node->right->height + 1;
				} else {
					left_rotation(tmp_node->left);
					right_rotation( tmp_node );
					tmp_height = tmp_node->left->left->height;
					tmp_node->left->height  = tmp_height + 1;
					tmp_node->right->height = tmp_height + 1;
					tmp_node->height = tmp_height + 2;
				}
			}
			else if (tmp_node->left->height - tmp_node->right->height == -2 ) {
				if( tmp_node->right->right->height - tmp_node->left->height == 1 ){
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
			} else /* update height even if there was no rotation */ {
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

char * delete_line( text_t *txt, int index) {
	char *ret = NULL;
	if(txt == NULL || index > numLines || index < 1)
		return NULL;
	else {
		int i = index;
		text_t *tmp_node;
		tmp_node = txt;
		text_t * stack[100];
		int stack_p = 0;
		int flag = 0;
		while(tmp_node->leftChildren + 1 != i){
			if(i <= tmp_node->leftChildren){
				tmp_node->leftChildren -= 1;
				stack[stack_p++] = tmp_node;
				tmp_node = tmp_node->left;
				flag = 0;
			} else {
				i = i - tmp_node->leftChildren - 1;
				tmp_node->rightChildren -= 1;
				stack[stack_p++] = tmp_node;
				tmp_node = tmp_node->right;
				flag = 1;
			}
		}
		if(stack_p == 0){
			if(txt->left == NULL && txt->right == NULL){
				ret = strdup(txt->text);
				free(txt);
				return ret;
			} else if (txt->right == NULL){
				text_t *tmp = txt;
				ret = strdup(txt->text);
				txt = txt->left;
				tmp->left = NULL;
				free(tmp);
				return ret;
			} else if (txt->left == NULL){
				text_t *tmp = txt;
				ret = strdup(txt->text);
				txt = txt->right;
				tmp->right = NULL;
				free(tmp);
				return ret;
			} else {
				text_t *upper_node = tmp_node;
				tmp_node->rightChildren -= 1;
				tmp_node = tmp_node->right;
				while(tmp_node->left != NULL){
					upper_node = tmp_node;
					tmp_node->leftChildren -= 1;
					tmp_node = tmp_node->left;
				}
				ret = strdup(txt->text);
				txt->text = strdup(tmp_node->text);
				upper_node->left = tmp_node->right;
				tmp_node->right = NULL;
				free(tmp_node);
				return ret;
			}
		} else {
			if(tmp_node->left == NULL && tmp_node->right == NULL){
				ret = strdup(tmp_node->text);
				if (flag == 0) stack[stack_p - 1]->left = NULL;
				else stack[stack_p - 1]->right = NULL;
				free(tmp_node);
				return ret;
			} else if (tmp_node->right == NULL){
				ret = strdup(tmp_node->text);
				if (flag == 0) stack[stack_p - 1]->left = tmp_node->left;
				else stack[stack_p - 1]->right = tmp_node->left;
				tmp_node->left = NULL;
				free(tmp_node);
				return ret;
			} else if (tmp_node->left == NULL){
				ret = strdup(tmp_node->text);
				if (flag == 0) stack[stack_p - 1]->left = tmp_node->right;
				else stack[stack_p - 1]->right = tmp_node->right;
				tmp_node->right = NULL;
				free(tmp_node);
				return ret;
			} else {
				text_t *tmp = tmp_node;
				text_t *upper_node = tmp_node;
				tmp_node->rightChildren -= 1;
				tmp_node = tmp_node->right;
				while(tmp_node->left != NULL){
					upper_node = tmp_node;
					tmp_node->leftChildren -= 1;
					tmp_node = tmp_node->left;
				}
				ret = strdup(tmp->text);
				if (flag == 0) stack[stack_p - 1]->left = tmp_node;
				else stack[stack_p - 1]->right = tmp_node->right;
				upper_node->left = tmp_node->right;
				tmp_node->right = NULL;
				free(tmp_node);
				return ret;
			}
		}
		int finished = 0;
		while(stack_p > 0 && !finished){
			int tmp_height, old_height;
			tmp_node = stack[--stack_p];
			old_height= tmp_node->height;
			if(tmp_node->left->height - tmp_node->right->height == 2 ){
				if( tmp_node->left->left->height - tmp_node->right->height == 1 ) {
					right_rotation( tmp_node );
					tmp_node->right->height = tmp_node->right->left->height + 1;
					tmp_node->height = tmp_node->right->height + 1;
				} else {
					left_rotation(tmp_node->left);
					right_rotation( tmp_node );
					tmp_height = tmp_node->left->left->height;
					tmp_node->left->height  = tmp_height + 1;
					tmp_node->right->height = tmp_height + 1;
					tmp_node->height = tmp_height + 2;
				}
			}
			else if (tmp_node->left->height - tmp_node->right->height == -2 ) {
				if( tmp_node->right->right->height - tmp_node->left->height == 1 ){
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
			} else /* update height even if there was no rotation */ {
				if( tmp_node->left->height > tmp_node->right->height )
					tmp_node->height = tmp_node->left->height + 1;
				else
					tmp_node->height = tmp_node->right->height + 1;
			}
			if( tmp_node->height == old_height )
				finished = 1;
		}
		return ret;
   }
}

int main() {
	return 0;
}

