from math import cos, sin, pi

def frange(start, stop, n):
  L = [0.0] * n;
  nm1 = n -1
  nm1inv = 1.0 / nm1
  for i in range(n):
    L[i] = nm1inv * (start*(nm1 - i) + stop*i)
  return L

def point_on_circle(r, theta):
  return (r*cos(theta), r*sin(theta))

print "48"

radii = [1, 1.25, 3.75, 4.0]

for m in range(0, 24):
  for r in radii:
    for theta in frange(m*pi/12, (m+1) * pi / 12, 4):
      x,y = point_on_circle(r, theta)
      if r == 1.25 or r == 3.75:
        z = -2
      else: 
        z = 0
      print "%.2f %.2f %.2f" % (x,y,z) ,
  print


for m in range(0, 24):
  for theta in frange(m*pi/12, (m+1) * pi / 12, 4):
    for r in radii:
      x,y = point_on_circle(r, theta)
      if r == 1.25 or r == 3.75:
        z = 2
      else: 
        z = 0
      print "%.2f %.2f %.2f" % (x,y,z) ,
  print 
