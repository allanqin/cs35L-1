#!/usr/bin/python

#Ported to Python3 - works with Python2 also
#Note: don't mix tabs and spaces when playing with Python -_-"

import random, sys
from optparse import OptionParser

class randline:
    def __init__(self, args):
        self.lines = []
        for i in range(len(args)):
            f = open(args[i], 'r')
            #calling readlines multiple times causes a bug. Use a temp variable
            temp = f.readlines()
            if len(temp) is 0:
                self.lines += '\n'
            else:
                self.lines += temp
            f.close()
	    
    def chooseline(self, wr):
        temp = random.choice(self.lines)
        if wr:
            if temp in self.lines:
                self.lines.remove(temp)
        return temp
    
    def unique(self):
        self.lines = list(set(self.lines))
	
def main():
    version_msg = "%prog 2.1"
    usage_msg = """%prog [OPTION]... [FILE]...

Output randomly selected lines from FILE."""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-n", "--numlines",
                      action="store", dest="numlines", default=1,
                      help="output NUMLINES lines (default 1)")
    parser.add_option("-u", "--unique", action="store_true", dest="u",
                      default=False, help="output from uniquely sorted list (default false)")
    parser.add_option("-w", "--without-replacement", action="store_true",
                      dest="w", default=False, help="output probability calculated without replacement (default false)")
    options, args = parser.parse_args(sys.argv[1:])

    try:
        numlines = int(options.numlines)
    except:
        parser.error("invalid NUMLINES: {0}".
                     format(options.numlines))
    if numlines < 0:
        parser.error("negative count: {0}".
                     format(numlines))
    if len(args) < 1:
        parser.error("wrong number of operands")

    try:
        generator = randline(args)
    except IOError as xxx_todo_changeme:
        (errno, strerror) = xxx_todo_changeme.args
        parser.error("I/O error({0}): {1}".format(errno, strerror))

    if options.u:
        generator.unique()

    #Be careful, options.numlines is stored as a string by default
    if options.u or options.w:
        if int(options.numlines) > len(generator.lines):
            parser.error("Not enough lines available: {0} > {1} lines".format(options.numlines, len(generator.lines)))
	
    try:
        for index in range(numlines):
            sys.stdout.write(generator.chooseline(options.w))
    except IOError as xxx_todo_changeme1:
        (errno, strerror) = xxx_todo_changeme1.args
        parser.error("I/O error({0}): {1}".
                     format(errno, strerror))
	
if __name__ == "__main__":
    main()
