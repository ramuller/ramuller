#!/bin/env python

import re
# from string import strip

fn = './resources/language/resource.language.de_de/strings.po'

msgDict = {}

reMsgctxt = re.compile('msgctxt*')

with open(fn) as fp:
    lines = fp.readlines()
    print('Number of lines : {}'.format(len(lines)))
    for line in lines:
        if re.match('msgctxt*', line):
            print('{}'.format(line))
                  
