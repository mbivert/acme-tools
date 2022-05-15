# XDump accumulating in main tagline @many-xdump-tagline
To be tested, but likely, XDump stores many "XDump" in
the dump file's tagline. If so, we could/should trim
them.

See also @auto-xdump-file.

# Do : option to avoid updating +Buffer directory @do-no-chdir
Or at least a mechanism so that our autodump mechanism
(XDump) restores it after the dump.

# Persistent Run/Exec on Switch @keep-exec-on-switch
Existing Run outputing in a given +Buffer seems to
die out of +Buffer being deleted/recreated on a Switch.

Simplest way would be to make +Buffer permanent in XLoad(/Delall),
but this wouldn't solve the issue in the general case.

Another option would be that all Exec would register/tag their
output windows, so that Delall wouldn't delete them. (e.g.
/tmp/+Buffer -> /tmp/+Keep+Buffer).

# optional/configurable autodump @autodump-Acme
Allow automatic reload of autodump in Acme on startup.
Allow autodump period configuration
Allow to disable autodump on startup.
Document in README.md

Perhaps have autodump in an external script.

# Ad-hoc buttons/scripts
Have a file with two fields:

  <regexp> <script>

When opening a file matching regexp, append to its tagline
the second field (ad-hoc script name)

Have a way to automatically create ad-hoc script and add them
to the previous file. E.g.

AddButton QTests 'XPutall && Exec go test -v *.go':
	- creates an ad-hoc QTests executable scripts in ~/acme.ad-hoc/,
	which is added to the $PATH in ./Acme
	- error if script name already exists
	- add line "^$%$ QTests" to ~/.acme.ad-hoc
	- eventually, clean the tag of the corresponding window
	to include QTests instead of the full line

# Open -m directories @open-dir-support
E.g. `Open -m static(/)` could climb up to / and open
the first occurence of it.

# Rename bug [] @rename-bug-non-standard-chars
Rename breaks e.g. if string to rename contains [].

# Acme -l to look for in $HOME/acme.dumps/ @acme-dump-semantic
./Acme could wrap -l as such:

  - if dump file isn't found, try to look for it in $HOME/acme.dumps/
  instead

# Automatic dump file naming @auto-xdump-file
Perhaps we could store "somewhere" a special token, "xdump:<name>"
to automatically name the dump files used by XDump?

# XPutall wrong exit code @xputall-exit-code
To be tested: "XPutall && echo ok" should fail when at least one
file cannot be written to.

# Shortcuts @shortcuts
The ability to register a few convenient shortcuts without
hacking .c code would be helpful. ^L on win(1)dows would be
a good testbed.

We can retrieve current window through

  9p read acme/log | awk '$2 == "focus" { print $1 }'

See https://github.com/fhs/acme-lsp/blob/v0.10.0/cmd/acmefocused/main.go,
and more generally, https://github.com/fhs/acme-lsp

Could be started from gits/acme-tools/Acme, always writing last
ID to some well-known location. Then we could rely on
[xbindkeys](http://www.nongnu.org/xbindkeys/xbindkeys.html) to
actually handle the shortcut.

# LookWin @lookwin
A LookWin command that would look for the first window matching
a given pattern and select it/place the cursor on it (do-able?)

# document base.dump @base.dump-doc #minor
cf. acme.bin/XLoad; also XLoad -l

# rc(1) implementation @rc-implementation #minor
Currently all tools are implemented in, hopefully, portable
sh(1).

# Clarify Plan9 vs. POSIX standard tools @plan9-vs-posix #minor
Typically, sed(1) behaves differently in POSIX and Plan9.

There are some hardcored /bin/sed, e.g. in acme.bin/+.

If we have @rc-implementation, we could enforce POSIX-only
in sh implementation, and Plan9-only in rc implementation.

# +Errors automatic cleaning @errors-removal #medium
+Errors windows are noisy (one per directory, breaks user
flow, etc.). +Buffer was introduced as a mean to reduce them.

Perhaps we could also watch for events targeting a +Errors
window and automatically redirect all that to +Buffer?

Current solution is mostly to prefix commands with a Exec,
e.g. `Exec 9 man ls`; we barely have a need for +Errors
windows anymore.

# Single window directory navigation @single-window-dir-nav #medium
See.c is an example of a solution, but perhaps we could
experiment with other techniques.

For instance, watching for special events, or relying on plumbing
rules.

Or assuming this is technically possible, a shortcut like ctrl-click that would:

  - when performed in body, append selected text to window's current
  location
  - when performed on tag, replace window's current location

Again, the goal is to reduce noise/number of opened window while
keeping a fluid workflow.

# Window position scripting @window-positioning #hacky
Acme's Load doesn't destroy existing windows by default. We should
thus be able to craft special dump files allowing scriptable windows
(re)positioning, without having to alter C code.

But at that point, we may want to just use Emacs™.

# Rm fails if file does not exists @resiliant-rm #minor
rm -f should do.

# Mv @mv #small
Implement a Mv command similar to Rm:

    Mv relative/path
    Mv /absolute/path

    Rename current buffer, automatically put new buffer
    & remove previous name or mv filesystem.

# Avoid double clean with Exec @exec-no-double-clean #minor
We're a bit quick in Exec to avoid double cleaning. This
should be practically negligible.

Happens e.g. in the case of  -e foo -t bar, where both foo
and bar patterns, different strings, would point to the same
buffer.

# awk(1) pattern forwarding @awk-pattern-forwarding #medium
In commands such as Getfn that takes an awk(1) pattern as
an argument, user needs to specially quotes the pattern as
a consequence. This can be unexpected.

E.g.
	Getfn '\/home\/$USER/gits/project\/'

The forwarding of the pattern can be discussable to, cf.
acme.bin/Rm (no quotes).

# documentation double-check @review-doc #small
Uniform documentation was written for the command, but not
yet doubled check; mistakes are likely.

# man pages @man-pages #medium
We could extract documentation from script as currently done
for the README.md and creates some man pages for the scripts.
