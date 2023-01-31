# Default installation directory.
#	make install dir=$HOME/bin
dir ?= /bin/
root ?= root
group ?= root

# XXX the hook is conceptually
# on a per-user basis
user ?= mb
hdir ?= /home/${user}/

.PHONY: all
all: bin/See

.PHONY: help
help:
	@echo 'install dir=... group=... root=...'
	@echo '            install bin/* to $dir (default /bin/);'
	@echo '            default owner:group is root:root'
	@echo 'uninstall dir=...'
	@echo '            uninstall bin/* from $dir (default: /bin/)'
	@echo 'update-doc: update README.md from inline bin/* doc'

bin/See: See.c
	@echo Compiling See...
	@9c See.c && 9l See.o -o bin/See && rm See.o

.PHONY: install
install: bin/See install-hook
	@echo "Installing bin/* to ${dir}/..."
	@for x in bin/*; do \
		install -o ${root} -g ${group} -m 755 $$x ${dir}/`basename $$x`; \
	done
	@# Convenient shortcut
	@rm -f ${dir}/O
	@ln -s ${dir}/Open ${dir}/O

install-hook: acme.hook
	@echo "Installing hook to ${hdir}..."
	@install -m 755 ./acme.hook ${hdir}/acme.hook

.PHONY: uninstall
uninstall:
	@echo "Removing all bin/* from ${dir}/..."
	@for x in bin/*; do echo rm -f ${dir}/`basename $$x`; done
	@rm -f ${dir}/O

.PHONY: update-doc
update-doc:
	@echo 'Updating README.md to include inline documentation...'
	@sh ./update-doc '^# Tools' ./README.md ./bin/
