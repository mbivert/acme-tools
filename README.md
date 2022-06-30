# Introduction
We provide some tools thereafter described, aiming at reducing the amount of windows,
and ease to some extent Acme's scriptability, pushing
[lib/acme.rc](https://github.com/9fans/plan9port/blob/master/lib/acme.rc) a bit further.

## "+Buffer" window
In practice, we found that having a single *+Errors* window, with automatic
content flushing, is more suitable than spanning one *+Errors* window per directory,
with no flushing.

*NOTE*: acme's default seem however most reasonable: better be verbose,
and let the user filtrate as needed.

By convention, we called this window the *+Buffer* window, being the
only window whose name suffixed by *+Buffer*. Its access is automatically
managed via the *Getids* command, described thereafter.

We furthermore use that window's tagline for running global commands,
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
Again aiming at reducing windows proliferation, we provide three
small scripts allowing to load/dump current windows' states:

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

## -

    NAME
    	-
    
    SYNOPSYS
    	- [-h]
    
    DESCRIPTION
    	- unindents text from stdin by removing a heading tab from
    	every line; resulting output on stdout.

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
    	Exec [-t [-a|id|pattern]] [-e [-a|id|pattern]] <cmd>
    	Exec [-o [-a|id|pattern]] <cmd>
    
    DESCRIPTION
    	Exec runs a command, sending stdout/stderr to acme buffers,
    	defaulting to +Buffer.
    
    	The buffers are automatically cleaned before hand.
    
    	-t selects stdout buffer, forwarding [-a|id|pattern] to Getids
    	-t selects stderr buffer, forwarding [-a|id|pattern] to Getids
    
    	-o is a shortcut to set both -t and -e.

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

## Open

    NAME
    	Open
    
    SYNOPSYS
    	Open [-h]
    	Open [-m] [-p] [path/to/file, ...]
    
    DESCRIPTION
    	Open given files to acme if any, creating them as an empty files
    	if necessary.
    
    	If -m is specified, move back to / and open the first file
    	that match given name. Note that this mimick Run's -m behavior,
    	hence the option's name.
    
    	If -p is specified, print opened files' acme windows' IDs,
    	one per line.

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
    	and related files from filesystems.

## Run

    NAME
    	Run
    
    SYNOPSYS
    	Run [-h]
    	Run [-t [-a|id|pattern]] [-e [-a|id|pattern]] [-m] [args]
    	Run [-o [-a|id|pattern]]                      [-m] [args]
    
    DESCRIPTION
    	Run is a smart script running a command automagically deduced
    	from filename, redirecting its stdout/stderr to the buffers
    	pointed by -t/-e/-o, after having written all buffers.
    
    	User is expected to complete/adjust this script following
    	his need.
    
    	The buffers are automatically cleaned before hand.
    
    	-t selects stdout buffer, forwarding [-a|id|pattern] to Getids
    	-t selects stderr buffer, forwarding [-a|id|pattern] to Getids
    
    	-o is a shortcut to set both -t and -e.
    
    	-m will force the use of a Makefile by going up to / from current
    	buffer's file location until a Makefile is found.
    
    EXAMPLES
    	By default, on a buffer pointing to a Makefile, 'Run tests' will:
    		- Put all buffers
    		- Run 'make tests', redirecting stdin/stdout to '+Buffer', creating
    		  it if necessary, and cleaning it beforehand.

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
    

