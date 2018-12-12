



function matrix = CreateCMatrix(Ex2, Ey2, ExEy, i, j)
    matrix = zeros(2,2);
    matrix(1,1) = Ex2(i-1, j-1) + Ex2(i-1, j) + Ex2(i-1, j+1) + ...
        + Ex2(i, j-1) + Ex2(i,j) + Ex2(i,j+1) + Ex2(i+1,j-1) + Ex2(i+1,j) + Ex2(i+1,j+1);
    matrix(1,2) = ExEy(i-1, j-1) + ExEy(i-1, j) + ExEy(i-1, j+1) + ...
        + ExEy(i, j-1) + ExEy(i,j) + ExEy(i,j+1) + ExEy(i+1,j-1) + ExEy(i+1,j) + ExEy(i+1,j+1);
    matrix(2,1) = matrix(1,2);
    matrix(2,2) = Ey2(i-1, j-1) + Ey2(i-1, j) + Ey2(i-1, j+1) + ...
        + Ey2(i, j-1) + Ey2(i,j) + Ey2(i,j+1) + Ey2(i+1,j-1) + Ey2(i+1,j) + Ey2(i+1,j+1);
end