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


function [ p ] = view_patch( vertex_list, face_list, vertex_color_list, alpha_list)

p = patch('Vertices', vertex_list, 'Faces', face_list, 'FaceVertexCData', vertex_color_list, 'FaceColor', 'interp', 'EdgeColor', 'black', 'EdgeAlpha', 0.15); 

if ( nargin == 4 )
    set(p, 'FaceVertexAlphaData', alpha_list, 'FaceAlpha', 'interp', 'EdgeAlpha', 'interp' ); 
end

% view(150,30); daspect([1 1 1]); %axis tight
% camlight; 
% lighting phong; 
