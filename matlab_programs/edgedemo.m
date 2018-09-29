% Edge Detection Demo
% Created by Octavia Camps
% Fall 2007

% read in checker image
imtool close all
close all

I = imread('checkgray.jpg');
imtool(I)


[bw,th,gv,gh] = edge(I,'sobel');
imtool(bw)
imtool(gv)
imtool(gv,[])
imtool(gh,[])

imtool close all

I = imread('boat.gif');
imtool(I)

bw = edge(I, 'canny',[0.04 0.1], 0.1);
imtool(bw)

bw = edge(I,'canny',[0.04, 0.1], 0.4);
imshow(bw)


bw = edge(I,'canny',[0.04, 0.1], 1);
imshow(bw)

bw = edge(I,'canny',[0.04, 0.1], 4);
imshow(bw)

bw = edge(I,'canny',[0.04, 0.1], 2);
imshow(bw)

bw = edge(I,'canny',[0.01, 0.1], 2);
imshow(bw)

bw = edge(I,'canny');
imshow(bw)