#include "rbtree.c"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h> /* rand() */

static int compare_int(void* left, void* right);
static void print_tree(rbtree t);
static void print_tree_helper(rbtree_node n, int indent);

int compare_int(void* leftp, void* rightp) {
    int left = (int)leftp;
    int right = (int)rightp;
    if (left < right) 
        return -1;
    else if (left > right)
        return 1;
    else {
        assert (left == right);
        return 0;
    }
}

#define INDENT_STEP  4

void print_tree_helper(rbtree_node n, int indent);

void print_tree(rbtree t) {
    print_tree_helper(t->root, 0);
    puts("");
}

void print_tree_helper(rbtree_node n, int indent) {
    int i;
    if (n == NULL) {
        fputs("<empty tree>", stdout);
        return;
    }
    if (n->right != NULL) {
        print_tree_helper(n->right, indent + INDENT_STEP);
    }
    for(i=0; i<indent; i++)
        fputs(" ", stdout);
    if (n->color == BLACK)
        printf("%d\n", (int)n->key);
    else
        printf("<%d>\n", (int)n->key);
    if (n->left != NULL) {
        print_tree_helper(n->left, indent + INDENT_STEP);
    }
}

int main() {
    int i,j, x;
    rbtree t = rbtree_create();
    print_tree(t);
while(1){
printf("What do u want to do??\n");
printf("1.Insert\n");
printf("2.Delete\n");
printf("3.Display\n");
printf("4.Exit\n");
//printf("What do u want to do??");
scanf("%d",&j);
switch(j)
{
	case 1:
		printf("Enter The data tobe Inserted:");
		scanf("%d",&x);
		printf("Inserting %d\n",x); //-> %d\n\n", x,x+1);
		rbtree_insert(t, (void*)x,(void*) (x+1), compare_int);
		print_tree(t);
		break;
	case 2:
		printf("Enter The data tobe Deleted:");
		scanf("%d",&x);
		rbtree_delete(t,(void*)x,compare_int);
		print_tree(t);
		break;
	case 3:
		print_tree(t);
		break;
	case 4:
		exit(1);
		break;
	default:
		break;
 //       assert(rbtree_lookup(t, (void*)x, compare_int) == (void*)y);
  //  }
//	print_tree(t);
//	rbtree_delete(t,(void*) 83, compare_int);
//	print_tree(t);

}
}
return 0;
}
