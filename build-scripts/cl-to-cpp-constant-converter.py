import sys

f = open(sys.argv[1])
contents = f.read()
cppFile = """
#ifndef __RENDER_KERNEL_CODE
#define __RENDER_KERNEL_CODE

std::string """ + sys.argv[2] + """ =
"""

cppFile += "\n".join(["\"" + line + "\\n\"" for line in contents.split("\n")])


cppFile += ";\n"
cppFile += "#endif"

print(cppFile)
