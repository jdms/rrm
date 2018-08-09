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


function [ lof ] = lambda( x, y, z, alphai, delta ) 

% alphai = 
% [ x_i y_i z_i, beta_x_i beta_y_i beta_z_i, v_x_i v_y_i v_z_i ];
%
% lambda_i(f)(x,y,z) = < D^{\beta_i}f( [x, y, z] - [x_i, y_i, z_i] ), v_i >

% d = 3; 
% wi = alphai(1:d); 
% betai = alphai(d+1:2*d); 
% vi = alphai(2*d+1:3*d);

d = 3; 
mi = numel(alphai); 

switch mi,
    case 3
        wi = alphai(1:d);
        betai = [ 0 0 0 ];
        ui = [ 1 0 0 ];
        vi = [ 1 0 0 ];
        
    case 6
        wi = alphai(1:d);
        betai = alphai(d+1:2*d);
        ui = [ 1 0 0 ];
        vi = [ 1 0 0 ];
        
    case 9
        wi = alphai(1:d);
        betai = alphai(d+1:2*d);
        ui = alphai(2*d+1:3*d);
        vi = alphai(2*d+1:3*d);
        
    case 12
        wi = alphai(1:d);
        betai = alphai(d+1:2*d);
        ui = alphai(2*d+1:3*d);
        vi = alphai(3*d+1:4*d);
end

%index = strcat( int2str(betai(1)), int2str(betai(2)), int2str(betai(3) ) ); 
index = sprintf('%d', betai ); 

switch index, 
    case '000'
        lof = phi( x-wi(1), y-wi(2), z-wi(3), delta ); 
        
    case '100'
        lof = -Dphi( x-wi(1), y-wi(2), z-wi(3), 1, delta);
        
    case '010'
        lof = -Dphi( x-wi(1), y-wi(2), z-wi(3), 2, delta);
        
    case '001'
        lof = -Dphi( x-wi(1), y-wi(2), z-wi(3), 3, delta);
        
    case '111'
        lof = 0; 
        for a = 1 : 3, 
            lof = lof -Dphi( x-wi(1), y-wi(2), z-wi(3), a, delta) * ui(a); 
        end 
        
    case '222'
        lof = 0; 
        for a = 1 : 3,
            for b = 1 : 3,
                lof = lof + D2phi( x-wi(1), y-wi(2), z-wi(3), a, b, delta ) * ui(a) * vi(b); 
            end
        end
end

end
