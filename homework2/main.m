
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
variance = 4 
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

% estimate the noise - Lookup Formula ToDo
noise = estimate_noise(ImgArray, width, height, count);
disp("Estimated noise in the images: " + noise)




% -------------------    Question 3 ------------------- %
% https://www.mathworks.com/help/images/ref/fspecial.html
% https://stackoverflow.com/questions/27499057/how-do-i-create-and-apply-a-gaussian-filter-in-matlab-without-using-fspecial-im#27500953
hsize = 5;
sigma = 1.4;
gaussian_filter = fspecial('gaussian',hsize,sigma)

