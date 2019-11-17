# The Tenth Edition UNIX Spell Program

This is the `spell` program from the Tenth Edition Research UNIX
system.  It has been updated somewhat to compile on modern systems,
particularly on Ubuntu Linux.

## Installation

The `makefile` and the code have been modified so that installation
locations are easily changed; editing the `PREFIX` setting in the
`makefile` and then running `make` should be all that's necessary.

The program installs itself under the name `v10spell` to avoid
conflicts with any native spelling checkers already installed or
that might be installed in the future.

After editing the `makefile` (if necessary), it should be enough
to do:

	make
	sudo make install

## History

The original C version of the UNIX `spell` program was written by
Doug McIlroy for V7 UNIX.  The tenth edition version, also by Doug
McIlroy, is apparently a descendant of the original one, but able
to take advantage of the larger address space available on VAXen
as compared to PDP-11s.

Doug McIlroy published a paper on the original UNIX `spell`:

> _Development of a Spelling List_, M.D. McIlroy,
> IEEE Transactions on Communications, volume 30, pp 91-99, 1982.

It's available from his [home page](https://www.cs.dartmouth.edu/~doug/spell.pdf).

In private mail to me, Doug says:

> By the 10th edition, the word list no longer needed to be compressed,
> and "part-of-speech" affixability codes had been added, thanks to
> work of Ruby Jane Elliott.  The codes cut down on the acceptance of
> silly derivatives.

However, this version still uses the compressed dictionaries. He
likely has a newer version of the code.

## Changes Made

The original code looks to have been written such that it would
compile for both C and C++.  I have removed the C++ bits and gone
for straight C.  Along the way, I fixed a core dump issue in the
main program that apparently didn't happen on the Research VAX
systems.

In addition, I've made use of modern headers and library bits that
are standard, such as `strdup()` and `EXIT_FAILURE` and `EXIT_SUCCESS`.
In what I think are all the appropriate cases I've replaced `0`
with either `NULL` or `'\0'`.

The original code was very sparing on white space; this seems to
have been Doug's personal style.  I have added more space to make
the code a little bit easier on the eyes.  (At least, on my eyes.)

The shell script driver no longer uses `deroff` or `delatex`.  Neither
of these is standard on Linux systems, and I am not aware of a
version of `deroff` that understands the extended facilities of GNU
`troff`.  Anyone who wants to help repair these deficiencies should
be in touch.

The original code is tagged with `v10-code-from-TUHS`; check out
that tag if you want to see it.

## Oddities

The `amspell` and `brspell` files differ slightly from those found in
the tenth edition sources tarball.  Nonetheless, they work ok.  (Or at
least, the `amspell` one does.  I have not tested British spelling.)

I did some debugging.  (Debugging code can be found in the `debugging`
branch.) It looks like the word lists used to create the tenth edition
files differed from what's in the tarball.

#### Last Updated

Sun Nov 17 21:28:38 IST 2019
