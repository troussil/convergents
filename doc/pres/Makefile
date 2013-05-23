LOG=evince
TEX=tl-pres-m2disco

#FIGSRC = $(wildcard fig/*.ipe)
#FIGOBJ = $(FIGSRC:.ipe=.pdf)
 
all: paper show 

#fig: $(FIGOBJ)

#fig/%.pdf: fig/%.ipe
#	ipetoipe -pdf $<

paper:  
	pdflatex $(TEX).tex
#	bibtex $(TEX)
#	pdflatex $(TEX).tex
	pdflatex $(TEX).tex

show:
	$(LOG) $(TEX).pdf

proper:
	rm -f *.log *.toc *.aux *.nav *.snm *.out *.bbl *.blg *.dvi
	rm -f *.backup *~
#	rm -f fig/*.pdf

clean:	proper
	rm -f $(TEX).ps $(TEX).pdf
