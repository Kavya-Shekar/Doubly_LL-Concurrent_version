1. To run sample test inputs using the script : execute
```
$ ./execute
```

When the prompt appears:
> Enter test file number(1,2 or 3) to run with corresponding test file (any other number to exit)
> Enter the number of times you want to run the program over the chosen test file.
	
Test case:
	test_1.txt : basic insertion and search operations
	test_2.txt : basic insertion, search and delete operations
	test_3.txt : corner cases

2. To run tests on any other test file
```
$ g++ doubly_linked_lists.c -lpthread -o dll
$ ./dll < input.txt
```

Operation code:
	1. Insert at the end
	2. Delete element
	3. Search element
	4. Print list
	Any other number : Exit


