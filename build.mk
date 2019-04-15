TEX		:= $(wildcard *.tex)
NAME		:= $(basename $(TEX))
PDF		:= $(NAME).pdf
LATEX		:= rubber
LATEX_FLAGS	:= -m xelatex -W all
IMAGES_DIA	:= $(wildcard images/*.dia)
IMAGES_DIAPDF	:= $(patsubst %.dia,%.pdf,$(IMAGES_DIA))
IMAGES_DIAEPS	:= $(patsubst %.dia,%.eps,$(IMAGES_DIA))

all: $(PDF)

$(PDF): $(TEX) $(IMAGES_DIAPDF)
	@printf "  LATEX  $@\n"
	@$(LATEX) $(LATEX_FLAGS) $(TEX)

clean:
	@printf "  CLEAN\n"
	@$(LATEX) --clean $(NAME)
	@-rm -f *.vrb
	@-rm -f $(IMAGES_DIAPDF) $(IMAGES_DIAEPS)

$(IMAGES_DIAPDF):
	@printf "  GEN    $@\n"
	@d=images/$$(basename $@ .pdf).dia;	\
	e=images/$$(basename $@ .pdf).eps;	\
	dia -t eps -e $$e $$d 2>/dev/null;	\
	epstopdf $$e

distclean: clean
	@printf "  DISTCLEAN\n"
	@-rm -f $(PDF)

.PHONY: all clean distclean
