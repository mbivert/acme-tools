.PHONY: help
help:
	@echo 'install:    install acme.bin/ to $$HOME/acme.bin/ and Acme to $$HOME/bin/Acme'
	@echo "update-doc: update README.md from inline acme.bin/* documentation"

.PHONY: install
install:
	@echo 'Installing acme.bin to $$HOME/acme.bin/'
	@if [ ! -d ${HOME}/acme.bin ]; then mkdir ${HOME}/acme.bin; fi
	@cp acme.bin/* ${HOME}/acme.bin/
	@chmod +x ${HOME}/acme.bin/* ${HOME}/bin/Acme
	@echo 'Installing Acme to $$HOME/bin/'
	@cp Acme ${HOME}/bin/
	@chmod +x ${HOME}/bin/Acme

update-doc:
	@echo Updating README.md to include inline documentation...
	@cp README.md /tmp/README.backup.$$
	@sed '/^# Tools/q' ./README.md > /tmp/README.$$
	@for x in ./acme.bin/*; do echo '##' `basename $$x`;echo;sh $$x -h | sed 's/^/    /';echo;done >> /tmp/README.$$
	@mv /tmp/README.$$ ./README.md
