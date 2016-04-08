/*
 * IDManager.hpp
 *
 *  Created on: Mar 31, 2016
 *      Author: felipe
 */

#ifndef _MODEL_IDMANAGER_HPP_
#define _MODEL_IDMANAGER_HPP_

#include <algorithm>
#include <numeric>
#include <cassert>
#include <set>


class IDManager
{
	public:
		// 0 is the neutral element
		typedef IDManager 					  Self;
		typedef std::set<unsigned int, std::less<unsigned int > > id_list;

		IDManager ( )
		{
			max_id_ = 0;
		}

		IDManager ( const Self& _id_manager )
		{
			*this = _id_manager;
		}

		~IDManager ( )
		{

		}

		Self& operator=(const Self& other )
		{
			// Assign to all  member.
			free_ids_ = other.free_ids_;
			used_ids_ = other.used_ids_;
			max_id_   = other.max_id_;

			return *this;
		}

		void initialize( unsigned int _max_id )
		{
			assert( "A must be bigger than 1" && _max_id > 1 );

			max_id_ = _max_id;

			free_ids_.clear();

			std::vector<unsigned int> initial_ids ( _max_id );
			std::iota ( initial_ids.begin ( ) , initial_ids.end ( ) , 1 );

			free_ids_ = id_list ( initial_ids.begin ( ) , initial_ids.end ( ) );
		}
		//FIXME return boolean to check if the id exist
		unsigned int getID ( )
		{

			if ( free_ids_.size ( ) == 0 )
				return 0;

			unsigned int id = ( *free_ids_.begin ( ) );

			free_ids_.erase ( id );
			used_ids_.insert ( id );

			return id;
		}

		void returnID ( unsigned int _id )
		{
			assert( "It cannot return id 0" && ( ( _id == 0 ) || ( _id >= max_id_ ) ) );

			used_ids_.erase ( _id );
			free_ids_.insert ( _id );
		}



	public:
		std::set<unsigned int, std::less<unsigned int > > free_ids_;
		std::set<unsigned int, std::less<unsigned int > > used_ids_;

		unsigned int max_id_;
};
#endif /* _MODEL_IDMANAGER_HPP_ */
