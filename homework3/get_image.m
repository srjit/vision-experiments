


function image = get_image()
    image = zeros(8, 8);
    
    for x = 1:8
        for y = 1:8
            image(x,y) = abs(x-y);
        end
    end
end
