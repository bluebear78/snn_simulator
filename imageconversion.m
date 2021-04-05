function [vector] = imageconversion(filename)

xgrid = 28;
ygrid = 28;

info = imfinfo(filename);

image = imread(filename);

if(strcmp(info.ColorType,'truecolor') == 1)
  image = rgb2gray(image);
end

ysize = size(image,1);
xsize = size(image,2);

imagemat1 = zeros(ysize,xgrid);
imagemat2 = zeros(ygrid,xgrid);

if(xgrid > xsize)
  for x = 1:xgrid
    ind = ceil((x-0.5)*xsize/xgrid);
    for y = 1:ysize
      imagemat1(y,x) = image(y,ind);
    end
  end
elseif(xgrid < xsize)
  for x = 1:xgrid
    inds = ceil((x-1)*xsize/xgrid+0.5):ceil(x*xsize/xgrid-0.5);
    for y = 1:ysize
      imagemat1(y,x) = mean(image(y,inds));
    end
  end
else
  imagemat1 = image;
end

if(ygrid > ysize)
  for y = 1:ygrid
    ind = ceil((y-0.5)*ysize/ygrid);
    for x = 1:xgrid
      imagemat2(y,x) = imagemat1(ind,x);
    end
  end
elseif(ygrid < ysize)
  for y = 1:ygrid
    inds = ceil((y-1)*ysize/ygrid+0.5):ceil(y*ysize/ygrid-0.5);
    for x = 1:xgrid
      imagemat2(y,x) = mean(imagemat1(inds,x));
    end
  end
else
  imagemat2 = imagemat1;
end

image2 = mat2gray(imagemat2);
image2 = im2bw(image2,mean(mean(image2))*0.92);
imshow(image2)
vector = find(image2' < 0.5)';
