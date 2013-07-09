LOG=evince
TEX=tl-pres-m2

FIGSRC = $(wildcard fig/2-mot/circle/*.ipe  fig/2-mot/as/*.ipe fig/2-mot/connexe/*.ipe fig/2-mot/tri/*.ipe fig/3-exi/suivi/*.ipe fig/3-exi/har/*.ipe fig/4-con/tri/*.ipe fig/4-con/nas/*.ipe fig/4-con/pas/*.ipe)
FIGOBJ = $(FIGSRC:.ipe=.pdf)
 
all: fig gnup paper show 

fig: $(FIGOBJ)

fig/2-mot/circle/%.pdf: fig/2-mot/circle/%.ipe
	ipetoipe -pdf $<
fig/2-mot/as/%.pdf: fig/2-mot/as/%.ipe
	ipetoipe -pdf $<
fig/2-mot/connexe/%.pdf: fig/2-mot/connexe/%.ipe
	ipetoipe -pdf $<
fig/2-mot/tri/%.pdf: fig/2-mot/tri/%.ipe
	ipetoipe -pdf $<
fig/3-exi/suivi/%.pdf: fig/3-exi/suivi/%.ipe
	ipetoipe -pdf $<
fig/3-exi/har/%.pdf: fig/3-exi/har/%.ipe
	ipetoipe -pdf $<
fig/4-con/tri/%.pdf: fig/4-con/tri/%.ipe
	ipetoipe -pdf $<
fig/4-con/nas/%.pdf: fig/4-con/tri/%.ipe
	ipetoipe -pdf $<
fig/4-con/pas/%.pdf: fig/4-con/tri/%.ipe
	ipetoipe -pdf $<	
	
gnup:
	gnuplot ../../tools/ch-time.gp
	gnuplot ../../tools/ch.gp
	gnuplot ../../tools/nas.gp
	
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
