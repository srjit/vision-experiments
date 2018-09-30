
img = zeros(8)

for i = 0:7
    for j = 0:7
        img(i+1,j+1) = abs(i-j);
    end
end

update = medfilt2(img,[3,3]);