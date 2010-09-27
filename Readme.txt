This is my implementation of the self balancing Red Black Tree... It has an insert function,  delete function and others such as display, inorder representation, preorder representation and postorder representation.

Basic Properties of Red Black Tree:
1. Each node is either red or black:
2. The root node is black.
3. All leaves (shown as NIL in the above diagram) are black and contain no data. Since we represent these empty leaves using NULL, this property is implicitly assured by always treating NULL as black. To this end we create a node_color() helper function:
4. Every red node has two children, and both are black (or equivalently, the parent of every red node is black).
5. All paths from any given node to its leaf nodes contain ithe same number of black node.
Properties 4 and 5 together guarantee that no path in the tree is more than about twice as long as any other path, which guarantees that it has O(log n) height.
