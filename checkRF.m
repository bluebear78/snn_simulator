
close all;

uni = ones(8,1);
dec = (7:-1:0)'/7;

map=[dec,dec,uni];

colormap(map);

for i = 0:35
  name = sprintf('output_%d',i);
  RF = load(name);
  subplot(6,6,i+1);
  image(RF);
end

