import sys, os

source = ""
name = sys.argv[1];

try:
    file = open( name+".txt", "rt" )
    source += "const char interface_string[] = \""
    source += file.read().encode("hex")
    source += "\";\n"
except: file  = None

try:
    file = open( name+".js", "rt" )
    source += "const char controller_string[] = \""
    source += file.read().encode("hex")
    source += "\";\n"
except: file = None

print source


