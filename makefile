##############################################################################
# (c) SZABO Gergely, 2009
# Free software, distributed under the GNU GPL v3 license
# There is absolutely no warranty.
##############################################################################

####### Macros #######
SHELL := /bin/bash

# Target and sources
BIN    := bin
WIN    := win

# Windows/Unix targets compiled on Windows/Unix
WARGET := szg.exe
ifdef SYSTEMROOT
TARGET := szg.exe
else
TARGET := szg
endif

# Defines
ifneq "$(VARS)" "no"
CDEFS  := -DVARS
endif

ifdef VARS_HASH
CDEFS  += -DVARS_HASH=$(VARS_HASH)
endif

ifdef VARS_HASHSIZE
CDEFS  += -DVARS_HASHSIZE=$(VARS_HASHSIZE)
endif

# Unix targets compiled on both Windows/Unix
CC     := gcc
CFLAGS := -lm -Wno-format -Wmissing-prototypes $(CDEFS)
WFLAGS := -lm -Wno-format -Wmissing-prototypes $(CDEFS)

# Windows targets compiled on Windows/Unix
ifdef SYSTEMROOT
WC     := i686-pc-mingw32-gcc
WLIBS  :=
else
WC     := i586-mingw32msvc-gcc
WLIBS  := -L/usr/i586-mingw32msvc/lib -I/usr/i586-mingw32msvc/include
endif

# Sources
CSRC   := szg.c grammar.c output.c tNum.c patterns.c arg1.c
HSRC   := szg.h output.h tNum.h arg1.h version.h usage.h

ifneq "$(VARS)" "no"
CSRC   += vars.c
HSRC   += vars.h
endif

TMPF   := patterns.c grammar.c grammar.h tNumTest usage.h version.h *~ *.tar.gz

MANPAGE:= szg.1
MAN    := /usr/share/man/man1


####### Rules ########
.PHONY: target tnum install uninstall clean commit tarball release

target: $(BIN)/$(TARGET) $(WIN)/$(WARGET)

# Compile target
$(BIN)/$(TARGET) $(WIN)/$(WARGET): $(CSRC) $(HSRC) makefile usage.txt version.txt
	@if [ ! -d $(BIN) ]; then mkdir $(BIN); fi
	$(CC) $(CFLAGS) -o $(BIN)/$(TARGET) $(CSRC)
	@if [ ! -d $(WIN) ]; then mkdir $(WIN); fi
	$(WC) $(WFLAGS) -o $(WIN)/$(WARGET) $(WLIBS) $(CSRC);

# tNum test-suite
tNum:
	@$(CC) $(CFLAGS) -o tNum tNum.c tNumTest.c

# vars test-suite
vars:
	@$(CC) $(CFLAGS) -o vars vars.c tNum.c varsTest.c

# c/h from y (yacc)
%.c: %.y
	yacc -d -o $@ $<

# c from l (lex)
%.c: %.l
	lex -o $@ $<

# Create headers with C string initializers from text files
%.h: %.txt
	@sed -e 's/[ \t]*$$//g' -e 's/^/"/g' -e 's/$$/\\n",/g' <$< >$@

# Install to c/cygdrive/WINDOWS /usr/bin
install:
	@cp -f $(BIN)/$(TARGET) /usr/bin;
	@cp -f $(MANPAGE) $(MAN);
	@if [ -n "$(SYSTEMROOT)" ];                        \
	  then cp -f $(WIN)/$(TARGET) /cygdrive/c/WINDOWS; \
	fi

# Uninstall from c/cygdrive/WINDOWS /usr/bin
uninstall:
	@rm -f /usr/bin/$(TARGET);
	@rm -f $(MAN)/$(MANPAGE);
	@if [ -n "$(SYSTEMROOT)" ];                        \
	  then rm -f /cygdrive/c/WINDOWS/$(TARGET);        \
	fi

# Remove all generated files
clean:
	@rm -fr $(BIN) $(WIN) $(TMPF)

# Commit to git repository
commit: clean
	@if [ "`git diff --no-ext-diff HEAD`" ]; \
	  then git commit -a;     \
	fi

