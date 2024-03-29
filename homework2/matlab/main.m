
%  Author: Sreejith Sreekumar

% -------------------    Question 1 ------------------- %

count = 10;
width = 256;
height = 256;


% 10 grayscale images and store in a stucture
ImgArray(1:count) = struct('Image', [], 'Label', '');
for j = 1:count
  ImgArray(j).Image = grayscale_generator(width, height);
  ImgArray(j).Label = j;
end

% add additive zero-mean Gaussian noise to all those images
variance = (2*2)/(255*255)
for j = 1:count
    ImgArray(j).Image = imnoise(ImgArray(j).Image,'gaussian', 0, variance);
end

noise = estimate_noise(ImgArray, width, height, count);
disp("Estimated noise in the images: " + noise)




% -------------------    Question 2 ------------------- %

% 3*3 Box Filter 
filterSize = 3;
for j = 1:count
    ImgArray(j).Image = imboxfilt(ImgArray(j).Image, filterSize);
end

% estimate the noise - Lookup Formula ToDo55Z5
noise = estimate_noise(ImgArray, width, height, count);
disp("Estimated noise in the images: " + noise)



% -------------------    Question 3 ------------------- %
% Ref: 3433https://www.mathworks.com/help/images/ref/fspecial.html
hsize = 7;
sigma = 1.4;
gaussian_filter = fspecial('gaussian',hsize,sigma)
