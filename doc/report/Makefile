LOG=evince
TEX=tl-rapport

 
all: paper show 

	
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
