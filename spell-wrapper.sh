#! /bin/sh -
### ====================================================================
### Add support to v10spell for additional command-line dictionaries
### supplied as options +dict1 +dict2 ... to match BSD/Solaris spell
### syntax.
###
### Usage:
###	v10spell.sh [ +dict1 +dict2 ...] infile(s)
###	v10spell.sh [ +dict1 +dict2 ...] < infile
###
### If A_SPELL points to a file, that file is the first private
### dictionary; otherwise, if $HOME/lib/spelldict is a file, it the
### first one.
###
### All command-line options that begin with a plus sign, and exist as
### files after the + is stripped, are added to the dictionary list.
### The contents of the complete list of dictionaries are then sorted
### into a temporary dictionary with unique words in traditional
### (POSIX or C locale) order, and supplied via A_SPELL to v10spell.
###
### The temporary dictionary is removed on exit or failure.
###
### Caveat: spaces in command line arguments break this script!
###
### Nelson H.F. Beebe, beebe@math.utah.edu
### [26-Sep-2019]
### ====================================================================

IFS='
 	'

PATH="$HOME/bin:/usr/uumath/bin:/usr/local/bin:/usr/bin:/bin"
export PATH

trap '/bin/rm -f $TMPSPELL' EXIT HUP INT QUIT PIPE TERM

TMPSPELL=${TMPDIR-/tmp}/v10spell.tmp.$$

touch $TMPSPELL

DICTFILES=

## If A_SPELL points to a file, it overrides the user default dictionary
for d in $A_SPELL $HOME/lib/spelldict
do
    test -f $d && DICTFILES="$DICTFILES $d" && break
done

ARGS=

for arg in "$@"
do
    case "$arg" in
    +*) DICTFILES="$DICTFILES `echo $arg | sed -e 's/^[+]//'`" ;;
    *)  ARGS="$ARGS $arg" ;;
    esac
done

## Combine all exception dictionaries into a
### unique list in traditional (POSIX, C) order
LC_ALL=C LANG=C sort -u $DICTFILES > $TMPSPELL

A_SPELL=$TMPSPELL v10spell $ARGS
