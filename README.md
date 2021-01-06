Homework 7 (UM Profile)

NAME:
Jae Hyun Cheigh(jcheig01)
SuYu Liu(sliu21)

Model Info:
Intel(R) Xeon(R) Silver 4214Y CPU @ 2.20GHz
Red Hat Enterprise Linux Server release 7.8 (Maipo)
gcc (GCC) 9.1.0

ACKNOWLEDGEMENT:

1. Julia and Jacqueline helped us clarify misunderstandings that
   we had on the spec.

2. Hanson's book provided us with implementation of its data structures
   that we referenced to profile our code.

BENCHMARKS:

midmark.um, advent.umz, sandmark.umz

WHAT ROUTINE TAKES UP THE MOST TIME/COULD THE ASSEMBLY CODE BE IMPROVED?

By using kcachegrind, we found that from run_um function, which
took up the most time as it handled all the UM operations, getting
opcode value & register A, B, C values took up the most time in our
code. It is the part where bit shifting took place.

This process had assembly code of:

mov    %eax,%edx
mov    %eax,%ecx
mov    %eax,%r8d
shr    $0x6,%edx
shr    $0x3,%ecx
and    $0x7,%r8d
mov    %edx,%esi
mov    %eax,%edx
and    $0x7,%ecx
and    $0x7,%esi

Since mov, and, shr calls were already made in an efficient
manner, there is no obvious way to improve the assembly code at
this point.

The other routines that took most time include malloc and free calls.
Yet, both have jmpq call to the library functions of malloc and free
so it is difficult at this point to improve the assembly code as well.


Time for analyzing problems: 3 Hours
Time for solving the problems after analysis: 18 Hours