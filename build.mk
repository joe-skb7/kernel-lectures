TEX		:= $(wildcard *.tex)
NAME		:= $(basename $(TEX))
PDF		:= $(NAME).pdf
LATEX		:= rubber
LATEX_FLAGS	:= -m xelatex -W all
IMAGES_DIA	:= $(wildcard images/*.dia)
IMAGES_DIAPDF	:= $(patsubst %.dia,%.pdf,$(IMAGES_DIA))
IMAGES_DIAEPS	:= $(patsubst %.dia,%.eps,$(IMAGES_DIA))

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
  Q = @
else
  Q =
endif

all: $(PDF)

$(PDF): $(TEX) $(IMAGES_DIAPDF)
	@printf "  LATEX  $@\n"
	$(Q)$(LATEX) $(LATEX_FLAGS) $(TEX)

clean:
	@printf "  CLEAN\n"
	$(Q)$(LATEX) --clean $(NAME) 2>/dev/null
	$(Q)-rm -f *.vrb
	$(Q)-rm -f $(IMAGES_DIAPDF) $(IMAGES_DIAEPS)

$(IMAGES_DIAPDF):
	@printf "  GEN    $@\n"
	$(Q)d=images/$$(basename $@ .pdf).dia;	\
	e=images/$$(basename $@ .pdf).eps;	\
	dia -t eps -e $$e $$d 2>/dev/null;	\
	epstopdf $$e

distclean: clean
	@printf "  DISTCLEAN\n"
	$(Q)-rm -f $(PDF)

.PHONY: all clean distclean
