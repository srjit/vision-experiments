
%

% Question 1

image = get_image();

[PGmag, PGdir] = imgradient(image,'prewitt');
[SGmag, SGdir] = imgradientxy(image, 'sobel');


% Question 3 - C Matrix

img2 = get_image_with_corners();
maskx = [-1 -1 -1;0 0 0;1 1 1];
Gx = conv2(img2, maskx);
masky = [-1 0 1;-1 0 1;-1 0 1];
Gy = conv2(img2, masky);



Ex2  = Gx.*Gx;
Ey2  = Gy.*Gy;
ExEy = Gx.*Gy;
%EyEx = Gy.*Gx;



%[Gx,Gy] = imgradientxy(img2);

CMatrix = zeros(2,2,21,20); 

% Excluding borders while summing
for i=2:20
    for j=2:19
       CMatrix(:,:,i,j) = CreateCMatrix(Ex2, Ey2, ExEy, i, j); 
    end
end


for i=1:19
    for j=1:18
        CM = CMatrix(:,:,i,j);
        if CM(2) == 0  && CM(3) == 0 && CM(1) > 10000 && CM(4) > 10000
            fprintf("(%d,%d)\n", i, j); 
        end
    end
end
            
    

% Diplaying values at corners
%disp(CMatrix(:,:,10,11))
%disp(CMatrix(:,:,11,10))
