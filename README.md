A concurrent version differs from the general sequential data structure in a way that the former will have multiple threads performing various actions on it.
As multiple threads are accessing a global structure, race conditions, deadlocks and other concurrency bugs must be taken care of. Mutexes have been used for ensuring synchronization. The basic goal would be to have a consistent data structure, even after multiple threads perform some actions on it.

1. To run sample test inputs using the script : execute
```
$ ./execute
```

When the prompt appears:
- Enter test file number(1,2 or 3) to run with corresponding test file (any other number to exit)
- Enter the number of times you want to run the program over the chosen test file.
	
Test case:
- test_1.txt : basic insertion and search operations
- test_2.txt : basic insertion, search and delete operations
- test_3.txt : corner cases

2. To run tests on any other test file
```
$ g++ doubly_linked_lists.c -lpthread -o dll
$ ./dll < input.txt
```

Operation code:
- Insert element at the end : 1
- Delete specified element : 2
- Search and return element's position : 3
- Print entire list : 4
- Exit : Any other number

