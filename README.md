# Buffer Overflow Demonstration

## Compilation

gcc -g -fno-stack-protector -no-pie vuln.c -o vuln

## GDB session transcript
```text
assessment/buffoverflow » gdb -q -nx ./vuln
Reading symbols from ./vuln...
(gdb) break vulnerable
Breakpoint 1 at 0x401132: file vuln.c, line 7.
(gdb) run AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Starting program: /home/rghv/assessment/buffoverflow/vuln AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

This GDB supports auto-downloading debuginfo from the following URLs:
  <https://debuginfod.archlinux.org>
Enable debuginfod for this session? (y or [n]) n
Debuginfod has been disabled.
To make this setting permanent, add 'set debuginfod enabled off' to .gdbinit.
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/usr/lib/libthread_db.so.1".

Breakpoint 1, vulnerable (input=0x7fffffffdbfd 'A' <repeats 38 times>) at vuln.c:7
7		strcpy(buffer, input);
(gdb) p &buffer
$1 = (char (*)[16]) 0x7fffffffd5f0
(gdb) p $rbp
$2 = (void *) 0x7fffffffd600
(gdb) info frame
Stack level 0, frame at 0x7fffffffd610:
 rip = 0x401132 in vulnerable (vuln.c:7); saved rip = 0x40116a
 called by frame at 0x7fffffffd630
 source language c.
 Arglist at 0x7fffffffd600, args: input=0x7fffffffdbfd 'A' <repeats 38 times>
 Locals at 0x7fffffffd600, Previous frame's sp is 0x7fffffffd610
 Saved registers:
  rbp at 0x7fffffffd600, rip at 0x7fffffffd608
(gdb) next
8	}
(gdb) x/gx $rbp+8
0x7fffffffd608:	0x4141414141414141
```
## Explanation

void vulnerable(char *input)
{
    char buffer[16];
    strcpy(buffer, input);
}

Before the strcpy() executes
	buffer = 0x7fffffffd5f0
	saved RBP = 0x7fffffffd600
	saved RIP = 0x7fffffffd608

The buffer is only of 16 bytes and strcpy() does no bounds checks and continues copying from the source string until the null bytes is encountered. In our case we supplied an input greater than 16 bytes beacuse of which bytes are written beyond the end of buffer and brgin overwriting adjacent stack memory.

As more bytes are copied overflow first overwrites the saved frame pointer and then the saved return address(The address is the address of the parent fucntion which called the vulnerable fucntion). The return address originally held the value 0x000000000040116a but after overflow we can see the value became 0x4141414141414141 (0x41 is the ascii for 'A').

When the function returns and executes the ret instruction, the CPU loads this corrupted value into the instruction pointer (RIP) and attempts to jump to it, resulting in a segmentation fault because the address is invalid.
