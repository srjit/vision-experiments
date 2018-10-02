

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


l = [4,4,4,4,8,8,8,8]





# median filter with n=3

def get_pixels_in_range(_l, index):

    a = []

    for i in range(index-1, index+2, 1):
        try:
            if i >= 0:
                a.append(_l[i])
        except:
            pass

    import ipdb
    ipdb.set_trace()
    return a


filtered_strip = [median(get_pixels_in_range(l, i)) for i in range(len(l))]
            
