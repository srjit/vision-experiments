



function matrix = CreateCMatrix(Ex, Ey)
    matrix = zeros(2,2);
    matrix(1,1) = power(Ex, 2);
    matrix(1,2) = Ex * Ey;
    matrix(2,1) = Ey * Ex;
    matrix(2,2) = power(Ey, 2);
end