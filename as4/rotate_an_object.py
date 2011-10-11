import os


for i in range(0,360):
  file = """c 0 0 3
ll -0.8 -0.3 1
lr 0.9 -0.3 1
ul -0.8 1.4 1
r 640 640
d 1.0 -1.0 -1.0 1.0 1.0 1.0
d -1.0 1.0 1.0 1.0 1.0 1.0
d -1.0 -1.0 -1.0 0.5 0.5 0.5 

t 0.05 0.05 0.05 0.1 0.1 0.1 1.0 1.0 1.0 1.0 1.0 1.0 0.0 0.0 0.0 1.0 50.0 15.0 -1.0 -30.0 -15.0 -0.0 -30.0 -15.0 -0.0 30.0
t 0.05 0.05 0.05 0.1 0.1 0.1 1.0 1.0 1.0 1.0 1.0 1.0 0.0 0.0 0.0 1.0 50.0 15.0 -1.0 30.0 15.0 -0.0 -30.0 -15.0 -0.0 30.0

o 0.1 0.1 0.1 0.0 1.0 0.0 1.0 1.0 1.0 0.0 0.0 0.0 0.0 0.0 0.0 1.0 50.0 1.0 1.0 1.0 0.0 0.0 0.0 0.0 %0.1f 0.0 scenes/angelDrop_fixed.obj"""  % i

  f = open('scenes/rot-scene', 'w')
  f.write(file)
  f.close()

  os.system('./render -i scenes/rot-scene -o angel_pics/test-%.4d.png -aa 4 -ref 1 ' %i)


