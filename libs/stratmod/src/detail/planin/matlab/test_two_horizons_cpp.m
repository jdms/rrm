%****************************************************************************
%                                                                            
% This file is part of the "Planar Interpolation Library" (PlanInLib)        
% Copyright (C) 2016, Julio Daniel Machado Silva.                            
%                                                                            
% PlanInLib is free software; you can redistribute it and/or                 
% modify it under the terms of the GNU Lesser General Public                 
% License as published by the Free Software Foundation; either               
% version 3 of the License, or (at your option) any later version.           
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


%% Clear workspace and set up enviroment 
init 

%% Definitions 

% Specifies the number of drawing planes
Number_of_Planes = 10;

%Selected_Planes = [ 1 2 3 4 5 6 7 8 9 10 ]; 
Selected_Planes = [ 3 5 8 ]; 

% Bounding box limits
xMin = 0; 
xMax = 1; 

yMin = 0; 
yMax = 1; 

tMin = 0; 
tMax = 1; 

% Specifies how coarse the curves (i.e. sketchs) will be 
Line_Discretization = 64; 

% Specifies how coarse the output mesh will be
Grid_Discretization = 128; 

%% Base data structures
build_data_structures

%% Create the canyon's input data (a)
Surface1 = surface1();
Surface2 = surface2('above', Surface1);  

%% Write data to the c++ program and make it do some number crunching

fprintf('\n******************** New test ********************\n\n'); 

fprintf('\nProcessing surface1: \n'); 
[vL1, fL1, cL1] = build_mesh_cpp(Surface1);

fprintf('\nProcessing surface2: \n'); 
[vL2, fL2, cL2] = build_mesh_and_remove_below(Surface2, Surface1);

%% Show final mesh 

figure 
hold on
plot3(Surface1(:,1), Surface1(:,2), Surface1(:,3), 'xb'); 
plot3(Surface2(:,1), Surface2(:,2), Surface2(:,3), 'or');  
axis equal
axis off
legend('Surface1', 'Surface2')
title('Input data for interpolation step.')

fig_num = 9; 
figure(fig_num)
close(fig_num)
figure(fig_num)

% Show meshes
view_patch(vL1, fL1, cL1)
view_patch(vL2, fL2, cL2)

colormap(cmap4)
colorbar
axis equal
axis off
title('Horizons.')

