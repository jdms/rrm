%****************************************************************************
%                                                                            
% This file is part of the "Planar Interpolation Library" (PlanInLib)        
% Copyright (C) 2016, Julio Daniel Machado Silva.                            
%                                                                            
% PlanInLib is free software; you can redistribute it and/or                 
% modify it under the terms of the GNU Lesser General Public                 
% License as published by the Free Software Foundation; either               
% version 2.1 of the License, or (at your option) any later version.         
%                                                                            
% PlanInLib is distributed in the hope that it will be useful,               
% but WITHOUT ANY WARRANTY; without even the implied warranty of             
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          
% Lesser General Public License for more details.                            
%                                                                            
% You should have received a copy of the GNU Lesser General Public           
% License along with PlanInLib.  If not, see <http://www.gnu.org/licenses/>, 
% or write to the Free Software Foundation, Inc., 51 Franklin Street,        
% Fifth Floor, Boston, MA  02110-1301  USA.                                  
%                                                                            
%****************************************************************************


function view_surface(OX, OT, OY, Points, fig_num)

global cur_dir tmp_dir bin_dir
global delta
global xmin xmax ymin ymax tmin tmax 
global line_discretization grid_discretization number_of_planes selected_planes
global p x xg tg X T 

if (nargin < 5 )
    figure;
else
    hold on
    figure(fig_num);
end

%% Show final mesh 

surfl(OX, OT, OY)
colormap bone
shading interp
hold on
plot3(Points(:,1), Points(:,2), Points(:,3),'or')
axis equal
axis off
title('Horizons'); 
legend('Interpolated Surface', 'Selected Points'); 
xlabel('x'); 
ylabel('t'); 
zlabel('y'); 

end
