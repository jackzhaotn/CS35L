#!/usr/bin/python

import argparse
import sys
import random


class input_to_lines:
    def __init__(self, inputfile, input_range):
        self.lines = []
        if inputfile and input_range:
            sys.exit("Input range incompatible with input file")
        if input_range:
            try:
                lo, hi = input_range.split('-')
                if lo > hi:
                    sys.exit("lo must be less than high")
                self.lines = list(range(int(lo), int(hi)+1))
            except:
                sys.exit("Invalid input range")
        elif inputfile == "-" or not inputfile:
            try:
                for line in sys.stdin.readlines():
                    self.lines.append(line.rstrip('\n'))
            except:
                sys.exit("Input is unreadable")

        elif inputfile:
            try:
                with open(inputfile) as infile:
                    for line in infile:
                        self.lines.append(line.rstrip('\n'))
            except:
                sys.exit("Invalid input file")

class shuf:
    def __init__(self, lines):
        self.lines = lines
        random.shuffle(self.lines)

    def generate(self, count):
        if count:
            self.count = int(count)
            if self.count > len(lines):
                self.count = len(lines)
        else:
            self.count = len(lines)

        for i in range(0, self.count):
            print(self.lines[i])

    def rgenerate(self, count):
        if count:
            self.count = int(count)
            while (self.count > 0):
                print(random.choice(self.lines))
                self.count -= 1
        else:
            while True:
                print(random.choice(self.lines))
                
        

if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument("-n", "--head-count", action="store", dest="head_count",
                        default = '', help="output at most COUNT lines")

    parser.add_argument("-i", "--inputrange", action="store", dest="input_range",
                        default = '',
                        help="treat each number LO through HI as an input line")

    parser.add_argument("-r", "--repeat", action="store_true", dest="repeat",
                        default=False, help="output lines can be repeated")
    
    parser.add_argument("infile", nargs="*")
    
    
    args = parser.parse_args()
    if args.head_count:
        try:
            test = int(args.head_count)
        except:
            sys.exit("Invalid head count")
        if int(args.head_count) < 0:
            sys.exit("Negative head count")


    if len(args.infile) > 1 :
        sys.exit("Extraneous arguments")
    elif len(args.infile) == 0:
        infile = ""
    else:
        infile = args.infile[0]

    itl = input_to_lines(infile, args.input_range)
    lines = itl.lines

    shuf = shuf(lines)

    if args.repeat:
        shuf.rgenerate(args.head_count)
    else:
        shuf.generate(args.head_count)
    
    
