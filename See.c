#include <u.h>
#include <libc.h>
#include <thread.h>
#include <9pclient.h>
#include <acme.h>

/*
 * NOTE: this was written a long time ago.
 *
 * Going back to previous directories is nicely handled as
 * side effect when mouse search the window's title:
 *	- click (M3) in the window's name after the slash of the
 *	path you want to reach
 *	- push mouse up and release
 */

enum {
	Pathmax = EVENTSIZE*UTFmax+1,
};

char cwd[Pathmax];

// if -n, files will be opened to fwin.
Win *win, *fwin;

void
cd(char *s)
{
	int n;

	memset(cwd, 0, sizeof cwd);
	if (*s == '/') {
		if (strlen(s)+1 > sizeof(cwd))
			goto err;
		strcpy(cwd, s);
	} else {
		getwd(cwd, sizeof(cwd));
		n = strlen(cwd);
		if (cwd[n] != '/')
			cwd[++n] = '/';
		if (strlen(s)+1+n > sizeof(cwd))
			goto err;
		strcat(cwd, s);
	}
	cleanname(cwd);

	winname(win, "%s", cwd);

	winctl(win, "get");
	return;

err:
	sysfatal("directory too long, increase Pathmax (%d)", Pathmax);
}

/*
 * from $PLAN9/src/cmd/netfiles/acme.c
 * The pipe allows to leave the file window in a clean state.
 */
int
pipetowin2(Win *w, char *name, int errto, char *cmd, ...)
{
	va_list arg;
	char *p;
	int fd[3], pid, pfd[2];
	char buf[1024];
	int n;

	/*
	 * cannot use winfd here because of buffering caused
	 * by pipe.  program might exit before final write to acme
	 * happens.  so we might return before the final write.
	 *
	 * to avoid this, we tend the pipe ourselves.
	 */
	if(pipe(pfd) < 0)
		sysfatal("pipe: %r");
	va_start(arg, cmd);
	p = evsmprint(cmd, arg);
	va_end(arg);
	fd[0] = open("/dev/null", OREAD);
	fd[1] = pfd[1];
	if(errto == 0)
		fd[2] = dup(fd[1], -1);
	else
		fd[2] = dup(errto, -1);
	pid = threadspawnl(fd, "rc", "rc", "-c", p, 0);
	free(p);
	while((n = read(pfd[0], buf, sizeof buf)) > 0)
		winwrite(w, name, buf, n);
	close(pfd[0]);
	return pid;
}

void
opentofwin(char *fn)
{
	winname(fwin, "%s", fn);
	winwrite(fwin, "addr", ",", 1);
	winwrite(fwin, "ctl", "dot=addr", 8);
	waitfor(pipetowin2(fwin, "data", 0, "%s %s", "cat", fn, NULL));
	winwrite(fwin, "ctl", "clean", 5);
}

int
isdir(char *s)
{
	Dir *d;
	int ret;

	d = dirstat(s);

	if (d == nil)
		return 0;

	ret = d->mode & DMDIR;

	free(d);

	return ret;
}

void
readevent(Event *e)
{
	if (e->c2 == 'L' || e->c2 == 'l') {
		if (isdir(e->text))
			cd(e->text);
		else if (fwin != nil)
			opentofwin(e->text);
	} else if (e->c2 == 'X' || e->c2 == 'x') {
		if (strncmp(e->text, "Del", 3) == 0) {
			windel(win, 1);
			winfree(win);
			if (fwin != nil) {
				windel(fwin, 1);
				winfree(fwin);
			}
			threadexitsall(nil);
		}
	}
	winwriteevent(win, e);
}

void
threadmain(int argc, char *argv[])
{
	Event e1, e2;

	// cd requires win to be initialized first
	win = newwin();
	fwin = nil;

	ARGBEGIN {
	case 'n':
		fwin = newwin();
		break;
	case 'h':
		fprint(2, "%s [-n]\n", argv[0]);
		threadexitsall(nil);
		break;
	default:
		print("unknown flag '%c'\n", ARGC());
		break;
	} ARGEND

	// XXX $HOME set but not $home
	putenv("home", getenv("HOME"));

	if (argc > 1)
		cd(argv[1]);
	else
		cd(getenv("home"));

	for (;;) {
		winreadevent(win, &e1);

		if (e1.c1 == 'M') {
			readevent(&e1);
			if (e1.flag & 2) {
				winreadevent(win, &e2);
				readevent(&e2);
			}
			continue;
		}
		winwriteevent(win, &e1);
	}
}
