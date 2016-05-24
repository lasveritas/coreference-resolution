import os
from collections import defaultdict, Counter
from sklearn.linear_model import LogisticRegression

dir = './devset2'

names = ['devset2/'+x[:x.index('.')] for x in os.listdir(dir) if 'txt' in x]


X = []
y = []

for name in names:
  SPANS = {}
  OBJECTS = {}
  COREF = {}
  forall = []
  
  with open(name+'.spans') as f:
    for line in f:
      line = line.split()
      if line[1] == 'name':
        line[1] = 'Firstname'
      if line[1] == 'surname':
        line[1] = 'Lastname'
      if line[1] == 'patronymic':
        line[1] = 'Patronymic'
      SPANS[line[0]] = {line[1]: line[line.index('#')+2]}
  f.close()
  
  #print (SPANS)
  with open(name+'.objects') as f:
    for line in f:
      line = line.split()
      if line[1] == 'Person':
        OBJECTS[line[0]] = {}
        for i in range(2, line.index('#')):
          OBJECTS[line[0]].update(SPANS[line[i]])
        OBJECTS[line[0]]['Gender'] = line[line.index('@')+1]  
        OBJECTS[line[0]]['Descr'] = line[line.index('@')+2]  
        if 'Firstname' not in OBJECTS[line[0]].keys():
          OBJECTS[line[0]]['Firstname'] = 'NONE'
        if 'Lastname' not in OBJECTS[line[0]].keys():
          OBJECTS[line[0]]['Lastname'] = 'NONE'
        if 'Patronymic' not in OBJECTS[line[0]].keys():
          OBJECTS[line[0]]['Patronymic'] = 'NONE'   
  f.close()    
  

  with open(name + '.coref') as f:
    for line in f:
      line = line.split()
      if len(line) > 1:  
        if line[1] in OBJECTS:
          for i in range(1, len(line)):
            COREF[tuple(OBJECTS[line[i]].items())] = line[0]
                        
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
    elif (((len(a)<=3 or len(b)<=3)) and current_row[n] < 2) or b.startswith(a) or a.startswith(b):
      return True
    else:
      return False  
  
  def distance(a, b):
    if (a == 'NONE' or b == 'NONE'):
      coef = [0, 0]
    elif compare(a, b):
      coef = [0, 1]
    else:
      coef = [1, 0]
    return coef
  
  def extra_distance(a, b):
    if a == b:
      return [0, 1]
    else:
      return [1, 0]
    
  def union_f(a, b):
    if a == [0, 0] or b == [0, 0]:
      return [0,0]
    elif a == [0, 1] and b == [0, 1]:
      return [0,1 ]
    else:
      return [1, 0]
      
    
  def feature_vector(a, b):
    a = dict(a)
    b = dict(b)
    
    name = distance(a['Firstname'], b['Firstname'])
    surname = distance(a['Lastname'], b['Lastname'])
    patr = distance(a['Patronymic'], b['Patronymic']) 
    gender = extra_distance(a['Gender'], b['Gender'])
    descr = extra_distance(a['Descr'], b['Descr'])
    
    result = name + surname + patr + gender + descr
    
    temp = [name, surname, patr, gender, descr]
    temp2 = ['name', 'surname', 'patr', 'gender', 'descr']
    
    for i in range(len(temp)-1):
      for j in range(i+1, len(temp)):
        #print (temp2[i] + " " + temp2[j])
        result += union_f(temp[i], temp[j])
   
        
    return result    
    
  CK = list(COREF.items())

  for i in range(len(CK)-1):
    for j in range(i+1, len(CK)):
      X.append(feature_vector(CK[i][0], CK[j][0]))
      y.append(CK[i][1] == CK[j][1])
      
      
model = LogisticRegression()
model.fit([[0], [1]], [0, 1])
model.fit(X, y)
names = ['no_name', 'name', 'no_surname', 'surname', 'no_patrn', 'patrn', 'no_gender', 'gender', 'no_descr', 'descr', 'no', 'name surname', 'no','name patr', 'no','name gender', 'no','name descr', 'no','surname patr', 'no','surname gender','no', 'surname descr', 'no','patr gender','no', 'patr descr', 'no','gender descr']

c = 0
for i in model.coef_:
  print (len(i))
  for j in i:
    
    print (names[c] + " " + str(round(j, 3)))
    c += 1 
        
