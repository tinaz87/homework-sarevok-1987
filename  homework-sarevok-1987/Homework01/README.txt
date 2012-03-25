EXPLAIN ALGORITHM:

The idea is: visit the matrix for levels and check if there are some puddle in this level. (The algorithm don't work with negative value)
The first level is the lowest height in the matrix and last level is the highest height in the matrix.

When level is scanned we check value of height in matrix's cell is less or equal then value of the level plus one. If it's we'll mark it as puddle.
Next step is to check some of this puddles have a leak (link with the edges of matrix) and if it's we'll spread the leaks to the nears puddle

Last level scan step is to sum the gain of puddels at this level ad pass to the next level.

Example

4 4 4 4 4 4    First level is 1
4 2 1 3 4 2    Last level is 6
3 1 2 3 2 4
4 3 2 1 3 4
5 6 6 6 6 6

In first scan we have

0 0 0 0 0 0    We don't have any leaks
0 0 1 0 0 0    so we have a total gain of 3
0 1 0 0 0 0
0 0 0 1 0 0
0 0 0 0 0 0

and so on.