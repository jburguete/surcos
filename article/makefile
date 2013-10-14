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

$(image1).eps: $(imdir)$(image1).png makefile
	convert $(imdir)$(image1).png $(image1).eps

$(image2).eps: $(imdir)$(image2).png makefile
	convert $(imdir)$(image2).png $(image2).eps

$(image3).eps: $(imdir)$(image3).png makefile
	convert $(imdir)$(image3).png $(image3).eps

$(image4).eps: $(imdir)$(image4).png makefile
	convert $(imdir)$(image4).png $(image4).eps

$(image5).eps: $(imdir)$(image5).png makefile
	convert $(imdir)$(image5).png $(image5).eps

$(image6).eps: $(imdir)$(image6).png makefile
	convert $(imdir)$(image6).png $(image6).eps

$(image7).eps: $(imdir)$(image7).png makefile
	convert $(imdir)$(image7).png $(image7).eps

$(image8).eps: $(imdir)$(image8).png makefile
	convert $(imdir)$(image8).png $(image8).eps

$(image9).eps: $(imdir)$(image9).png makefile
	convert $(imdir)$(image9).png $(image9).eps

$(image10).eps: $(imdir)$(image10).png makefile
	convert $(imdir)$(image10).png $(image10).eps

$(image11).eps: $(imdir)$(image11).png makefile
	convert $(imdir)$(image11).png $(image11).eps

$(image12).eps: $(imdir)$(image12).png makefile
	convert $(imdir)$(image12).png $(image12).eps
