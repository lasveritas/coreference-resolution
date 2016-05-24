#encoding: "utf-8"
import sys
import xml.etree.ElementTree as etree
from collections import defaultdict
from collections import OrderedDict


w = open(sys.argv[1], 'w')
tree = etree.parse('strict_out.xml')
root = tree.getroot()

PER = defaultdict(dict)
c = 0

for child in root.iter('StrictName'):
  if child[1].attrib['val'] == 'NONE' and child[2].attrib['val'] == 'NONE' and child[3].attrib['val'] == 'NONE':
    pass
  else:
    PER[c] = {'NameNotNorm':child[0].attrib['val'], 'Firstname': child[1].attrib['val'], 'Lastname': child[2].attrib['val'], 'Patronymic':child[3].attrib['val'], 'Gender':child[5].attrib['val'], 'Descr':child[6].attrib['val']}
    for a in ['Firstname', 'Lastname', 'Patronymic']:
      if PER[c][a].endswith('-то'):
        PER[c][a] = PER[c][a][:-3]
    c += 1

nob_part = ['де ', 'ван ', 'ля ', 'ле ','ла ', 'али ', 'эль ', 'эбд ', 'абд ', 'эль-', 'аль-', 'али-', 'эбд-', 'абд-']

def compare(a, b):
  n, m = len(a), len(b)
  if n > m:
      a, b = b, a
      n, m = m, n

  current_row = range(n+1) 
  for i in range(1, m+1):
      previous_row, current_row = current_row, [i]+[0]*n
      for j in range(1,n+1):
          add, delete, change = previous_row[j]+1, current_row[j-1]+1, previous_row[j-1]
          if a[j-1] != b[i-1]:
              change += 1
          current_row[j] = min(add, delete, change)
    
  if (len(a)>3 and len(b)>3) and current_row[n] < 3:
    return True
  elif (((len(a)<=3 or len(b)<=3)) and a == b) or b.startswith(a) or a.startswith(b):
    return True
  else:
    return False
  
def distance(a, b):
  coef = ()
  coef0 = (0, 0, 0)
  coef1 = (2.76, 3.55, 0.49)
  coef2 = (-3.18, -3.4, -0.006)
  if (a == 'NONE' or b == 'NONE'):
    coef = coef0
  elif compare(a, b):
    coef = coef1
  elif len(a.split()) > 1 and len(b.split()) == 1:
    for i in a.split():
      if compare(i, b):
        coef = coef1
    if coef == ():      
      coef = coef2            
  elif len(b.split()) > 1 and len(a.split()) == 1:
    for i in b.split():
      if compare(i, a):
        coef = coef1             
    if coef == ():      
      coef = coef2         
  elif len(b.split()) > 1 and len(a.split()) > 1: 
    temp = 0
    for i in a.split():
      for j in b.split():
        if compare(i, j):
          temp += 1
    if temp == min(len(a.split()), len(b.split())):
      coef = coef1
    else:
      coef = coef2
  elif a != b:
    coef = coef2
  return coef    

N = len(PER)      
D = []
for i in range(1, N+1):
  D.append([None]*i+[0.00]*(N-i))

max_value = []  
for i in range(len(PER)-1):
  for j in range(i+1, len(PER)):
    F_coef = distance(PER[i]['Firstname'], PER[j]['Firstname'])[0]
    S_coef = distance(PER[i]['Lastname'], PER[j]['Lastname'])[1]
    P_coef = distance(PER[i]['Patronymic'], PER[j]['Patronymic'])[2]
    if PER[i]['Gender'] == 'None' or PER[j]['Gender'] == 'None':
      G_coef = 0
    elif PER[i]['Gender'] == PER[j]['Gender']:
      G_coef = 1.257
    else:
      G_coef = -2.676
    if PER[i]['Descr'] == PER[j]['Descr']:
      D_coef = -0.6
    else:
      D_coef = -0.8
    D_coef = 0  
    if F_coef == 0 and S_coef == 0 and P_coef == 0:
      D[i][j] = 0
    elif F_coef < 0 or S_coef < 0 or G_coef < 0 or (F_coef + S_coef + P_coef + D_coef) <=0 :
      D[i][j] = None     
    else:        
      D[i][j] = round(F_coef + S_coef + P_coef  + G_coef + D_coef, 2)
      if D[i][j] > 0:
        max_value.append(((i, j), D[i][j]))

