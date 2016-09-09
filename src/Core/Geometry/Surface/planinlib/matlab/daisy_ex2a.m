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
Grid_Discretization = 1; 

%% Base data structures
build_data_structures

%% Create the horizon's input data 

Surface1 = sin_horizon(0.9); 
Surface2 = sin_horizon(0.5); 
Surface3 = sin_horizon(0.1); 
[ SurfaceN, BottomN ] = valley(0.6);
SurfaceN = [ SurfaceN; BottomN ]; 

ex2a_fig_num = 20; 
figure(ex2a_fig_num)
hold off
view_points(Surface1, '.b');
hold on
view_points(Surface2, '.g'); 
view_points(Surface3, '.m'); 
view_points(SurfaceN, 'or');
legend('Surface_1', 'Surface_2', 'Surface_3', 'Surface_n'); 
axis on

%% Write data to the c++ program

write_vector3(Surface1, strcat(tmp_dir, '/ex2a_surface_1.mat'));
write_vector3(Surface2, strcat(tmp_dir, '/ex2a_surface_2.mat'));
write_vector3(Surface3, strcat(tmp_dir, '/ex2a_surface_3.mat'));
write_vector3(SurfaceN, strcat(tmp_dir, '/ex2a_surface_n.mat'));

%% Run the c++ program

cd(tmp_dir)
!./daisy_ex2a
cd(cur_dir)

%% Reads the c++ program' output data

% reads the output mesh generated by the c++ program
[vL1, fL1] = load_patch( strcat(tmp_dir, '/ex2a_s1_vlist.mat'), strcat(tmp_dir, '/ex2a_s1_flist.mat') ); 
fL1 = fL1 + 1;
cL1 = vL1(:,3); 

[vL2, fL2] = load_patch( strcat(tmp_dir, '/ex2a_s2_vlist.mat'), strcat(tmp_dir, '/ex2a_s2_flist.mat') ); 
fL2 = fL2 + 1;
cL2 = vL2(:,3); 

[vL3, fL3] = load_patch( strcat(tmp_dir, '/ex2a_s3_vlist.mat'), strcat(tmp_dir, '/ex2a_s3_flist.mat') ); 
fL3 = fL3 + 1;
cL3 = vL3(:,3); 

[vLN, fLN] = load_patch( strcat(tmp_dir, '/ex2a_sn_vlist.mat'), strcat(tmp_dir, '/ex2a_sn_flist.mat') ); 
fLN = fLN + 1;
cLN = vLN(:,3); 

%% 

figure(ex2a_fig_num)
view_patch(vL1, fL1, cL1)
view_patch(vL2, fL2, cL2)
view_patch(vL3, fL3, cL3)
view_patch(vLN, fLN, cLN)

colormap(cmap4)
colorbar
axis equal
axis off
title('Example 2A: remove above Surface_n.') 

