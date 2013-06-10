LOG=evince
TEX=pres

FIGSRC = $(wildcard fig/*.ipe)
FIGOBJ = $(FIGSRC:.ipe=.pdf)
 
all: figs slides show 

figs: $(FIGOBJ)

fig/%.pdf: fig/%.ipe
	ipetoipe -pdf $<

slides:
	pdflatex $(TEX).tex
	pdflatex $(TEX).tex

show:
	$(LOG) $(TEX).pdf

proper:
	rm -f *.log *.toc *.aux *.nav *.snm *.out 
	rm -f *.backup *~
	rm -f fig/*.pdf

clean:	proper
	rm -f $(TEX).pdf
