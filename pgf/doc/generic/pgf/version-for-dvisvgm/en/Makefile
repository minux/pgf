rerun = "(There were undefined references|Rerun to get (cross-references|the bars) right)"
doc   = pgfmanual
latex = lualatex --output-format=dvi
TEXINPUTS := ../../text-en:$(TEXINPUTS)

all: $(doc).svg

test: pgfmanual-test.svg

pgfmanual.idx: 
	touch pgfmanual.idx

pgfmanual.ind: pgfmanual.idx 
	makeindex $(doc).idx

pgfmanual.svg: $(doc).tex $(doc).ind 
	TEXINPUTS="$(TEXINPUTS)" $(latex) $(doc).tex
	(egrep -q $(rerun) $(doc).log && TEXINPUTS="$(TEXINPUTS)" $(latex) $(doc).tex) || true
	dvisvgm --no-merge --page=1- --bbox=a4 --linkmark=none $(doc)

pgfmanual-test.svg: $(doc)-test.tex $(doc).ind 
	TEXINPUTS="$(TEXINPUTS)" $(latex) $(doc)-test.tex
	(egrep -q $(rerun) $(doc).log && TEXINPUTS="$(TEXINPUTS)" $(latex) $(doc)-test.tex) || true
	dvisvgm --no-merge --page=1- --bbox=a4 --linkmark=none $(doc)-test

clean:
	rm -f  \
		plots/* \
		pgfmanual.out \
		pgfmanual.log \
		pgfmanual.idx \
		pgfmanual.ind \
		pgfmanual.ilg \
		pgfmanual.pdf \
		pgfmanual.toc \
		pgfmanual.bbl \
		pgfmanual.blg \
		pgfmanual.ps \
		pgfmanual.dvi \
		pgfmanual*.aux 