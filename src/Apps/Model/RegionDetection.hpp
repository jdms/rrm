/*
 * RegionDetection.hpp
 *
 *  Created on: Jun 1, 2016
 *      Author: felipe
 */

#ifndef _MODEL_REGIONDETECTION_HPP_
#define _MODEL_REGIONDETECTION_HPP_

#include <iostream>
//Algorithms
#include <algorithm>
//Container
#include <vector>
#include <map>
#include <set>


namespace RRM
{

	class RegionDetection
	{
		public:
			typedef std::pair<unsigned int, unsigned int> 			Path;
			typedef std::vector<std::pair<unsigned int, unsigned int> > 	Cycle;

			struct SetComp
			{
					bool operator() ( const Cycle& left , const Cycle& right ) const
					{
						return left.size ( ) <= right.size ( );
					}
			};

			typedef std::set<Cycle,SetComp> 				Cycles;
			typedef std::map<unsigned int,Cycle>				AdjancencyMatrix;

			void printCycle( const Cycle& _c )
			{
				for (auto& c: _c)
				{
					std::cout << "(" << c.first << "," <<  c.second << ")";
				}
				std::cout << std::endl;
			}

			void printCycles( const Cycles& _cs)
			{
				std::cout << "--Cycles--" << std::endl;
				for (auto& c: _cs)
				{
					printCycle(c);
				}
				std::cout << std::endl;
			}

			void printGraph ( const AdjancencyMatrix& _vm )
			{
				std::cout << "--Graph--" << std::endl;
				for (auto& g: _vm)
				{
					printCycle(g.second);
				}
				std::cout << std::endl;
			}
			/// Rotate Cycle to the small Vertex, in order to avoiding duplicate cycles
			void rotateSmall ( Cycle&  _c )
			{
				Cycle::iterator it;
				/// Find the smallest vertex
				it = (std::min_element(_c.begin(),_c.end(),[](Path& left,Path& right)
						       {
								return ( (left.first  <=  right.first)  );
						       }
				));
				Cycle temp;
				/// Reordering
				temp = Cycle(it,_c.end());
				std::copy (_c.begin(),it,back_inserter(temp));
				_c = temp;
			}

			Cycle subCycle(Path vertex, Cycle c)
			{
				Cycle::iterator it;

				it = std::find(c.begin(),c.end(),vertex);

				return Cycle(it,c.end());
			}

			bool is_visited ( const Path& _path, const Cycle& _c )
			{

				return (std::find_if(_c.begin(),_c.end(),[_path](Path curr) {return curr.first == _path.first; }) != _c.end());

			}

			void findCycles ( Path parent, Path current_vertex, Cycle current_path, AdjancencyMatrix& cs, Cycles& cycles)
			{
				Cycle edges = cs[current_vertex.first];

				for ( auto e: edges)
				{
					if ( e.first == parent.first)
					{
						continue;
					}
					if ( !is_visited(current_vertex,current_path) )
					{
						Cycle subPath = current_path;
						subPath.push_back(current_vertex);
					        findCycles(current_vertex,e,subPath,cs,cycles);
					}else if (current_path.size() > 2)
					{
						Cycle c = subCycle(current_vertex,current_path);
						Cycle c_inv = c;
						rotateSmall(c);
						std::reverse(c_inv.begin(),c_inv.end());
						rotateSmall(c_inv);

						Cycles::iterator it;
						Cycles::iterator it_inv;

						it = std::find(cycles.begin(),cycles.end(),c);
						it_inv = std::find(cycles.begin(),cycles.end(),c_inv);

						if ( (it == cycles.end() ) and  (it_inv == cycles.end() ) )
						{
							cycles.insert(c);
						}

						break;

					}
				}

			}

			void findCycles( AdjancencyMatrix& cs, Cycles& cycles )
			{

				Cycle edges = cs[1];
				Cycle current_path = {std::make_pair(1,0)};

				for (auto e: edges)
				{
					findCycles(std::make_pair(1,0),e,current_path,cs,cycles);
				}

			}

			AdjancencyMatrix vm_;
			Cycles regions_;
	};


} /* namespace RRM */

#endif /* SRC_APPS_MODEL_REGIONDETECTION_HPP_ */
