all: bom

bom:
	xsltproc -o "kicad/espdsc-bom.csv" \
	    "/Applications/Kicad/kicad.app/Contents/SharedSupport/plugins/bom_with_title_block_2_csv.xsl" \
	    "kicad/espdsc.xml"
