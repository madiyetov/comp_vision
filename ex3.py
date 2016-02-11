import argparse

def sup(arr):
    s = 1
    for x in arr:
        s *= x
    return s

parser = argparse.ArgumentParser(description='Get info from arguments')

parser.add_argument('--int', type=int, required=True, 
                    help='enter some integer like 1')
parser.add_argument('--flt', type=float, required=True, 
                    help='enter some float like 5.1')
parser.add_argument('boolean', metavar='B', type=bool, nargs='?',
                   help='enter boolean like True, False')

args = parser.parse_args()
print args