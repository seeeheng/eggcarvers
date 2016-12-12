'''
Poeegg is the main script for the Inkscape extension that works in conjunction with poeegg.inx
'''
import sys, os
import inkex
from math import *
import getopt
from poe_parse import Parser
# import the other files that you used to write the code
# unicorn.context gcodecontext unicorn.svg parser svg parser

# The class PoeEgg(inkex.Effect) will be called by the Inkscape program, and can store any
# information input by the users.
class PoeEgg(inkex.Effect):
    def __init__(self):
        inkex.Effect.__init__(self)
        self.OptionParser.add_option("--tab",
                          action="store", type="string",
                          dest="tab")

    def output(self):
        #Calls the generate function within the Parser class in poe_parse.py that generates the txt file
        self.parser.generate()
        self.parser_serial_out()

    def effect(self):
        #Effect is called by inkscape when the extension is run
        self.parser = Parser(self.document.getroot())
        self.parser.parse()

if __name__ == '__main__':   #pragma: no cover
  e = PoeEgg()
  e.affect()
