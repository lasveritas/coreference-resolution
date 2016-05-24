#encoding: "utf-8"
import xml.etree.ElementTree as etree
from collections import defaultdict
from collections import OrderedDict

#[Name, Surname, Patronymic, Unlabeled, NameNotNorm, Gender, Address]

tree = etree.parse('strict_out.xml')
root = tree.getroot()

for child in root.iter('NameNotNorm'):
  a = 'per {} {}'.format(child.attrib['pos'], child.attrib['len'])
  print(a)