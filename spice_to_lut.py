import sys, struct, random

file = open( sys.argv[1], "rt" );

xx = []
yy = []
while True:
    line = file.readline()
    if len(line) < 8: break
    line = line.replace(",","")
    line = line.split()
    xx.append( float(line[0]) )
    yy.append( float(line[1]) )

yymin = min(yy)
yymax = max(yy)

print len(yy), min(yy),max(yy)

for ii in range(0,len(yy)): yy[ii] -= yymin
for ii in range(0,len(yy)): yy[ii] /= (yymax-yymin)
for ii in range(0,len(yy)): yy[ii] = 1.999998 * (yy[ii] - 0.5)

print "{";
for ii in range(0,len(yy)):
    if ii == 0 or ii%8 == 0: sys.stdout.write("    ")
    sys.stdout.write( "FQ(%+1.6f)," % yy[ii] )
    if (ii+1)%8 == 0: print ""
print "};";
