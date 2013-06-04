LOG=evince
TEX=pres



all: slides show

slides:
	pdflatex $(TEX).tex

show:
	$(LOG) $(TEX).pdf

proper:
	rm -f *.log *.toc *.aux *.nav *.snm *.out 
	rm -f *.backup *~

clean:	proper
	rm -f $(TEX).pdf
