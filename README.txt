MiniShell
Authored by Laith Al-mouhtaseb
211353297

==Description==
The program took a polynomial and a value to place in it, and return the result.
There's two method to solve this, first, using threads, second using forks and child process with shared memory.

functions:
ex4a:

1.Struct POL, this struct contains two parameters, string and the length of it.

2.calc: method for the threads to calculate place the value into x and return the result for the main process.

3.freeStruct: free the allocated memory of the using struct.

4.loop: infinity loop, first get input from the user and check the syntax if it's correct,
the split it to 3 section to calculate using threads.

5.check: check the syntax of the input and return 1 if there's problem with it and 0 if everything correct.

6.countPlus:count how many '+' in the Polynomial.

7.mathPow:calculate the result of base in power, and return the value.

8.splitToStruct:split the pol into struct according to the '+' in the polynomial.

ex4b:

1.calc: calculate the value if the polynomial using forks and new process and shared memory to communicate between the process.

2.loop: infinity loop, first get input from the user and check the syntax if it's correct,
the split it to 3 section to calculate using threads.

3.check: check the syntax of the input and return 1 if there's problem with it and 0 if everything correct.

4.countPlus:count how many '+' in the Polynomial.

5.mathPow:calculate the result of base in power, and return the value.

6.splitToArray:split the pol into array according to the '+' in the polynomial.

==Program Files==
ex4a.c, ex4b.c

==How to compile==
compile: gcc ex4a.c -o ex4a
run: ./ex4a

compile: gcc ex4b.c -o ex4b
run: ./ex4b

==input==
string from user "POL, VAL"

==output==
the result of the polynomial
