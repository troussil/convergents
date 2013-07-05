LOG=evince
TEX=tl-pres-m2

FIGSRC = $(wildcard fig/2-mot/circle/*.ipe  fig/2-mot/as/*.ipe fig/2-mot/connexe/*.ipe fig/2-mot/tri/*.ipe)
FIGOBJ = $(FIGSRC:.ipe=.pdf)
 
all: fig paper show 

fig: $(FIGOBJ)

fig/2-mot/circle/%.pdf: fig/2-mot/circle/%.ipe
	ipetoipe -pdf $<
fig/2-mot/as/%.pdf: fig/2-mot/as/%.ipe
	ipetoipe -pdf $<
fig/2-mot/connexe/%.pdf: fig/2-mot/connexe/%.ipe
	ipetoipe -pdf $<
fig/2-mot/tri/%.pdf: fig/2-mot/tri/%.ipe
	ipetoipe -pdf $<

	
paper:
	pdflatex $(TEX).tex
	#bibtex $(TEX)
	#pdflatex $(TEX).tex
	pdflatex $(TEX).tex

show:
	$(LOG) $(TEX).pdf

proper:
	rm -f *.log *.toc *.aux *.nav *.snm *.out *.bbl *.blg *.dvi
	rm -f *.backup *~

clean:	proper
	rm -f $(TEX).ps $(TEX).pdf
