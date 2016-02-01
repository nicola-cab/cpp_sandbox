Tail recursive algorithms vs stl ones 
===================


I compared a bunch of stl algorithms with a their counter part written using tail recursion, in order to understand how much can a compiler optimise a recursive function that uses tail recursion.

Since all stl algorithms accept ranges as input, I used a std::vector of ints with size N set 10000 (without optimisation flag I could not go beyond these size without getting segfault for the recursive algorithms). These are  the numbers I got compiling this code using clang and optimisation flag -O3

Notice: I ran the same set of algorithms on windows using visual studio 2013 and I got different results but these result were comparable with what reported in this table. 

| algorithm      | time stl (ns) | time tail recursive (ns) | winner
| ------------- |-------------| -----|
| find           |    0         |    0  |   same |
| find_if        |    0         |    0  |   same |
| find_if_not    |    0         |    0  |   same |
| find_first_of  |    0         |    0  |   same |
| adject_find    |    1000      |    0  |   tail | 
| count          |    9000      |    8000 | tail |
| count_if       |    5000      |    16000 | stl | 
| accumulate     |    2000      |    1000 |  tail |
| mismatch       |    0         |       0 |  same |
| equal          |    9000      |     8000 | tail |
| equal (false pred)|    0      |     1000 | stl |
| advance        |    0         |        0 | same |
| distance       |    0         |        0 | same | 
| is_permutation |    26000     |        0 | tail |
| search         |    0         |    32000 | stl|
| min_element    |   19000      | 29000    | stl |
| adjacent_difference | 0       | 0        | same |
| inner_product | 6000 | 7000 | stl|
| partial_sum | 19000 | 10000| tail |

**Turning down the optimisation flag (by default O1)... here there results I got**

| algorithm      | time stl (ns) | time tail recursive (ns) | winner
| ------------- |-------------| -----|
| find           |    1959000         |    0  |   tail |
| find_if        |    177000        |    1000  |   tail |
| find_if_not    |    5000         |    0  |   tail |
| find_first_of  |    202000         |    1000  |   tail |
| adject_find    |    0      |    217000  |   stl | 
| count          |    95000      |    1278000 | stl |
| count_if       |    5000      |    16000 | stl | 
| accumulate     |    2576000      |    404000 |  tail |
| mismatch       |    2368000         |       0 |  tail|
| equal          |    134000      |     2745000 | stl |
| equal (false pred)|    0      |     0 | same |
| advance        |    13000        |     146000 | stl |
| distance       |    0         |        3329000 | stl | 
| is_permutation |    246000     |        3390000 | stl |
| search         |    3000        |    669000 | stl|
| min_element    |   130000      | 432000    | stl |
| adjacent_difference | 1000       | 2112000        | stl|
| inner_product |181000 | 1975000 | stl|
| partial_sum | 189000 | 1951000| stl |



Conclusion
-------------

It looks like without optimisation stl over-performs tail recursive implemented algorithms. With O3 enable there is tail recursion optimisation does its job and makes a recursive call run as fast as the iterative counterpart.





----------


