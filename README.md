Homework 6 (UM)

NAME:
Jae Hyun Cheigh(jcheig01)
SuYu Liu(sliu21)

ACKNOWLEDGEMENT:

1. Julia helped us clarify some concepts during our design stage.

2. Hanson's book provides us with some very useful data structures during
implementation stage.

WHAT HAS/HAS NOT BEEN IMPLEMENTED.

To the best of our knowledge, we have carefully implemented:
1. um_loader by initializing registers and memory segments and executing
the program by reading in instructions words. Also, the module contains
3 um operations that are used at program level: halt, load value, load
program.

2. arith by implementing 5 um operations that are used for arithmetic
purposes: conditional move, addition, multiplication, division,
bitwise NAND.

3. io_handler by implementing 2 um operations that are used for I/O
of program: output, input.

4. mem_segments by implementing 4 um operations that are used for
dealing with memory segments: segmented load, segmented store,
map segment, unmap segment.

DEPARTURE FROM DESIGN:

We made one more module named um_loader that serves as the core module
as it handles initialization and execution of the program. We believed
this module was necessary because if these steps were all contained in the
main file, it would've been disorganized, which defeats the purpose of
abstraction and modularity.

ARCHITECTURE:

1. um_loader used "uarray" and "seq" Hanson modules for registers and
memory segments. Also, it used "bitpack" module to get instruction words
and necessary values and opcodes from the words. It used arith, io_handler,
mem_segments modules to be used for appropriate opcode. Hence, it implements
abstractions for initializing the program, executing the program based on
the instructions, and 3 um operations: halt, load value, load program.

2. arith used "uarray" Hanson module for register. With this, it implements
abstractions for 5 um operations: conditional move, addition, multiplication,
division, bitwise NAND.

3. io_handler used "uarray" Hanson module for register. With this, it
implements abstractions for 2 um operations: output, input.

4. mem_segments used "uarray" and "seq" Hanson modules for registers and
memory segments. With this, it implements abstractions for 4 um 
operations: segmented load, segmented store, map segment, unmap segment.

EXECUTING 50 MILLION INSTRUCTIONS:

We received a test file called midmark.um which contains more than 80 million
instructions. By using time command, our program ran midmark.um in
6.56 seconds. By multiplying 5/8 from this value, we can say that 
emulator runs 50 million insturctions in approximately 4.1 seconds.

UNIT TESTS:

1. halt.um
Test if halt operation exits program with freeing all memory.

2. halt_verbose.um
Test if halt operation takes place before other operations.

3. add.um
Test if addition operation takes place without any error.

4. output.um
Test if output operation prints out '4' which is the value
loaded in the register.

5. print-six.um
Test if addition operation outputs '6' by adding 2 loaded values
from 2 registers to another register.

6. mult.um (excluded because it's a test to fail machine)
Test for edge case if multiplication operation outputs with error for
multiplying 2 loaded values from 2 registers to another register since
output value > 255.

7. mult2.um
Test if multiplication operation outputs 'P' by multiplying 2 loaded
values from 2 registers to another register.

8. div.um
Test if division operation outputs 'A' by dividing 2 loaded values
from 2 registers to another register.

9. cond_mov.um
Test if conditional move operation outputs '0' by moving value from
one register to another if 3rd register value is not 0.

10. cond_mov2.um
Test if conditional move operation outputs '(' by not moving value
from one register to another if 3rd register value is 0.

11. bit_NAND.um
Test if bitwise NAND operation outputs '%' by using & operator on
values from 2 registers and use ~ operator on the resulting value 
and storing that value to other register.

12. input.um
Test if input operation reads 3 times (3 characters) and output
operation prints out those 3 input values.

13. map.um
Test if map segment operation and unmap segment operation 
works properly without any leaks or error.

14. segmented.um
Test if segmented store operation stores value correctly and
segmented load operation loads value correctly and output
same value of '2'. 

15. load_program.um
Test if load program operation loads program and skip printing
'h' and 'E' and directly prints 'y'.


Time for analyzing homework: 4 Hours
Time for preparing design: 10 Hours
Time for solving the problems after analysis: 20 Hours