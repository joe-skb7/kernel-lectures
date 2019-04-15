DIRS		= $(sort $(dir $(wildcard lecture*/)))
DIRS_BUILD	= $(DIRS:%=build-%)
DIRS_CLEAN	= $(DIRS:%=clean-%)
DIRS_DISTCLEAN	= $(DIRS:%=distclean-%)

all: $(DIRS_BUILD)
$(DIRS): $(DIRS_BUILD)
$(DIRS_BUILD):
	$(MAKE) -C $(@:build-%=%)

clean: $(DIRS_CLEAN)
$(DIRS_CLEAN):
	$(MAKE) -C $(@:clean-%=%) clean

distclean: $(DIRS_DISTCLEAN)
$(DIRS_DISTCLEAN):
	$(MAKE) -C $(@:distclean-%=%) distclean

.PHONY: all clean distclean
.PHONY: subdirs $(DIRS)
.PHONY: subdirs $(DIRS_BUILD)
.PHONY: subdirs $(DIRS_CLEAN)
.PHONY: subdirs $(DIRS_DISTCLEAN)
