/* Remember the Properties
1. Each node is either red or black:
2. The root node is black.
3. All leaves (shown as NIL in the above diagram) are black and contain no data. Since we represent these empty leaves using NULL, this property is implicitly assured by always treating NULL as black. To this end we create a node_color() helper function:
4. Every red node has two children, and both are black (or equivalently, the parent of every red node is black).
5. All paths from any given node to its leaf nodes contain ithe same number of black node.
.... IDEA - traversing the tree, incrementing a black node count as i go. The first time i reach a leaf we save the count. Pinne adutha thavana leaves ethumbol compare chey...
Properties 4 and 5 together guarantee that no path in the tree is more than about twice as long as any other path, which guarantees that it has O(log n) height.
*/

#include "rbtree.h"
#include <assert.h>
#include <stdlib.h>

node grandparent(node n) {
    assert (n != NULL);
    assert (n->parent != NULL);
    assert (n->parent->parent != NULL);
    return n->parent->parent;
}

node sibling(node n) {
    assert (n != NULL);
    assert (n->parent != NULL);
    if (n == n->parent->left)
        return n->parent->right;
    else
        return n->parent->left;
}

node uncle(node n) {
    assert (n != NULL);
    assert (n->parent != NULL);
    assert (n->parent->parent != NULL);
    return sibling(n->parent);
}

void verify_properties(rbtree t) {
    property_1(t->root);
    property_2(t->root);
    property_4(t->root);
    property_5(t->root);
}

void property_1(node n) {
    assert(node_color(n) == RED || node_color(n) == BLACK);
    if (n == NULL) return;
    property_1(n->left);
    property_1(n->right);
}

void property_2(node root) {
    assert(node_color(root) == BLACK);
}

color node_color(node n) {
    return n == NULL ? BLACK : n->color;
}

void property_4(node n) {
    if (node_color(n) == RED) {
        assert (node_color(n->left)   == BLACK);
        assert (node_color(n->right)  == BLACK);
        assert (node_color(n->parent) == BLACK);
    }
    if (n == NULL) return;
    property_4(n->left);
    property_4(n->right);
}

void property_5(node root) {
    int black_count_path = -1;
    property_5_helper(root, 0, &black_count_path);
}

void property_5_helper(node n, int black_count, int* path_black_count) {
    if (node_color(n) == BLACK) {
        black_count++;
    }
    if (n == NULL) {
        if (*path_black_count == -1) {
            *path_black_count = black_count;
        } else {
            assert (black_count == *path_black_count);
        }
        return;
    }
    property_5_helper(n->left,  black_count, path_black_count);
    property_5_helper(n->right, black_count, path_black_count);
}

rbtree rbtree_create() {
    rbtree t = malloc(sizeof(struct rbtree_t));
    t->root = NULL;
    verify_properties(t);
    return t;
}

node new_node(void* key , color node_color, node left, node right) {
    node result = malloc(sizeof(struct rbtree_node_t));
    result->key = key;
    result->color = node_color;
    result->left = left;
    result->right = right;
    if (left  != NULL)  left->parent = result;
    if (right != NULL) right->parent = result;
    result->parent = NULL;
    return result;
}

node lookup_node(rbtree t, void* key, compare_func compare) {
    node n = t->root;
    while (n != NULL) {
        int comp_result = compare(key, n->key);
        if (comp_result == 0) {
            return n;
        } else if (comp_result < 0) {
            n = n->left;
        } else {
            assert(comp_result > 0);
            n = n->right;
        }
    }
    return n;
}

void* rbtree_lookup(rbtree t, void* key, compare_func compare) {
    node n = lookup_node(t, key, compare);
    return n == NULL ? NULL : n->key;
}

void rotate_left(rbtree t, node n) {
    node r = n->right;
    replace_node(t, n, r);
    n->right = r->left;
    if (r->left != NULL) {
        r->left->parent = n;
    }
    r->left = n;
    n->parent = r;
}

void rotate_right(rbtree t, node n) {
    node L = n->left;
    replace_node(t, n, L);
    n->left = L->right;
    if (L->right != NULL) {
        L->right->parent = n;
    }
    L->right = n;
    n->parent = L;
}

void replace_node(rbtree t, node oldn, node newn) {
    if (oldn->parent == NULL) {
        t->root = newn;
    } else {
        if (oldn == oldn->parent->left)
            oldn->parent->left = newn;
        else
            oldn->parent->right = newn;
    }
    if (newn != NULL) {
        newn->parent = oldn->parent;
    }
}

