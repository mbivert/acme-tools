# Default installation directory.
#	make install dir=$HOME/bin
dir ?= /bin/
root ?= root
group ?= root

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
install: bin/See
	@echo "Installing bin/* to ${dir}/..."
	@for x in bin/*; do \
		install -o ${root} -g ${group} -m 755 $$x ${dir}/`basename $$x`; \
	done

.PHONY: uninstall
uninstall:
	@echo "Removing all bin/* from ${dir}/..."
	@for x in bin/*; do echo rm -f ${dir}/`basename $$x`; done

.PHONY: update-doc
update-doc:
	@echo 'Updating README.md to include inline documentation...'
	@sh ./update-doc '^# Tools' ./README.md ./bin/
