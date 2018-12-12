
% create the image

image = zeros(8,8);
image(6,1) = 1;
image(7,2) = 1;
image(8,2) = 1;
image(6,3) = 1;
image(2,2) = 2;
image(6,2) = 2;
image(2,4) = 2;
image(6,6) = 2;
image(4,7) = 2;
image(5,7) = 2;
image(6,8) = 2;
image(3,2) = 2;
image(2,3) = 4;
image(4,3) = 2;
image(6,7) = 4;


%create the patch
template = zeros(3,3);
template(1,1) = 1;
template(1,3) = 1;
template(2,2) = 1;
template(3,2) = 1;
template(1,2) = 2;

% calculation of SSD
f = image;
g = template;
ssd = zeros(8,8);

for i = 2:7
    for j = 2: 7
           var  = (f(i-1, j-1) - g(1,1)).^2 + ...
                + (f(i-1, j) - g(1,2)).^2+ ...
                + (f(i-1, j+1) - g(1,3)).^2 + ...
                + (f(i, j-1) - g(2,1)).^2 + ...
                + (f(i,j) - g(2,2)).^2 + ...
                + (f(i,j+1) - g(2,3)).^2 + ...
                + (f(i+1,j-1) - g(3,1)).^2 + ...
                + (f(i+1,j) - g(3,2)).^2 + ...
                + (f(i+1,j+1) - g(3,3)).^2;
       ssd(i,j) = sum(sum(var));
    end
end
        

ssd(1,:) = 0;
ssd(:,1) = 0;
ssd(8,:) = 0;
ssd(:,8) = 0;

disp("SSD is:");
disp(ssd)

% Part 2
cross_correlation_score = imfilter(image,template);
cross_correlation_score(1,:) = 0;
cross_correlation_score(:,1) = 0;
cross_correlation_score(8,:) = 0;
cross_correlation_score(:,8) = 0;
disp("Cross Correlation score");
disp(cross_correlation_score);

%Part 3 
% Normalized Cross Correlation

%normxcorr2(template, image)










normalized_image = zeros(8,8);
%normalized_template = zeros(3,3);

for i=2:7
    for j=2:7
      denominator = f(i-1, j-1).^2 + ...
                  + f(i-1, j).^2 + ...
                  + f(i-1, j+1).^2 + ...
                  + f(i, j-1).^2 ...
                  + f(i,j).^2 ...
                  + f(i,j+1).^2 ...
                  + f(i+1,j-1).^2 ...
                  + f(i+1,j).^2 ...
                  + f(i+1,j+1).^2;
       denominator = sqrt(denominator);
       if denominator == 0
        normalized_image(i,j) = 0;
       else
        normalized_image(i,j) = (f(i,j) / denominator);
       end
    end
end

% denominator = sqrt(g(1,1).^2 + g(1,2).^2 + g(1,3).^2 + g(2,1).^2 ...
%     + g(2,2).^2 + g(2,3).^2 + g(3,1).^2 + g(3,2).^2 + g(3,3).^2);

normalized_template = g./sqrt(sum(sum(g.^2)));
normalized_cross_correlation = imfilter(normalized_image,normalized_template);

normalized_cross_correlation(1,:) = 0;
normalized_cross_correlation(:,1) = 0;
normalized_cross_correlation(8,:) = 0;
normalized_cross_correlation(:,8) = 0;

disp("Normalized Cross Correlation");
disp(normalized_cross_correlation);