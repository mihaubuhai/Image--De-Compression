## Objective
    This project, written in C, can compress and decompress .ppm P6 file images, square in shape and of dimensions powers of 2.

## Use
    make && ./quadtree [ -c <threshold> || -d ] <input_file> <output_file>,
    where:
        -c <threshold>: compresses <input_file> with given <threshold> for accuracy (lower the threshold, better the compression; must be positive or zero!!)
        -d: decompresses <input_file>
    
## Implementation
    For compression, the idea relies on building a tree, each node having exactly 4 nodes, where each leaf node symbolizes a colored region from the initial image.
    A node becomes a leaf node when the arithmetic mean of the RGB values of the region it defines is smaller than the given threshold.
    This procedure "divide_mat" is called with 'dim' argument equal to the value of the width of the image (the image has a square shape, so it does not matter which side is given) and the procedure calls itself with halfed 'dim' each time the current node does not satisfy the correct arithmetic mean value.
    The pair of coordinates (x, y) define the leftmost corner of the submatrix the procedure has reached, something like:

                _____dim / 2______ _____dim / 2_____
                | (x,y) (a,b) ... | (x,y) (c,d) ... |
                | .               |                 |
    dim / 2     | .    Mat_1      |     Mat_2       |
                | .               |                 |         -->Each square matrix (Mat_X) can be seen as a Cartesian Coordinate System,
                | --------------  | ----------------|            and it can be divided into 4 quadrants.
                | (x,y) (e,f) ... | (x,y) (g,h) ... |            The values that help creating the submatrices are the ones from 
                | .               |                 |            the first line and column.
    dim / 2     | .    Mat_4      |     Mat_3       |            
                | .               |                 |            
                | ...             | ...             |

    After the tree is completed, its' node's type (if its leaf node or not) and RGB values are written in a .ppm file, in binary format; the tree is traversed by level.    
#
    For decompression, a tree is built from the information read from the input file, that respects the format of the output file of the procedure above.
    After that, the tree is traversed in a DFS manner, finding the leaf nodes and assign the coresponding matrix's values the leaf's RGB values.
    Traversing the tree implies the same idea for building the tree, implemented in compression: the procedure starts from the big matrix and each recursive call works on a submatrix of size 4 times less than the initial matrix; if the node is a leaf, the entire submatrix is filled with leaf's RGB values.
    Once the matrix is completed, its contents are written in a .ppm file in binary format