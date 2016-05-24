from collections import Counter
import re


FILE = open('raw_out.txt')
NAMES = open('tes/names.txt', 'w')
MISC = open('tes/misc.txt', 'w')


justnames = Counter()
misc = Counter()
person = Counter()

name = ""
for line in FILE:
  line = line.strip()
  if 'Name =' in line: 
    name = line[line.index("=")+2:]
    name = name.lower()
    name = re.sub('"', '', name)
    justnames[name] +=1
  
  if 'Misc_check = true' in line:
    misc[name] += 1
  if 'Pers_check = true' in line:
    person[name] += 1

misc_substr = ['стрит', 'парк', 'сайд', 'сити', 'надзор']

for name in sorted(justnames):
  c = 0
  for substr in misc_substr:
    if substr in name:
      MISC.write(name+"\t"+str(justnames[name])+"\n")
      c = 1
      break
  if c == 0:    
    if misc[name] > person[name]:
      MISC.write(name+"\n")
    else:
      NAMES.write(name + "\n")
