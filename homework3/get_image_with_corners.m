

function image = get_image_with_corners()
    image = zeros(21,20);
    for x = 11:21
        for y = 1:10
            image(x,y) = 40;
        end
    end
    
    for x = 1:10
        for y = 11:20
            image(x,y) = 40;
        end
    end    
end
    