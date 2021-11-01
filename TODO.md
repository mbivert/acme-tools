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
e.g. `Exec 9 man ls`

# Single window directory navigation @single-window-dir-nav #medium
See.c is an example of a solution, but perhaps we could
experiment with other techniques.

For instance, watching for special events, or relying on plumbing
rules.

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
