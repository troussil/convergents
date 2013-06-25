LOG=evince
TEX=tl-rapport

FIGSRC = $(wildcard fig/2-mot/circle/*.ipe  fig/2-mot/as/*.ipe fig/4-exi/suivi/*.ipe fig/4-exi/har/*.ipe)
FIGOBJ = $(FIGSRC:.ipe=.pdf)
 
all: fig paper show  
 
fig: $(FIGOBJ)

fig/2-mot/circle/%.pdf: fig/2-mot/circle/%.ipe
	ipetoipe -pdf $<
fig/2-mot/as/%.pdf: fig/2-mot/as/%.ipe
	ipetoipe -pdf $<
fig/4-exi/suivi/%.pdf: fig/4-exi/suivi/%.ipe
	ipetoipe -pdf $<
fig/4-exi/har/%.pdf: fig/4-exi/har/%.ipe
	ipetoipe -pdf $<
	
paper:
	pdflatex $(TEX).tex
	bibtex $(TEX)
	pdflatex $(TEX).tex
	pdflatex $(TEX).tex

show:
	$(LOG) $(TEX).pdf

proper:
	rm -f *.log *.toc *.aux *.nav *.snm *.out *.bbl *.blg *.dvi
	rm -f *.backup *~
	

clean:	proper
	rm -f $(TEX).ps $(TEX).pdf
