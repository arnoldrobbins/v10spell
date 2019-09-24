# The Tenth Edition UNIX Spell Program

This is the <tt>spell</tt> program from the Tenth Edition Research UNIX system.  It has been updated somewhat to compile on modern systems, particularly on Ubuntu Linux.

## Installation

The <tt>makefile</tt> and the code have been modified so that installation locations are easily changed; editing the <tt>PREFIX</tt> setting in the <tt>makefile</tt> and then running <tt>make</tt> should be all that's necessary.

The program installs itself under the name <tt>v10spell</tt> to avoid conflicts with any native spelling checkers already installed or that might be installed in the future.

After editing the <tt>makefile</tt> (if necessary), it should be enough to do:

	make
	sudo make install

## History

The original C version of the UNIX <tt>spell</tt> program was written by Doug McIlroy for V7 UNIX.  The tenth edition version, also by Doug McIlroy, is apparently a descendant of the original one, but able to take advantage of the larger address space available on VAXen as compared to PDP-11s.

Doug McIlroy published a paper on the original UNIX <tt>spell</tt> in IEEE Transactions on Communications. It's available from his [home page](https://www.cs.dartmouth.edu/~doug/spell.pdf).

In private mail to me, Doug says:

> By the 10th edition, the word list no longer needed to be compressed, and "part-of-speech" affixability codes had been added, thanks to work of Ruby Jane Elliott. The codes cut down on the acceptance of silly derivatives.

However, this version still uses the compressed dictionaries. He likely has a newer version of the code.

## Changes Made

The original code looks to have been written such that it would compile for both C and C++. I have removed the C++ bits and gone for straight C. Along the way, I fixed a core dump issue in the main program that apparently didn't happen on the Research VAX systems.

In addition, I've made use of modern headers and library bits that are standard, such as <tt>strdup()</tt> and <tt>EXIT_FAILURE</tt> and <tt>EXIT_SUCCESS</tt>.  In what I think are all the appropriate cases I've replaced <tt>0</tt> with either <tt>NULL</tt> or <tt>'\0'</tt>.

The original code was very sparing on white space; this seems to have been Doug's personal style. I have added more space to make the code a little bit easier on the eyes. (At least, on my eyes.)

The shell script driver no longer uses <tt>deroff</tt> or <tt>delatex</tt>. Neither of these is standard on Linux systems, and I am not aware of a version of <tt>deroff</tt> that understands the extended facilities of GNU <tt>troff</tt>.  Anyone who wants to help repair these deficiencies should be in touch.

The original code is tagged with <tt>v10-code-from-TUHS</tt>; check out that tag if you want to see it.

## Oddities

The <tt>amspell</tt> and <tt>brspell</tt> files differ slightly from those found in the tenth edition sources tarball. I'm not sure why. Nonetheless, they work ok. (Or at least, the <tt>amspell</tt> one does. I have not tested British spelling.)

Also, it seems that "prioritize" appears in both the standard list and in the list of American words. It likely should be removed from the standard list, but I haven't done that yet. (Doug McIlroy - if you have anything to say about this, please let me know.)

#### Last Updated
Tue Sep 24 19:59:22 IDT 2019
