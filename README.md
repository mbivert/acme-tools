# Introduction
We provide some tools thereafter described, aiming at reducing the amount of windows,
and ease to some extent Acme's scriptability, pushing
[lib/acme.rc](https://github.com/9fans/plan9port/blob/master/lib/acme.rc) a bit further.

## Run
This is perhaps the most useful, at least to me. Most others tools were developed
mainly in order to create [``Run``][gh-mb-at-run]. It aims at implementing a typical
"build/run" feature, more precisely, it:

  - Puts (save) all opened files (see [``XPutall``][gh-mb-at-xputall]);
  - Opens if necessary an output buffer/window (by default, it
  targets the ``+Buffer`` window, described in the
  [next subsection](https://github.com/mbivert/acme-tools#buffer-window)),
  and clearing its content (see [``Clear``][gh-mb-at-clear]) if necessary
  - Executes (see [``Exec``][gh-mb-at-exec], [``To``][gh-mb-at-to]) the
  program related to the current buffer (there's an ad-hoc
  long list of ``if/else`` in [``Run``][gh-mb-at-run] to determine
  what to do);
  - Redirecting stderr/stdout to the output buffer along the way,
  eventually tweaking stderr's output by piping it to a program
  (this for instance allows to rewrite some error message to
  be more [acme(1)](http://man.cat-v.org/plan_9/1/acme)/[plumber(4)](http://man.cat-v.org/plan_9/4/plumber) compliant).

Of course, CLI arguments can be provided to the program to be executed,
for instance: ``Run -- -o foo -x bar baz`` will feed the program
``-o foo -x bar baz`` as CLI arguments.

A convenient ``-m <target>`` option will execute ``make <target>`` in
the first directory containing a ``Makefile``, starting from the current
directory, and going up in the tree until it reaches ``/``. For instance, when:

  - middle clicking ``Run -m tests`` on the tagline of a
  file ``/home/user/gits/project/dir/subdir/foo.go``;
  - assuming a ``/home/user/gits/project/Makefile`` exists;
  - will run ``make tests`` in ``/home/user/gits/project/`` (of course,
  surrounded by all previous boilerplate: saving files, opening output
  buffer, etc.)

[gh-mb-at-run]:           https://github.com/mbivert/acme-tools/blob/master/bin/Run
[gh-mb-at-clear]:         https://github.com/mbivert/acme-tools/blob/master/bin/Clear
[gh-mb-at-exec]:          https://github.com/mbivert/acme-tools/blob/master/bin/Exec
[gh-mb-at-xputall]:       https://github.com/mbivert/acme-tools/blob/master/bin/XPutall
[gh-mb-at-to]:            https://github.com/mbivert/acme-tools/blob/master/bin/To

## "+Buffer" window
In practice, I've found that having a single *+Errors* window, with automatic
content flushing, is more suitable than spanning one *+Errors* window per directory,
with no flushing.

*NOTE*: acme's default seem however most reasonable: better be verbose,
and let the user filter as needed.

By convention, we called this window the *+Buffer* window, being the
only window whose name suffixed by *+Buffer*. Its access is automatically
managed via the *Getids* command, described thereafter.

This window's tagline can be used for running global commands,
such as the session-related ones, also described later on.

## Getids
*Getids* is a key script, on which most other scripts are built upon.
It allows to easily target:

  - the *+Buffer* window (creating it when necessary);
  - multiple windows (X//-like looping);
  - current window (the default).

It outputs a list of window IDs, one per line, from either:

  - a number (window ID);
  - an awk(1) regular expression;
  - **-a** for **a**utomatically targeting the *+Buffer* window;
  - nothing (current window).

As an example, a script like *Do* that runs a command from a window's
tagline, thus allowing to run/chain acme(1) commands, has an optional
argument that is forwarded to *Getids*:

    # Clean current window
    (tagline)$ Do 'Edit ,d'

    # Clean the content of all +Errors windows
    (sh|tagline)$ Do 'Edit ,d' '\+Errors$'

    # Write is used to store data in special buffer's files (9P),
    # similar to lib/acme.rc's winwrite().
    #
    # The following writes and closes all windows whose name contain
    # "/project42/", cleaning the +Buffer window's content thereafter,
    # creating it if  necessary.
    (sh|tagline)$ Write ctl put '\/project42\/'    && \
                  Write ctl delete '\/project42\/' && \
                  Do 'Edit ,d' -a

## XDump, XLoad, Switch (sessions)
Again aiming at reducing windows proliferation, those three
small scripts load/dump current windows' states:

    # Put all files and creates a dump file in
    # $HOME/acme.dumps/project42.dump with current state
    (sh|tagline)$ XPutall && XDump project42 # optional .dump extension

    # Restore state from $HOME/acme.dumps/projects42.dump,
    # closing all opened windows beforehand
    (sh|tagline)$ XLoad project42.dump       # optional .dump extension

    # Dump current state to $HOME/acme.dumps/project42.dump and
    # load session $HOME/acme.dumps/website.dump
    (sh|tagline)$ Switch project42 website   # optional .dump extension

# Tools (do not edit, automatically generated cf. Makefile)
## +

    NAME
    	+

    SYNOPSYS
    	+ [-h]

    DESCRIPTION
    	+ indents text from stdin by prepending a tab to
    	every line; resulting output on stdout.

## ++

    NAME
    	++

    SYNOPSYS
    	++ [-h]

    DESCRIPTION
    	++ indents text from stdin by prepending 2 tabs to
    	every line; resulting output on stdout.

## +++

    NAME
    	+++

    SYNOPSYS
    	+++ [-h]

    DESCRIPTION
    	+++ indents text from stdin by prepending 3 tabs to
    	every line; resulting output on stdout.

## -

    NAME
    	-

    SYNOPSYS
    	- [-h]

    DESCRIPTION
    	- unindents text from stdin by removing a heading tab from
    	every line; resulting output on stdout.

## --

    NAME
    	--

    SYNOPSYS
    	-- [-h]

    DESCRIPTION
    	-- unindents text from stdin by removing 2 heading tabs from
    	every line; resulting output on stdout.

## ---

    NAME
    	---

    SYNOPSYS
    	--- [-h]

    DESCRIPTION
    	--- unindents text from stdin by removing 3 heading tab from
    	every line; resulting output on stdout.

## Acme

    NAME
    	Acme

    SYNOPSYS
    	Acme [-h]
    	Acme [args...]

    DESCRIPTION
    	Acme is a basic wrapper to launch acme(1). It starts
    	the plumber if needed, set a few convenient environment
    	variable, and will periodically create a
    	/home/mb/acme.dumps/autodump.dump using "XDump autodump".

## Auto

    NAME
    	Auto

    SYNOPSYS
    	Auto [-h]
    	Auto [-k] [cmd]
    	Auto [-n seconds] [-r] [cmd]

    DESCRIPTION
    	Auto will periodically run the given command from the tagline
    	where Auto was started.

    	-n allows to specify a running period (default: 10 seconds)
    	-k will kill either all automatic jobs associated with the concerned
    	window, or all the ones associated with the given command.
    	-r will first kill all automatic jobs associated with the given
    	command before (re)starting it

    	Unless for -k, where we look for all commands, cmd defaults
    	to "Run".

    EXAMPLES
    	Running 'Auto -n 5 -r "Run -- -k"' from a window will:

    		- kill all "Run -- -k" jobs associated to $winid;
    		- run "Run -k" from the tagline every 5 seconds.

    	Running 'Auto' from a window will:

    		- Execute 'Run' from that tagline every 10 seconds.

## Clear

    NAME
    	Clear

    SYNOPSYS
    	Clear [-h]
    	Clear [-a|id|pattern]

    DESCRIPTION
    	Clear removes all content from the windows pointed by -a|id|pattern,
    	which is forwarded to Getids, thus defaulting to $winid (current
    	window).

## Delall

    NAME
    	Delall

    SYNOPSYS
    	Delall [-h]
    	Delall

    DESCRIPTION
    	Delall deletes all existing windows.

## Do

    NAME
    	Do

    SYNOPSYS
    	Do [-h]
    	Do <cmd> [-a|id|pattern]

    DESCRIPTION
    	Do runs a command <cmd> in the taglines of the windows pointed by
    	[-a|id|pattern], which is forwarded to Getids, thus defaulting to
    	$winid (current window).

    	Similar to https://github.com/mkhl/cmd/blob/master/acme/acmeeval/main.go,
    	but written in sh(1), orthogonal with Getids's behavior (+Buffer
    	management, and X// like looping on filenames patterns.

    EXAMPLES
    	$ Do 'Edit ,d' -a
    		Clear '+Buffer' window's body

## Exec

    NAME
    	Exec

    SYNOPSYS
    	Exec [-h]
    	Exec [-t [-a|id|pattern]] [-e [-a|id|pattern]]<cmd>
    	     [-o [-a|id|pattern]] [-p outp] [-q errp] [-r both] <cmd>

    DESCRIPTION
    	Exec runs a command, sending stdout/stderr to acme buffers,
    	defaulting to +Buffer.

    	The buffers are automatically cleaned before hand.

    	-t selects stdout buffer, forwarding [-a|id|pattern] to Getids
    	-t selects stderr buffer, forwarding [-a|id|pattern] to Getids

    	-o is a shortcut to set both -t and -e.

    	-p and -q allows to pipe stdout/stderr through the given shell
    	commands, before forwarding the result to To, which will be in
    	charge of writing the content to an acme(1) buffer.

    	-r allows to set both outp and errp at once.


## Getall

    NAME
    	Getall

    SYNOPSYS
    	Getall [-h]
    	Getall

    DESCRIPTION
    	Getall updates all windows from filesystem (runs Get everywhere).

## Getfn

    NAME
    	Getfn

    SYNOPSYS
    	Getfn [-h]
    	Getfn [-a|id|pattern]

    DESCRIPTION
    	Getfn prints the filenames of windows pointed by [-a|id|pattern],
    	which is forwarded to Getids, thus defaulting to $winid
    	(current window).

    	This is a nice shortcut to avoid callers to handle multi-lines tag.

## Getids

    NAME
    	Getids

    SYNOPSYS
    	Getids [-h]
    	Getids [-a|id|pattern]

    DESCRIPTION
    	This is a key script; goals are:

    		- to transform a pattern to a list of acme buffer IDs;
    		- allow easy targetting the special '+Buffer' window;
    		- to default to current window ($winid).

    	When called with no arguments, simply display $winid, failing
    	with an error code if it is not set.

    	When called with -a, look for a +Buffer window, creating it
    	if needed, and display its id.

    	When called with a number, assume an existing windo id, and
    	display it.

    	Otherwise, assume argument is an awk(1) pattern, and display
    	all matching buffers IDs, one per line.

## Godoc

    NAME
    	Godoc

    SYNOPSYS
    	Godoc [-h]
    	Godoc [args...]

    DESCRIPTION
    	Godoc executes go doc with the given arguments. This
    	is a shortcut for:
    		$ Exec go doc [args...]


## Mv

    NAME
    	Mv

    SYNOPSYS
    	Mv [-h]
    	Mv <to> [-a|id|pattern]

    DESCRIPTION
    	Move (mv(1)) current file, or the one pointed by [-a|id|pattern],
    	to the new one, updating acme's buffer's name accordingly.

    	If to doesn't start with a leading '/', renaming is performed
    	relatively instead of absolutely.

    	If to describes a missing path, intermediate directories
    	are created (mkdir(1) -p); existing file is overwritten,
    	following mv(1)' semantic.

    EXAMPLE
    	Rename from "/home/foo/bar.c" to "/home/foo/baz.c":
    		Mv /home/foo/baz.c
    		Mv baz.c

    	Renaming "/tmp/bar/" to "/tmp/foo/":
    		Mv foo
    		Mv foo/

    BUGS:
    	We may want to allow regexp(7) with capture, so as to allow
    	renaming multiple files/buffers at once.


## Open

    NAME
    	Open

    SYNOPSYS
    	Open [-h]
    	Open [-m] [-n] [-p] [-u] [-x] <name> [pattern]

    DESCRIPTION
    	Open given file with acme, creating it as an empty file
    	if necessary.

    	If -m is specified, move back to / and open the first file
    	that match given name. Note that this mimick Run's -m behavior,
    	hence the option's name.

    	If -n is specified, the file nor its parent directory will
    	be created on disk/loaded from disk.

    	If -p is specified, print opened files' acme windows' IDs,
    	one per line.

    	If -u is specified, Open will only open the file if no buffer
    	matches the given pattern. If no pattern is specified, the
    	name will be used as a pattern instead.

    	If -x is specified, a chmod +x on the file is automatically
    	performed.

    	Paths starting with '/' are considered absolutely, otherwise,
    	relatively.

    	Nonexistent paths are created (mkdir(1) -p).

    EXAMPLES
    	The following will create a '+Buffer' window if there's no
    	window with filename suffixed by +Buffer; this will be
    	considered a "virtual" file, not tied to an on-disk file:

    		Open -n -u '+Buffer' '\+Buffer$'


## Read

    NAME
    	Read

    SYNOPSYS
    	Read [-h]
    	Read [-a|id|pattern]

    DESCRIPTION
    	Read reads the body of the buffer pointed by [-a|id|pattern]
    	and prints it to stdout.

    EXAMPLES
    	Display '+Buffer's content on stdout:
    		Read -a

    	Look for a buffer suffixed by '+Files'' and displays its
    	content on stdout:
    		Read '\+Files'

## Rename

    NAME
    	Rename

    SYNOPSYS
    	Rename [-h]
    	Rename [-d dot] <from> <to> [-a|id|pattern]

    DESCRIPTION
    	Rename renames an identifier in current selection of the
    	windows pointed by [-a|id|pattern], forwarded to Getids,
    	thus defaulting to $winid (default window), using the
    	famous sam(1) idiom described in sam_tut.pdf.

    	By default, renaming is performed on the whole file (dot=,);
    	the -d option allows to narrow the scope by setting the
    	dot.

    EXAMPLE
    	Rename identifiers from "fs" to "ifs" on current window:
    		Rename fs ifs

    	Same, but perform renaming on current selection only:
    		Rename -d '' fs ifs
    		Rename -d .  fs ifs

    	Same, but on all opened C files/headers:
    		Rename -d . fs ifs '\.[ch]$'


## Rm

    NAME
    	Rm

    SYNOPSYS
    	Rm [-h]
    	Rm [-a|id|pattern]

    DESCRIPTION
    	Rm removes buffers pointed by [-a|id|pattern], forwarded
    	to Getids, thus defaulting to $winid (current window),
    	and related files/directories from filesystems.

## Run

    NAME
    	Run

    SYNOPSYS
    	Run [-h]
    	Run [-t [-a|id|pattern]] [-e [-a|id|pattern]] [-m] [args]
    	Run [-o [-a|id|pattern]]                      [-m] [args]

    DESCRIPTION
    	Run is a smart script running a command automagically inferred
    	from file/filename, redirecting its stdout/stderr to the buffers
    	pointed by -t/-e/-o, after having written all buffers.

    	User is expected to complete/adjust this script following
    	his need. In particular, the $errp variable allows to
    	pipe stderr to a command before sending to the error buffer
    	pointed by -e/-o: this can be used for instance to adjust
    	error messages to match plumbing rules.

    	The buffers are automatically cleaned before hand.

    	-t selects stdout buffer, forwarding [-a|id|pattern] to Getids
    	-t selects stderr buffer, forwarding [-a|id|pattern] to Getids

    	-o is a shortcut to set both -t and -e.

    	-m will force the use of a Makefile by going up to / from current
    	buffer's directory, until a Makefile is found.

    EXAMPLES
    	1. By default, on a buffer pointing to a Makefile, 'Run tests' will:
    		- Put all buffers
    		- Run 'make tests', redirecting stdin/stdout to '+Buffer', creating
    		  it if necessary, and cleaning it beforehand.

    	2. If executing 'Run -m tests' on a file /home/user/project/lib/lib.go,
    	and if a /home/user/project/Makefile file exist, will use that
    	makefile in a 'make tests'.

    	3. With the default setup, 'Run -- -m x' on a .c file will compile
    	the C file (assuming single file project) and execute the resulting
    	binary with '-m x' as CLI arguments.

## See

    ./bin/See [-n]

## Switch

    NAME
    	Switch

    SYNOPSYS
    	Switch [-h]
    	Switch    <from> <to>
    	Switch -r <to>   <from>

    DESCRIPTION
    	Switch dumps current state to from and loads the state described
    	by to.

## To

    NAME
    	To

    SYNOPSYS
    	To [-h]
    	To [-a|id|pattern]

    DESCRIPTION
    	To redirects stdin/stdout to the first window pointed by
    	[-a|id|pattern]. We default to -a (+Buffer file).

    	If there's no buffer to write to, act as a cat(1).

    EXAMPLES
    	# Display stat(1)'s output for current buffer in +Buffer,
    	# creating it if necessary.
    	(tagline)$ stat $% | To

## Write

    NAME
    	Write

    SYNOPSYS
    	Write [-h]
    	Write <file> <data> [-a|id|pattern]

    DESCRIPTION
    	Write writes data to buffer's special file (ctl/addr/body/data, etc.),
    	for all files pointed by [-a|id|pattern], forwarded to Getids,
    	thus defaulting to $winid (current window).

    EXAMPLES
    	# Deletes all existing windows.
    	(sh|tagline)$ Write ctl delete '.*'

    	# Appends "hello" to current window's body
    	(sh|tagline)$ Write body hello

## XDel

    NAME
    	XDel

    SYNOPSYS
    	XDel [-h]
    	XDel [-a|id|pattern]

    DESCRIPTION
    	XDel stores the content of the buffers pointed by [-a|id|pattern],
    	and then proceed to close the buffer, while storing
    	their filenames to the '+Files' buffer if necessary. The
    	'+Files' buffer is created is necessary.


## XDump

    NAME
    	XDump

    SYNOPSYS
    	XDump [-h]
    	XDump <session>

    DESCRIPTION
    	XDump dumps current state to <session>. <session> is automatically
    	appended a .dump suffix if needed, and is stored as a file in
    	$HOME/acme.dumps/

## XLoad

    NAME
    	XLoad

    SYNOPSYS
    	XLoad [-h]
    	XLoad <session>

    DESCRIPTION
    	XLoad loads the state described by <session>. <session> is automatically
    	appended a .dump suffix, and is looked as a file in $HOME/acme.dumps/.

    	We nuke all existing windows before hand, as Acme's Load just
    	add windows.


## XPut

    NAME
    	XPut

    SYNOPSYS
    	XPut [-h]
    	XPut [-a|id|pattern]

    DESCRIPTION
    	XPut stores files pointed by [-a|id|pattern], forwarded to Getids,
    	thus defaulting to $winid (current window).

    EXAMPLES
    	# Writes all windows
    	(sh|tagline)$ XPut '.*'

## XPutall

    NAME
    	XPutall

    SYNOPSYS
    	XPutall [-h]
    	XPutall [-a|id|pattern]

    DESCRIPTION
    	XPutall stores all buffers (runs Put everywhere).


