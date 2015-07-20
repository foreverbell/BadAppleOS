MD_DOCS := $(patsubst %.md, %.pdf, $(wildcard *.md))

all: $(MD_DOCS)

$(MD_DOCS): %.pdf : %.md
	pandoc -o $@ --latex-engine=xelatex $<

clean: 
	rm *.pdf
	
.PHONY: all clean