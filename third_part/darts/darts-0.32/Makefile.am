SUBDIRS = tests
EXTRA_DIRS = doc
pkglibexecdir = ${libexecdir}/darts
pkglibexec_PROGRAMS = mkdarts darts
always_built_SUBDIRS = tests
EXTRA_DIST = darts.spec BSD LGPL
mkdarts_SOURCES = mkdarts.cpp darts.h
darts_SOURCES   = darts.cpp   darts.h
include_HEADERS = darts.h

dist-hook:
	for subdir in $(EXTRA_DIRS); do \
	cp -rp $$subdir $(distdir)/$$subdir; \
	rm -f $(distdir)/$$subdir/*~; \
	rm -f $(distdir)/$$subdir/*.{bak,orig}; \
	rm -rf $(distdir)/$$subdir/CVS; \
	rm -rf $(distdir)/$$subdir/*/CVS; \
        rm -rf $(distdir)/$$subdir/.svn; \
        rm -rf $(distdir)/.svn; \
        rm -rf $(distdir)/*/.svn; \
        rm -rf $(distdir)/*/*/.svn; \
	done

rpm: dist
	rpmbuild -ta @PACKAGE@-@VERSION@.tar.gz
