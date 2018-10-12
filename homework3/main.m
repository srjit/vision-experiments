
%

% Question 1

image = get_image();

[PGmag, PGdir] = imgradient(image,'prewitt');
[SGmag, SGdir] = imgradientxy(image);


% Question 3 - C Matrix

img2 = get_image_with_corners();
[Gx,Gy] = imgradientxy(img2);

CMatrix = zeros(2,2,21,20); 

for i=1:21
    for j=1:20
       CMatrix(:,:,i,j) = CreateCMatrix(Gx(i,j), Gy(i,j)); 
    end
end

% Diplaying values at corners
disp(CMatrix(:,:,10,11))
disp(CMatrix(:,:,11,10))
