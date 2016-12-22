
n = 11 #100001
m = 10 #100000

print(n, m)

vertices = [x for x in range(1, n + 1)]
edges = []

def valid(x): return x <= n

for v in vertices:
    left = 2*v
    right = 2*v + 1
    if valid(left):
        print(v, left, 1)
    if valid(right):
        print(v, right, 1)