for i in range(1):        
  sort_values = sorted(max_value, key = lambda v:v[1], reverse=True)
  ENT = defaultdict(list)
  DOTS = {}


  for val in sort_values:
    val1 = val[0][0]
    val2 = val[0][1]
    if val1 not in DOTS and val2 not in DOTS:
      a = len(ENT)
      ENT[a].append(val2)
      ENT[a].append(val1)
      DOTS[val1] = a
      DOTS[val2] = a

    elif val1 not in DOTS and val2 in DOTS:  
      check = True
      for m in ENT[DOTS[val2]]:
        i = min(m, val1)
        j = max(m, val1)
        if D[i][j] == None:
          check = False
      if check == True:
        ENT[DOTS[val2]].append(val1)
        DOTS[val1] = DOTS[val2]
    
    elif val2 not in DOTS and val1 in DOTS:  
      check = True
      for m in ENT[DOTS[val1]]:
        i = min(m, val2)
        j = max(m, val2)                 
        if D[i][j] == None:
          check = False
      if check == True:
        ENT[DOTS[val1]].append(val2)
        DOTS[val2] = DOTS[val1]
    
    elif val1 in DOTS and val2 in DOTS and (DOTS[val1] != DOTS[val2]):
      check = True
      for m in ENT[DOTS[val1]]:
        for n in ENT[DOTS[val2]]:
          i = min(m, n)
          j = max(m, n)
          if D[m][n] == None:
            check = False
      if check == True:
        for r in ENT[DOTS[val2]]:
          ENT[DOTS[val1]].append(r)
          DOTS[r] = DOTS[val1]
        ENT.pop(DOTS[val2])    
   
         
  for i in PER:
    if i not in DOTS:
      w.write ('\nPER\n')
      if PER[i]['Firstname'] == 'NONE' and PER[i]['Patronymic'] == 'NONE' and len(PER[i]['Lastname'].split()) == 1:
        w.write('Lastname: '+ str(i))
      else:  
        if PER[i]['Firstname'] != 'NONE':
          F = PER[i]['Firstname']
          if '-' in F:
            w.write ('Firstname: ')
            for a in F.split():
              w.write (a+' ')
            w.write('\n') 
          else:  
            for a in PER[i]['Firstname'].split():
              w.write ('Firstname: '+a+'\n')
        if PER[i]['Lastname'] != 'NONE':
          n = False
          for nob in nob_part:
            if nob in PER[i]['Lastname'].lower():
              n = True
              print (PER[i]['Lastname'].lower())
          if n:    
            w.write ('Lastname: '+PER[i]['Lastname']+'\n')
          else:  
            for a in PER[i]['Lastname'].split() :
              if '-' not in a:
                w.write ('Lastname: '+a+'\n')  
            
        if PER[i]['Patronymic'] != 'NONE':
          w.write ('Patronymic: '+PER[i]['Patronymic']+'\n')

        
        
  for i in ENT.items():
    F = ''
    w.write('\nPER\n')
    dual = [PER[j]['Firstname'] for j in i[1] if len(PER[j]['Firstname'].split()) > 1] 
    short = [PER[j]['Firstname'] for j in i[1] if PER[j]['Firstname'] != 'NONE'] 
    if len(dual) > 0:
      F = min(dual)
      if '-' in F:
        w.write ('Firstname: ')
        for a in F.split():
          w.write (a+' ')
        w.write('\n')  
      else:  
        for a in F.split():
          w.write ('Firstname: '+a+'\n')
    elif len(short)>0:
      if len([x for x in short if len(x)>1]) > 0:
        F = min([x for x in short if len(x)>1])
      else:
        F = min(short)        
      w.write ('Firstname: '+F+'\n')
      
    dual = [PER[j]['Lastname'] for j in i[1] if len(PER[j]['Lastname'].split()) > 1] 
    short = [PER[j]['Lastname'] for j in i[1] if PER[j]['Lastname'] != 'NONE'] 
    if len(dual) > 0:
      F = min(dual)     
      n = False
      for nob in nob_part:
        if nob in F.lower():
          n = True
      if n:    
        w.write ('Lastname: '+F+'\n')
        #print (F)
      else:  
        for a in F.split() :
          if '-' not in a:
            w.write ('Lastname: '+a+'\n') 
      
    elif len(short)>0:
      if len(min(short)) > 1:
        F = min(short)
      else:
        F = short[0]        
      w.write ('Lastname: '+F+'\n')
    
    dual = [PER[j]['Patronymic'] for j in i[1] if len(PER[j]['Patronymic'].split()) > 1] 
    short = [PER[j]['Patronymic'] for j in i[1] if PER[j]['Patronymic'] != 'NONE'] 
    if len(dual) > 0:
      F = min(dual)
      for a in F.split():
        w.write ('Patronymic: '+a+'\n')
    elif len(short)>0:
      F = min(short)
      w.write ('Patronymic: '+F+'\n')
        