# Create tarball for distribution
tarball:
	mkdir .szg && mkdir .szg/szg$(TAG)
	cp -rt .szg/szg$(TAG) *
	$(MAKE) target
	cp -rt .szg/szg$(TAG) bin win
	cd .szg && tar -czf ../szg$(TAG).tar.gz szg$(TAG)
	rm -rf .szg

# Tag HEAD and Create compressed tarball
release: commit
	@echo 'Chose old tag to follow: '; \
	select OLD in `git tag`; do break; done; \
	export TAG; \
	read -p 'Please Enter new tag name: ' TAG; \
	sed -r -e "s/^szg.*$$/szg $$TAG/" \
	       -e 's/([0-9]{4}-)[0-9]*/\1'`date +%Y`/ \
	       -i version.txt || exit 1; \
	git commit -a -m "version $$TAG"; \
	echo Adding git tag $$TAG; \
	echo "szg ($$TAG)" > changelog; \
	if [ -n "$$OLD" ]; then \
	  git log --pretty=format:"  * %h %an %s" $$OLD.. >> changelog; \
	else \
	  echo '  * Initial release' >> changelog; \
	fi; \
	echo " -- `git config user.name` <`git config user.email`>  `date -R`" >> changelog; \
	$$EDITOR changelog; \
	git tag -a -F changelog $$TAG HEAD; \
	$(MAKE) tarball TAG=-$$TAG

publish:
	@REMOTES=`git remote -v | sed -rn 's/^(.+)\t[^ ]+ \(push\)$$/\1/p'`; \
	ORIGIN=`git remote -v \
	| sed -rn 's,^origin\t(ssh://)?(.+)szg\.git \(push\)$$,\2,p'\
	| sed 's,/~,:,'`; \
	for REMOTE in $$REMOTES; do git push --mirror $$REMOTE; done; \
	$(MAKE) tarball; \
	echo scp to origin: $$ORIGIN; \
	scp *.tar.gz $$ORIGIN

deb:
	@-rm *.deb
	mkdir -p debian/DEBIAN
	@echo 'Package: szg'                                               > debian/DEBIAN/control
	@sed -nr 's/^szg (.+)$$/Version: \1-1/p' version.txt              >> debian/DEBIAN/control
	@echo 'Section: math'                                             >> debian/DEBIAN/control
	@echo 'Priority: optional'                                        >> debian/DEBIAN/control
	@echo 'Architecture: i386'                                        >> debian/DEBIAN/control
	@echo 'Depends: libc6'                                            >> debian/DEBIAN/control
	@echo 'Maintainer: SZABO Gergely <szg@subogero.com>'              >> debian/DEBIAN/control
	@echo 'Description: Fast command line calculator'                 >> debian/DEBIAN/control
	@echo ' Command line calculator with a very fast workflow,'       >> debian/DEBIAN/control
	@echo ' unsigned/signed int float modes, dec hex oct bin formats' >> debian/DEBIAN/control
	@echo ' user defined variables, comments and unlimited undo.'     >> debian/DEBIAN/control
	mkdir -p debian/usr/bin
	@cp bin/szg debian/usr/bin
	mkdir -p debian/usr/share/man/man1
	@cp szg.1 debian/usr/share/man/man1
	@gzip --best debian/usr/share/man/man1/szg.1
	mkdir -p debian/usr/share/doc/szg
	@grep Copyright version.txt                    > debian/usr/share/doc/szg/copyright
	@echo 'License: GPL-3'                        >> debian/usr/share/doc/szg/copyright
	@echo ' See /usr/share/common-licenses/GPL-3' >> debian/usr/share/doc/szg/copyright
	@git tag \
	| sort -rh \
	| xargs git show \
	| sed -n '/^szg/,/^ --/p' \
	| sed -r 's/^szg \((.+)\)$$/szg (\1-1)/' \
	> debian/usr/share/doc/szg/changelog.Debian
	gzip --best debian/usr/share/doc/szg/changelog.Debian
	dpkg-deb --build debian .
	@rm -rf debian
	lintian *.deb
