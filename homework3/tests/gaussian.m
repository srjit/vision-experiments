
%x = [-3:.1:3];
%norm = normpdf(x,0,1);
%figure;
%plot(x,norm)

k = 1;
sigma = 1.5;

G1=fspecial('gauss',[round(k*sigma), round(k*sigma)], sigma);
[Gx,Gy] = gradient(G1);   
[Gxx,Gxy] = gradient(Gx);
[Gyx,Gyy] = gradient(Gy);