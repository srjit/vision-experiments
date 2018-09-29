[R,C] = meshgrid(-400:399);
d = sqrt(R.^2 + C.^2);
mask = d <= 375;          %you do not want the circle to go edge to edge
d(~mask) = 0;             %outside circle is black
maxd = max(d(:));
gradient = d ./ maxd * 0.75;     %so 0 at center and 0.75 at radius
tone_down_by = 0.9;
d(mask) = (1 - gradient(mask)) .* tone_down_by;
imagesc(d, [0 1]); colormap(gray(256));