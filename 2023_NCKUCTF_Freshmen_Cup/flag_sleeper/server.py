from secret import FLAG
import sys
import time

print("Shhh... flag is sleepy")
x = input()

if len(x) > len(FLAG):
    print("Stop!")
    sys.exit()

correct = 0

for i in range(len(x)):
    if x[i] == FLAG[i]:
        correct += 1
    else:
        break

time.sleep(correct)
print("Stop!")
sys.exit()
