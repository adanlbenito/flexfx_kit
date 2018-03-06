import sys, os

source = ""
name1 = sys.argv[1];
name2 = sys.argv[2];

try:
    file = open( name1, "rt" )
    source += "const char interface_string[] = \""
    source += file.read().encode("hex")
    source += "\";\n"
except: file  = None

try:
    file = open( name2, "rt" )
    source += "const char controller_string[] = \""
    source += file.read().encode("hex")
    source += "\";\n"
except: file = None

print source


