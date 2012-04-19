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
CFLAGS := -Wno-format -Wmissing-prototypes $(CDEFS)
WFLAGS := -Wno-format -Wmissing-prototypes $(CDEFS)
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
CSRC   := szg.c grammar.c output.c tNum.c patterns.c arg1/arg1.c
HSRC   := szg.h output.h tNum.h arg1/arg1.h

ifneq "$(VARS)" "no"
CSRC   += vars.c
HSRC   += vars.h
endif

TMPF   := patterns.c grammar.c grammar.h tNumTest arg1/usage.h arg1/version.h *~ *.tar.gz

MANPAGE:= szg.1
MAN    := /usr/share/man/man1


####### Rules ########
.PHONY: target arg1 tnum install uninstall clean commit tarball release

target: arg1 $(BIN)/$(TARGET) $(WIN)/$(WARGET)

# Compile target
$(BIN)/$(TARGET) $(WIN)/$(WARGET): $(CSRC) $(HSRC) makefile usage.txt version.txt
	@if [ ! -d $(BIN) ]; then mkdir $(BIN); fi
	$(CC) $(CFLAGS) -o $(BIN)/$(TARGET) $(CSRC) $(LIBS)
	@if [ ! -d $(WIN) ]; then mkdir $(WIN); fi
	$(WC) $(WFLAGS) -o $(WIN)/$(WARGET) $(WLIBS) $(CSRC) $(LIBS);

# Recursive make for the arg1 subdir
arg1:
	$(MAKE) -C arg1

# tNum test-suite
tNum:
	@$(CC) $(CFLAGS) -o tNum tNum.c tNumTest.c

# vars test-suite
vars:
	@$(CC) $(CFLAGS) -o vars vars.c tNum.c varsTest.c

# c/h from y (yacc)
%.c: %.y
	bison -y -d -o $@ $<

# c from l (lex)
%.c: %.l
	flex -l -o $@ $<

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
	rm -rf .szg/szg$(TAG)/arg1/.git*
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
	sed -r -e "s/version\s..*$$/version $$TAG/" \
	       -e 's/^(\(c\).+, [0-9]{4}).*$$/\1-'"`date +%Y`/" \
	       -i version.txt || exit 1; \
	git commit -a -m "version $$TAG"; \
	echo Adding git tag $$TAG; \
	echo Changes since $$OLD > Release.txt; \
	echo >> Release.txt; \
	git log --pretty=format:"%h %an %s" $$OLD.. >> Release.txt; \
	$$EDITOR Release.txt; \
	git tag -a -F Release.txt $$TAG HEAD; \
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
