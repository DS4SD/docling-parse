# Basic Makefile for (un)installing the files, and creating archives manually.
#
# Supports standard GNU Makefile variables for specifying the paths:
# * prefix
# * datarootdir
# * datadir
# * DESTDIR
#

SHELL        = /bin/bash

AUTHOR       = adobe
PACKAGE      = cmap-resources
NAME         = $(AUTHOR)-$(PACKAGE)

# So far there's no versioning in place. If there will be any, then we will
# switch to commented line below...
VERSION      =
ARCHIVE_NAME = $(NAME)
#ARCHIVE_NAME = $(NAME)-$(VERSION)

GIT_ARCHIVE_TYPE = tar.gz

prefix       = /usr/local
datarootdir  = $(prefix)/share
datadir      = $(datarootdir)
packagedir   = $(datadir)/$(AUTHOR)/resources/mapping

# Source & destination folders:
CMap_CNS1_source = Adobe-CNS1-7
CMap_CNS1_dest = CNS1

CMap_GB1_source = Adobe-GB1-5
CMap_GB1_dest = GB1

CMap_ID_source = Adobe-Identity-0
CMap_ID_dest = Identity

CMap_Japan1_source = Adobe-Japan1-7
CMap_Japan1_dest = Japan1

CMap_Japan2_source = deprecated/Adobe-Japan2-0
CMap_Japan2_dest = deprecated/Japan2

CMap_KR_source = Adobe-KR-9
CMap_KR_dest = KR

CMap_Korea1_source = Adobe-Korea1-2
CMap_Korea1_dest = Korea1

# Other related files:
DISTFILES    =                          \
	LICENSE.txt                     \
	Makefile                        \
	README.md                       \
	VERSIONS.txt                    \

.PHONY: install uninstall

# Create the necessary directories first, and then install all the files:
install:
	install -m 0755 -d $(DESTDIR)$(packagedir)
	install -m 0755 -d $(DESTDIR)$(packagedir)/$(CMap_CNS1_dest)/CMap
	install -m 0755 -d $(DESTDIR)$(packagedir)/$(CMap_GB1_dest)/CMap
	install -m 0755 -d $(DESTDIR)$(packagedir)/$(CMap_ID_dest)/CMap
	install -m 0755 -d $(DESTDIR)$(packagedir)/$(CMap_Japan1_dest)/CMap
	install -m 0755 -d $(DESTDIR)$(packagedir)/$(CMap_Japan2_dest)/CMap
	install -m 0755 -d $(DESTDIR)$(packagedir)/$(CMap_KR_dest)/CMap
	install -m 0755 -d $(DESTDIR)$(packagedir)/$(CMap_Korea1_dest)/CMap

	install -m 0644 -p   $(CMap_CNS1_source)/cid2code.txt $(DESTDIR)$(packagedir)/$(CMap_CNS1_dest)
	install -m 0644 -p    $(CMap_GB1_source)/cid2code.txt $(DESTDIR)$(packagedir)/$(CMap_GB1_dest)
	install -m 0644 -p $(CMap_Japan1_source)/cid2code.txt $(DESTDIR)$(packagedir)/$(CMap_Japan1_dest)
	install -m 0644 -p $(CMap_Japan2_source)/cid2code.txt $(DESTDIR)$(packagedir)/$(CMap_Japan2_dest)
	install -m 0644 -p $(CMap_KR_source)/cid2code.txt $(DESTDIR)$(packagedir)/$(CMap_KR_dest)
	install -m 0644 -p $(CMap_Korea1_source)/cid2code.txt $(DESTDIR)$(packagedir)/$(CMap_Korea1_dest)

	install -m 0644 -p $(CMap_CNS1_source)/CMap/* $(DESTDIR)$(packagedir)/$(CMap_CNS1_dest)/CMap
	install -m 0644 -p $(CMap_GB1_source)/CMap/* $(DESTDIR)$(packagedir)/$(CMap_GB1_dest)/CMap
	install -m 0644 -p $(CMap_ID_source)/CMap/* $(DESTDIR)$(packagedir)/$(CMap_ID_dest)/CMap
	install -m 0644 -p $(CMap_Japan1_source)/CMap/* $(DESTDIR)$(packagedir)/$(CMap_Japan1_dest)/CMap
	install -m 0644 -p $(CMap_Japan2_source)/CMap/* $(DESTDIR)$(packagedir)/$(CMap_Japan2_dest)/CMap
	install -m 0644 -p $(CMap_KR_source)/CMap/* $(DESTDIR)$(packagedir)/$(CMap_KR_dest)/CMap
	install -m 0644 -p $(CMap_Korea1_source)/CMap/* $(DESTDIR)$(packagedir)/$(CMap_Korea1_dest)/CMap

# Other files might be written in $(packagedir), remove only what has been installed:
uninstall:
	rm -rf $(DESTDIR)$(packagedir)/$(CMap_CNS1_dest)
	rm -rf $(DESTDIR)$(packagedir)/$(CMap_GB1_dest)
	rm -rf $(DESTDIR)$(packagedir)/$(CMap_ID_dest)
	rm -rf $(DESTDIR)$(packagedir)/$(CMap_Japan1_dest)
	rm -rf $(DESTDIR)$(packagedir)/$(CMap_Japan2_dest)
	rm -rf $(DESTDIR)$(packagedir)/$(CMap_KR_dest)
	rm -rf $(DESTDIR)$(packagedir)/$(CMap_Korea1_dest)

# We are putting the source files into subfolder, same as Github does for its releases:
git-archive:
	git archive -v --format=$(GIT_ARCHIVE_TYPE) --prefix=$(ARCHIVE_NAME)/ HEAD -o $(ARCHIVE_NAME).$(GIT_ARCHIVE_TYPE)

# The --transform option is what will put the source files into a $(ARCHIVE_NAME) subdirectory:
tar.bz2:
	tar --transform="s|^|$(ARCHIVE_NAME)/|" -cjvf $(ARCHIVE_NAME).tar.bz2 $(CMap_CNS1_source) $(CMap_GB1_source) $(CMap_ID_source) $(CMap_Japan1_source) $(CMap_Japan2_source) $(CMap_KR_source) $(CMap_Korea1_source) $(DISTFILES)

tar.gz:
	tar --transform="s|^|$(ARCHIVE_NAME)/|" -czvf $(ARCHIVE_NAME).tar.gz  $(CMap_CNS1_source) $(CMap_GB1_source) $(CMap_ID_source) $(CMap_Japan1_source) $(CMap_Japan2_source) $(CMap_KR_source) $(CMap_Korea1_source) $(DISTFILES)

tar.xz:
	tar --transform="s|^|$(ARCHIVE_NAME)/|" -cJvf $(ARCHIVE_NAME).tar.xz  $(CMap_CNS1_source) $(CMap_GB1_source) $(CMap_ID_source) $(CMap_Japan1_source) $(CMap_Japan2_source) $(CMap_KR_source) $(CMap_Korea1_source) $(DISTFILES)
