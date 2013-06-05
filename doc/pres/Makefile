LOG=evince
TEX=tl-pres-m2disco

FIGSRC = $(wildcard fig/as-algo/*.ipe fig/*.ipe fig/as/*.ipe fig/har/*.ipe fig/cercle/*.ipe fig/pas/*.ipe)
FIGOBJ = $(FIGSRC:.ipe=.pdf)
 
all: fig paper show 

fig: $(FIGOBJ)

fig/%.pdf: fig/%.ipe
	ipetoipe -pdf $<
fig/as/%.pdf: fig/as/%.ipe
	ipetoipe -pdf $<
fig/har/%.pdf: fig/har/%.ipe
	ipetoipe -pdf $<
fig/cercle/%.pdf: fig/cercle/%.ipe
	ipetoipe -pdf $<
fig/as-algo/%.pdf: fig/as-algo/%.ipe
	ipetoipe -pdf $<
fig/pas/%.pdf: fig/pas/%.ipe
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
	rm -f fig/as/*.pdf
	rm -f fig/har/*.pdf
	rm -f fig/cercle/*.pdf
	rm -f fig/as-algo/*.pdf
	

clean:	proper
	rm -f $(TEX).ps $(TEX).pdf
