# changewait
File watching utility for FreeBSD and Mac OS X.

## Introduction
The `changewait` utility allows you to wait on a change to a file of your
choice. It is able to listen for all kinds of changes: writes, reads, 
attribute changes, cases where someone removes the file or revokes the 
access to it, renames the file or changes the hardlink count.

## Usage
The program expects zero or mode optional flags, specifying the set of
changes that should be watched and a path to a file. In case that no flag 
was specified, all event types are considered.

## Flags
 * `-a` the file attributes changed
 * `-d` the file was deleted
 * `-e` the file was extended
 * `-l` the number of links to the file changed
 * `-m` the file was moved (renamed)
 * `-r` the access rights to the file were revoked
 * `-w` the file has been written into

## Example 
The utility alone has not much use, yet it is a great addition to 
your shell script automation workflow.

```sh
#!/bin/sh

while true
do
	changewait thesis.tex
	pdflatex thesis.tex
	xpdf thesis.pdf
done
```

## Build & install 
```
$ clang -o changewait main.c
$ sudo mv changewait /usr/bin
```

## License
2-clause BSD license. For more information please consult the
[LICENSE](LICENSE.md) file. In the case that you need a different license, 
feel free to contact me.

## Author
Daniel Lovasko (lovasko@freebsd.org)

