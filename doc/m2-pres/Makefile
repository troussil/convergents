LOG=evince
TEX=tl-pres-m2

#FIGSRC = $(wildcard )
#FIGOBJ = $(FIGSRC:.ipe=.pdf)
 
all: fig paper show 

fig: $(FIGOBJ)

#fig/2-motivation/geometrie/%.pdf: fig/2-motivation/geometrie/%.ipe
#	ipetoipe -pdf $<

	
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