void rbtree_insert(rbtree t, void* key, compare_func compare) {
    node inserted_node = new_node(key, RED, NULL, NULL);
    if (t->root == NULL) {
        t->root = inserted_node;
    } else {
        node n = t->root;
        while (1) {
            int comp_result = compare(key, n->key);
            if (comp_result == 0) {
                free (inserted_node);
                return;
            } else if (comp_result < 0) {
                if (n->left == NULL) {
                    n->left = inserted_node;
                    break;
                } else {
                    n = n->left;
                }
            } else {
                assert (comp_result > 0);
                if (n->right == NULL) {
                    n->right = inserted_node;
                    break;
                } else {
                    n = n->right;
                }
            }
        }
        inserted_node->parent = n;
    }
    insert_case1(t, inserted_node);
    verify_properties(t);
}

void insert_case1(rbtree t, node n) {
    if (n->parent == NULL)
        n->color = BLACK;
    else
        insert_case2(t, n);
}

void insert_case2(rbtree t, node n) {
    if (node_color(n->parent) == BLACK)
        return;
    else
        insert_case3(t, n);
}

void insert_case3(rbtree t, node n) {
    if (node_color(uncle(n)) == RED) {
        n->parent->color = BLACK;
        uncle(n)->color = BLACK;
        grandparent(n)->color = RED;
        insert_case1(t, grandparent(n));
    } else {
        insert_case4(t, n);
    }
}

void insert_case4(rbtree t, node n) {
    if (n == n->parent->right && n->parent == grandparent(n)->left) {
        rotate_left(t, n->parent);
        n = n->left;
    } else if (n == n->parent->left && n->parent == grandparent(n)->right) {
        rotate_right(t, n->parent);
        n = n->right;
    }
    insert_case5(t, n);
}

void insert_case5(rbtree t, node n) {
    n->parent->color = BLACK;
    grandparent(n)->color = RED;
    if (n == n->parent->left && n->parent == grandparent(n)->left) {
        rotate_right(t, grandparent(n));
    } else {
        assert (n == n->parent->right && n->parent == grandparent(n)->right);
        rotate_left(t, grandparent(n));
    }
}

void rbtree_delete(rbtree t, void* key, compare_func compare) {
    node child;
    node n = lookup_node(t, key, compare);
    if (n == NULL) return; 
    if (n->left != NULL && n->right != NULL) {
        node pred = maximum_node(n->left);
        n->key   = pred->key;
        n = pred;
    }

    assert(n->left == NULL || n->right == NULL);
    child = n->right == NULL ? n->left  : n->right;
    if (node_color(n) == BLACK) {
        n->color = node_color(child);
        delete_case1(t, n);
    }
    replace_node(t, n, child);
    if (n->parent == NULL && child != NULL)
        child->color = BLACK;
    free(n);

    verify_properties(t);
}

static node maximum_node(node n) {
    assert (n != NULL);
    while (n->right != NULL) {
        n = n->right;
    }
    return n;
}

void delete_case1(rbtree t, node n) {
    if (n->parent == NULL)
        return;
    else
        delete_case2(t, n);
}

void delete_case2(rbtree t, node n) {
    if (node_color(sibling(n)) == RED) {
        n->parent->color = RED;
        sibling(n)->color = BLACK;
        if (n == n->parent->left)
            rotate_left(t, n->parent);
        else
            rotate_right(t, n->parent);
    }
    delete_case3(t, n);
}

void delete_case3(rbtree t, node n) {
    if (node_color(n->parent) == BLACK &&
        node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == BLACK &&
        node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        delete_case1(t, n->parent);
    }
    else
        delete_case4(t, n);
}

void delete_case4(rbtree t, node n) {
    if (node_color(n->parent) == RED &&
        node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == BLACK &&
        node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        n->parent->color = BLACK;
    }
    else
        delete_case5(t, n);
}

void delete_case5(rbtree t, node n) {
    if (n == n->parent->left &&
        node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == RED &&
        node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        sibling(n)->left->color = BLACK;
        rotate_right(t, sibling(n));
    }
    else if (n == n->parent->right &&
             node_color(sibling(n)) == BLACK &&
             node_color(sibling(n)->right) == RED &&
             node_color(sibling(n)->left) == BLACK)
    {
        sibling(n)->color = RED;
        sibling(n)->right->color = BLACK;
        rotate_left(t, sibling(n));
    }
    delete_case6(t, n);
}

void delete_case6(rbtree t, node n) {
    sibling(n)->color = node_color(n->parent);
    n->parent->color = BLACK;
    if (n == n->parent->left) {
        assert (node_color(sibling(n)->right) == RED);
        sibling(n)->right->color = BLACK;
        rotate_left(t, n->parent);
    }
    else
    {
        assert (node_color(sibling(n)->left) == RED);
        sibling(n)->left->color = BLACK;
        rotate_right(t, n->parent);
    }
}


