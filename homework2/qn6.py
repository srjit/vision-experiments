import numpy as np

__author__ = "Sreejith Sreekumar"
__email__ = "sreekumar.s@husky.neu.edu"
__version__ = "0.0.1"


def median(lst):
    sortedLst = sorted(lst)
    lstLen = len(lst)
    index = (lstLen - 1) // 2

    if (lstLen % 2):
        return sortedLst[index]
    else:
        return (sortedLst[index] + sortedLst[index + 1])/2.0

    

arr = np.zeros((8,8))


for i in range(8):
    for j in range(8):
        arr[i,j] = np.abs((i+1) - (j+1))



def get3by3neighbourhood(arr, a,b):

    l = []
    for i in range(a-1, a+2, 1):
        for j in range(b-1, b+2, 1):
            try:
                if i >= 0 and j >= 0:
                    l.append(arr[i, j])
            except:
                pass
    
    return l

op = np.zeros((8,8))

for i in range(8):
    for j in range(8):
        op[i,j] = median(get3by3neighbourhood(arr, i, j))

        
