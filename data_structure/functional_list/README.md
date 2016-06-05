DESCRIPTION
--------------

Singly linked list implemented following functional approach.


COMPLEXITY
---------------
  - Insertion -> O(1)
  - Constructor -> O(N) where N is the size of items to insert
  - reverse -> O(N) time - O(N) space 
  - filter -> O(N) time - O(N) space 
  - map -> O(N) time - O(N) space
  - foldLeft -> O(N) time - O(N) space

Memory caveats:
The List keeps an extra pointer to the tail of the list to guarantee O(1) insertions

MULTITHREADING
----------------

The list follows a fully functional design approach. The list itself it is immutable (except when a new element is pushed in the list itself.. in this example this happens only when the constructor runs).
Then the follow operations are not thread safe:

  - Constructor (constructing an object concurrently is never a good idea)
  - Inserting a new item in the collection

All the rest could be viewed as a thread safe operations, because the list is always only read and a new local copy gets constructed and returned. 
Then multiple threads could be safely interleaved as long as the list does not get altered (with an insertion) by another thread.
To wrap this up. N threads can safely read the list as long as none of them tries to insert a new element into it.
