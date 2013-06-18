LOG=evince
TEX=tl-pres-m2disco

FIGSRC = $(wildcard fig/2-motivation/geometrie/*.ipe fig/2-motivation/circle/*.ipe fig/2-motivation/alpha-shape-circle/*.ipe fig/3-existant/circle/*.ipe fig/3-existant/connexe/*.ipe fig/3-existant/convexe/*.ipe fig/3-existant/har/*.ipe fig/3-existant/tri/*.ipe fig/4-contribution/negatif/*.ipe fig/4-contribution/positif/*.ipe)
FIGOBJ = $(FIGSRC:.ipe=.pdf)
 
all: fig paper show 

fig: $(FIGOBJ)

fig/2-motivation/geometrie/%.pdf: fig/2-motivation/geometrie/%.ipe
	ipetoipe -pdf $<
fig/2-motivation/circle/%.pdf: fig/2-motivation/circle/%.ipe
	ipetoipe -pdf $<	
fig/2-motivation/alpha-shape-circle/%.pdf: fig/2-motivation/alpha-shape-circle/%.ipe
	ipetoipe -pdf $<
fig/3-existant/circle/%.pdf: fig/3-existant/circle/%.ipe
	ipetoipe -pdf $<
fig/3-existant/connexe/%.pdf: fig/3-existant/connexe/%.ipe
	ipetoipe -pdf $<
fig/3-existant/convexe/%.pdf: fig/3-existant/convexe/%.ipe
	ipetoipe -pdf $<
fig/3-existant/har/%.pdf: fig/3-existant/har/%.ipe
	ipetoipe -pdf $<	
fig/3-existant/tri/%.pdf: fig/3-existant/tri/%.ipe
	ipetoipe -pdf $<	
fig/4-contribution/negatif/%.pdf: fig/4-contribution/negatif/%.ipe
	ipetoipe -pdf $<
fig/4-contribution/positif/%.pdf: fig/4-contribution/positif/%.ipe
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
	rm -f fig/2-motivation/geometrie/*.pdf
	rm -f fig/2-motivation/circle/*.pdf
	rm -f fig/2-motivation/alpha-shape-circle/*.pdf
	rm -f fig/3-existant/circle/*.pdf
	rm -f fig/3-existant/connexe/*.pdf
	rm -f fig/3-existant/convexe/*.pdf
	rm -f fig/3-existant/har/*.pdf
	rm -f fig/3-existant/tri/*.pdf
	rm -f fig/4-contribution/negatif/*.pdf
	rm -f fig/4-contribution/positif/*.pdf
	

clean:	proper
	rm -f $(TEX).ps $(TEX).pdf
