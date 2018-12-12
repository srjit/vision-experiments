
function sigma = estimate_noise(ImageArray, width, height, count)

    % calculation of E(i,j)
    E = double(zeros(height,width));
    %E = im2double(E)

    for k = 1:count
        for i = 1:width
            for j = 1:height
                E(i,j) = E(i,j) + double(ImageArray(k).Image(i,j));
            end
        end
    end

    E = 1/count * E;
    
    % calculation of standard deviation of E(i,j)
    sum_sq_diff = 0
    
    for k = 1:count
        for i = 1:width
            for j = 1:height
                sum_sq_diff = sum_sq_diff + (E(i,j) - double(ImageArray(k).Image(i,j)))^2;
            end
        end
    end
    
    sigma = sqrt(((1/(count -1)) * sum_sq_diff))
    
end


    