##############################################################################
# (c) SZABO Gergely, 2009
# Free software, distributed under the GNU GPL v3 license
# There is absolutely no warranty.
##############################################################################
define DESCR
Description: Fast command line calculator
 Command line calculator with a very fast workflow,
 unsigned/signed int float modes, dec hex oct bin formats
 user defined variables, comments and unlimited undo.
endef
export DESCR

####### Macros #######
SHELL := /bin/bash

# Target and sources
BIN    := .bin
WIN    := .win
REL    := .release

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
CFLAGS := -g -Wno-format -Wmissing-prototypes $(CDEFS)
LIBS   := -lm

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

TMPF   := patterns.c patterns.h grammar.c grammar.h tNumTest usage.h version.h *~ *.tar.gz

MANPAGE:= szg.1
MANDIR := $(DESTDIR)/usr/share/man/man1
BINDIR := $(DESTDIR)/usr/bin

####### Rules ########
.PHONY: all tnum install uninstall clean commit tarball tag release deb

all: $(BIN)/$(TARGET)
win: $(WIN)/$(WARGET)

# Compile targets
$(BIN)/$(TARGET): $(CSRC) $(HSRC) makefile usage.txt version.txt
	@if [ ! -d $(BIN) ]; then mkdir $(BIN); fi
	$(CC) $(CFLAGS) -o $(BIN)/$(TARGET) $(CSRC) $(LIBS)
$(WIN)/$(WARGET): $(CSRC) $(HSRC) makefile usage.txt version.txt
	@if [ ! -d $(WIN) ]; then mkdir $(WIN); fi
	$(WC) $(WFLAGS) -o $(WIN)/$(WARGET) $(WLIBS) $(CSRC) $(LIBS);

# tNum test-suite
tNum:
	@$(CC) $(CFLAGS) -o tNum tNum.c tNumTest.c

# vars test-suite
vars:
	@$(CC) $(CFLAGS) -o vars vars.c tNum.c varsTest.c

# c/h from y (yacc)
%.c: %.y
	bison -d -o $@ $<

# c from l (lex)
%.c: %.l
	flex --header-file=patterns.h -o $@ $<

# Create headers with C string initializers from text files
%.h: %.txt
	@sed -e 's/[ \t]*$$//g' -e 's/^/"/g' -e 's/$$/\\n",/g' <$< >$@

# Install to c/cygdrive/WINDOWS /usr/bin
install:
	@mkdir -p $(BINDIR)
	@cp -f $(BIN)/$(TARGET) $(BINDIR)
	@mkdir -p $(MANDIR)
	@cp -f $(MANPAGE) $(MANDIR)
	@gzip --best $(MANDIR)/$(MANPAGE)
	@if [ -n "$(SYSTEMROOT)" ];                        \
	  then cp -f $(WIN)/$(TARGET) /cygdrive/c/WINDOWS; \
	fi

# Uninstall from c/cygdrive/WINDOWS /usr/bin
uninstall:
	@rm -f $(BINDIR)/$(TARGET)
	@rm -f $(MANDIR)/$(MANPAGE).gz
	@if [ -n "$(SYSTEMROOT)" ];                 \
	  then rm -f /cygdrive/c/WINDOWS/$(TARGET); \
	fi

# Remove all generated files
clean:
	@rm -fr $(BIN) $(WIN) $(TMPF)

# Commit to git repository
commit:
	@if [ "`git diff --no-ext-diff HEAD`" ]; \
	  then git commit -a;     \
	fi

# Create tarballs for distribution
# Tag HEAD and Create compressed tarball
tag: commit
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

publish:
	@REMOTES=`git remote -v | sed -rn 's/^(.+)\t[^ ]+ \(push\)$$/\1/p'`; \
	ORIGIN=`git remote -v \
	| sed -rn 's,^origin\t(ssh://)?(.+)szg\.git \(push\)$$,\2,p'\
	| sed 's,/~,:,'`; \
	for REMOTE in $$REMOTES; do git push --all --tags $$REMOTE; done

tarball: clean
	export TAG=`sed -rn 's/^szg (.+)$$/\1/p' version.txt`; \
	$(MAKE) balls
balls:
	mkdir -p $(REL)/szg-$(TAG); \
	cp -rt $(REL)/szg-$(TAG) *; \
	cd $(REL); \
	tar -czf szg_$(TAG).tar.gz szg-$(TAG); \
	[ -f *zip ] && rm *zip; \
	zip -r   szg_$(TAG).zip    szg-$(TAG); \
	cd ..; \
	$(MAKE) $(WIN)/$(WARGET); \
	zip -j $(REL)/szg_$(TAG).zip $(WIN)/$(WARGET)

deb: tarball $(BIN)/$(TARGET)
	export TAG=`sed -rn 's/^szg (.+)$$/\1/p' version.txt`; \
	export DEB=$(REL)/szg-$${TAG}/debian; \
	$(MAKE) debs
debs:
	-rm $(REL)/*.deb
	cp -f $(REL)/szg_$(TAG).tar.gz $(REL)/szg_$(TAG).orig.tar.gz 
	mkdir -p $(DEB)
	echo 'Source: szg'                                            > $(DEB)/control
	echo 'Section: math'                                         >> $(DEB)/control
	echo 'Priority: optional'                                    >> $(DEB)/control
	sed -nr 's/^C.+ [-0-9]+ (.+)$$/Maintainer: \1/p' version.txt >> $(DEB)/control
	echo 'Build-Depends: debhelper, flex, bison'                 >> $(DEB)/control
	echo 'Standards-version: 3.8.4'                              >> $(DEB)/control
	echo                                                         >> $(DEB)/control
	echo 'Package: szg'                                          >> $(DEB)/control
	echo 'Architecture: any'                                     >> $(DEB)/control
	echo 'Depends: $${shlibs:Depends}, $${misc:Depends}'         >> $(DEB)/control
	echo "$$DESCR"                                               >> $(DEB)/control
	grep Copyright version.txt                    > $(DEB)/copyright
	echo 'License: GPL-3'                        >> $(DEB)/copyright
	echo ' See /usr/share/common-licenses/GPL-3' >> $(DEB)/copyright
	echo 7 > $(DEB)/compat
	git tag \
	| sort -rh \
	| xargs git show \
	| sed -n '/^szg/,/^ --/p' \
	| sed -r 's/^szg \((.+)\)$$/szg (\1-1) UNRELEASED; urgency=low/' \
	> $(DEB)/changelog
	echo '#!/usr/bin/make -f' > $(DEB)/rules
	echo '%:'                >> $(DEB)/rules
	echo '	dh $$@'          >> $(DEB)/rules
	chmod 755 $(DEB)/rules
	mkdir -p $(DEB)/source
	echo '3.0 (quilt)' > $(DEB)/source/format
	cd $(REL)/szg-$(TAG); dpkg-buildpackage	-us -uc
	lintian $(REL)/*.deb
