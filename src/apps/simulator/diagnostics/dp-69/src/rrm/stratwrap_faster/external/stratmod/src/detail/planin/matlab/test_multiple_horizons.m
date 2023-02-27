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
Selected_Planes = [ 1 2 3 4 5 6 7 8 9 10 ]; 

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
Surface3 = surface3('below', Surface1); 

%% Write data to the c++ program
[OX1, OT1, OY1] = build_mesh(Surface1);
[OX2, OT2, OY2] = build_mesh(Surface2);
[OX3, OT3, OY3] = build_mesh(Surface3);

%% Create patches from the regular grids. 
[vL1, fL1, cL1] = get_patch_from_graph(OX1, OT1, OY1, OY1);
[vL2, fL2, cL2] = get_patch_from_graph(OX2, OT2, OY2, OY2);
[vL3, fL3, cL3] = get_patch_from_graph(OX3, OT3, OY3, OY3);

% Remove from Surface2 everything below Surface1
alpha2 = remove_below_patch(vL1, vL2); 

% Remove from Surface3 everything above Surface1
alpha3 = remove_above_patch(vL1, vL3); 

%% Show final mesh 

figure 
hold on
plot3(Surface1(:,1), Surface1(:,2), Surface1(:,3), 'xb'); 
plot3(Surface2(:,1), Surface2(:,2), Surface2(:,3), 'or'); 
plot3(Surface3(:,1), Surface3(:,2), Surface3(:,3), '*g'); 
axis equal
axis off
legend('Surface1', 'Surface2', 'Surface3')
title('Input data for interpolation step.')

fig_num = 9; 
figure(fig_num)
close(fig_num)
figure(fig_num)

% % Show complete meshes
% view_surface(OX1, OT1, OY1, Surface1, fig_num)
% view_surface(OX2, OT2, OY2, Surface2, fig_num)
% view_surface(OX3, OT3, OY3, Surface3, fig_num)

% Show trimmed meshes
view_patch(vL1, fL1, cL1)
view_patch(vL2, fL2, cL2, alpha2)
view_patch(vL3, fL3, cL3, alpha3)

colormap(cmap4)
colorbar
axis equal
axis off
title('Horizons.')


