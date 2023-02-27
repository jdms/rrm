```c
/*
 * This file is part of Rapid Reservoir Modelling Software.
 *   | https://rapidreservoir.org/
 *   | https://bitbucket.org/rapidreservoirmodelling/rrm/
 *
 * Copyright (c) 2022
 *   | Dmytro Petrovskyy, PhD
 *   | dmytro.petrovsky@gmail.com
 *   | https://www.linkedin.com/in/dmytro-petrovskyy/
 *
 * RRM is free software: you can redistribute it and/or modify              
 * it under the terms of the GNU General Public License as published by     
 * the Free Software Foundation, either version 3 of the License, or        
 * (at your option) any later version.                                      
 *                                                                         
 * RRM is distributed in the hope that it will be useful,                   
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            
 * GNU General Public License for more details.                             
 *                                                                         
 * You should have received a copy of the GNU General Public License        
 * along with RRM. If not, see <http://www.gnu.org/licenses/>.
 *
 */
```

# Flow diagnostics configuration file specification

## Introduction

Value formats

- _b_ boolean, ```true``` | ```false```
- _s_ string, ```"poro"```
- _d_ real, ```2.5```
- _i_ integer, ```150```
- _?_ nullable, ```null```
- _3d_ vector of 3 reals, ```[2.5, 5, 10.]```
- _2i_ vector of 2 integers, ```[300, 700]```
- _4d?_ vector of 4 nullabe reals, ```[null, 2.5, null, 10.]```
- _2d, X_ vector of 2 reals of unit X, ```[34., 20.]```
- _path_ absolute or relative path, ```"C:/test/file.txt"``` | ```"C:\\test\\file1.txt"``` | ```"../relative_dir/file.txt"```

## General

- **/base** _path_ – parent configuration that is overridden by the present file 

## Visual

Rendering settings that do not affect flow calculations.

- **/visual/exaggeration** _3d_
- **/visual/font_size** _d_ – 16 (default) for 96dpi display
- **/visual/display_property** _s_ – ```"region"``` | ```"domain"``` | ```"pressure"``` | ```"poro"``` | ```"perm_x"``` | ```"tof_forward"``` | ```"vel_h"``` | ```"grav_num"```

###

- **/visual/cutoff/type** _3i_ – cubic cut-off format, 1 for upper and -1 for lower limits, 0 disabled
- **/visual/cutoff/value** _3d_ – cubic cut-off values
- **/visual/cutoff/tof_forward** _d_ – years
- **/visual/cutoff/tof_backward** _d_ – years

###

- **/visual/axes/min** _3d_ 
- **/visual/axes/max** _3d_ 

###

- **/visual/colour_range** _JSON_ – note **/visual/display_property** for names
```json
{
  "tof_forward" : { "value": [1, 10000], "log": true },
  ...
}
```
- **/visual/colour_bar/visible** _b_
- **/visual/colour_bar/shape** _s_ – "h_top_right" | "h_bottom_left"
- **/visual/colour_bar/scale** _2d_ – [length, width]

###

- **/visual/window/maximised** _b_
- **/visual/window/viewport** _2i_ – [height, width]

###

- **/visual/camera** _JSON | path_ – Edit/Copy Camera in Main Menu
```json
{
  "focal_point": [250.0, 250.0, 253.23],
  "parallel": false,
  "parallel_scale": 243.259,
  "position": [-215.09, -1861.52, 587.54],
  "view_up": [0.015548, 0.1530, 0.9881]
}
```

###

- **/publishing** _b_ – when true, the selected well is not highlighted with blue colour

## Flow modelling settings

- **/wells** _JSON | path_ – Wells/Export in Main Menu
```json
[
  {
    "location": [2596.90, 396.23, 223.55],
    "name": "Injector",
    "perforation_md": [23.55, 100.0],
    "pressure": 30582860.0,
    "wellbore_radius": 0.1
  },
  {
    "location": [286.3, 2351.4, 223.56],
    "name": "Pos 1",
    "perforation_md": [23.56, 100.0],
    "pressure": 30000000.0,
    "wellbore_radius": 0.1
  },
  ...
]
```

- **/boundary_conditions** _6d?_, Pa – [x_min, x_max, y_min, ...]
- **/viscosity** _d_, Pa s – [x_min, x_max, y_min, ...]

###

- **/grid/size** _3d_, m
- **/grid/location** _3d_, m
- **/grid/resolution** _3i_
- **/grid/edges** _b_ – show edges

###

- **/domains** _JSON | path_ – permeability in mD, porosity in fraction
```json
[    
  { "name": "Shale", "k_xy": 0.01, "k_z": 0.001, "poro": 0.01, "rgb": [145, 145, 145] },
  { "name": "Mud", "k_xy": 0.1, "k_z": 0.01, "poro": 0.01, "rgb": [83, 53, 0] },
  ...  		
]
```


## Stratmod settings

- **/stratmod/discretisation** _2i_ – override ```SModeller::changeDiscretization(width, length)```
- **/stratmod/scale** _3d_ – scale original model
- **/stratmod/override_size** _3d_ – override original model size, **/stratmod/scale** is ignored
- **/stratmod/y_invert** _b_ – flip Y-axis




## Gravity number calculation

N<sub>bv,T</sub> Transverse buoyancy number in Table 1

Debbabi, Y., Jackson, M.D., Hampson, G.J., Salinas, P., 2018. Impact of the Buoyancy-Viscous Force Balance on Two-Phase Flow in Layered Porous Media. Transport in Porous Media 124, 263-287.
  
- **/gravity_number/delta_density** _d_, kg/m3
- **/gravity_number/viscosity_defending** _d_, Pa s
- **/gravity_number/LH_ratio** _d_


    