% -------------------    Question 1 ------------------- %

count = 10;
width = 256;
height = 256;


% 10 grayscale images and store in a stucture
ImgArray(1:count) = struct('Image', [], 'Label', '');
for j = 1:count
  ImgArray(j).Image = 128 * ones(256,'uint8');
  ImgArray(j).Label = j;
end