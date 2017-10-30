close all

% view_curves('front', 1)
% title('front')
% 
% view_curves('back', 2)
% title('back')
% 
% view_curves('left', 3)
% title('left')
% 
% view_curves('right', 4)
% title('right')

view_curves('front', 5, 'b')
view_curves('back', 5, 'c')
view_curves('left', 5, 'r')
view_curves('right', 5, 'm')

title('Boundary curves');


function view_curves( filename, fig_num, plot_color )

if nargin >= 2
    figure(fig_num); 
else
    figure;
end

hold on

V = load(strcat(filename, '_interior.mat'), '-ascii');
[rows, cols] = size(V);

for i = 1 : rows
    C = V(i, :);
    C = reshape(C, 3, cols/3)';
    
    if nargin == 3
        plot3(C(:,1), C(:,2), C(:,3), plot_color)
    else
        plot3(C(:,1), C(:,2), C(:,3))
    end
end

B = load(strcat(filename, '_lateral.mat'), '-ascii');
[brows, bcols] = size(B);

for i = 1 : brows
    C = B(i, :);
    C = reshape(C, 3, bcols/3)';
    
    if nargin == 3
        plot3(C(:,1), C(:,2), C(:,3), plot_color)
    else
        plot3(C(:,1), C(:,2), C(:,3))
    end
end

xlabel('x')
ylabel('y')
zlabel('z')

end