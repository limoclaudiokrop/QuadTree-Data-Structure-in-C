# QuadTree-Data-Structure-in-C
A quadtree is a data structure that stores -dimensional points and enables e�cient search for the
stored points. We will only consider the case . One particular quadtree which can be used to
store -dimensional points is the point-region quadtree, simply referred as a PR quadtree. A binary
tree can be de�ned as a �nite set of nodes that are either empty or have a root and two binary trees
 and (the left and right subtree). A quadtree has a similar recursive de�nition: instead of two
subtrees we have four subtrees, hence the name quad. This means that each node either has four
children or is a leaf node. The four leaf nodes are often referred to as NW, NE, SW, SE
