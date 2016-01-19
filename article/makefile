figures = water1-9.eps fertilizer123.eps fertilizer456.eps fertilizer789.eps

imdir = ../5.2/manual/images/
image1 = mprincipalEN
image2 = confGeomEN
image3 = confSurcoEN
image4 = confInputEN
image5 = confSondasEN
image6 = confParamEN
image7 = menuRepresEN
image8 = evo1EN
image9 = evoSurcoEN
image10 = evoSondaEN
image11 = sumarioEN
image12 = sumario2EN
images = $(image1).eps $(image2).eps $(image3).eps $(image4).eps $(image5).eps \
	$(image6).eps $(image7).eps $(image8).eps $(image9).eps $(image10).eps \
	$(image11).eps $(image12).eps

surcos.pdf: surcos.tex makefile bib.bib $(figures) $(images)
	latex surcos
	latex surcos
	bibtex surcos
	latex surcos
	latex surcos
	dvipdf surcos
