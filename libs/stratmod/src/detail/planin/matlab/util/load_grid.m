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


function [X, Y, Z] = load_grid( filename ) 

if ( nargin < 1 ), 
    filename = 'fgrid.mat'; 
end

file = load(filename, '-ascii'); 

m = file(1); 
n = file(2); 
l = file(3); 
lenght = m*n*l; 
if ( numel(file) ~= 3*lenght + 3 ), 
    abort('Data size mismatch.'); 
end

x_begin = 3 + 1; 
x_end = x_begin + lenght - 1; 
x_linear = file( x_begin:x_end );

y_begin = x_end + 1; 
y_end = y_begin + lenght - 1; 
y_linear = file( y_begin:y_end );

z_begin = y_end + 1;  
z_end = z_begin + lenght - 1; 
z_linear = file( z_begin:z_end );

X = reshape(x_linear, m, n, l ); 
Y = reshape(y_linear, m, n, l ); 
Z = reshape(z_linear, m, n, l ); 

end
