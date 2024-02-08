import os
import re

regex = r"[a-zA-Z]*RM\.geo|[a-zA-Z]*\.msh|[a-zA-Z]*\.geo_unrolled"

path = os.getcwd()
dir_list = os.listdir(path)

del_list = []

for d in dir_list:
#    if  d[-3:] == "msh" or d[-3:] == "led":
#        print(d)
    m = re.match(regex,d)
    if (m):
        del_list.append(d)


print("Deleting: ")
for d in del_list:
    print(d)
print("confirm? (y/N)")

if (input() == "y"):
    for d in del_list:
        os.remove(d)
else:
    print("Aborting")
    
