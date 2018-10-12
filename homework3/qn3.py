import numpy as np





def get_matrix():

    mat = np.zeros((21,20))

    for i in range(0, 10):
        for j in range(11, 20):
            mat[i,j] = 40

    for i in range(11,20):
        for j in range(0, 11):
            mat[i,j] = 40
        

    return mat



print(get_matrix())
