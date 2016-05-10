/*
 * SeismicVolume.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#include "SeismicVolume.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include<iterator>

namespace RRM
{

	SeismicVolume::SeismicVolume ( )
	{
		// TODO Auto-generated constructor stub

	}

	SeismicVolume::~SeismicVolume ( )
	{
		// TODO Auto-generated destructor stub
	}

	void SeismicVolume::getSlice ( int x , int y , int z )
	{
	}

	bool SeismicVolume::read ( std::string _filepath )
	{
		/// HARDCODED!!!!!!!!!!!!!!!!! ///////////////
		this->width = 596;
		this->height = 291;
		this->depth = 297;
		/// HARDCODED!!!!!!!!!!!!!!!!! ///////////////

		FILE *file = fopen ( _filepath.c_str ( ) , "rb" );

		unsigned long long size = width * height * depth;


		this->seismic_short = new unsigned short[width * height * depth];
		this->id 	    = new unsigned short[width * height * depth];
		this->distance 	    = new unsigned short[width * height * depth];

		this->seismic_data_.resize(size);
		this->seismic_id_.resize(size);
		this->seismic_distance_.resize(size);

		std::vector<unsigned short> seismic_data_ ( ( width * height * depth * 3 ) );

		// open the file:
		std::ifstream is
		{ _filepath.c_str ( ), std::ios::ate | std::ios::binary };

		// Stop eating new lines in binary mode!!!
		is.unsetf ( std::ios::skipws );
		// get its size:
		std::streampos fileSize;

		is.seekg ( 0 , std::ios::end );
		fileSize = is.tellg ( );
		is.seekg ( 0 , std::ios::beg );

		is.read ( reinterpret_cast<char*> ( seismic_data_.data ( ) ) , seismic_data_.size ( ) * sizeof(unsigned) );

		for ( std::size_t it = 0; it < size; it++)
		{
			this->seismic_data_[it]     = static_cast<float>(seismic_data_[(it*3)]);
			this->seismic_id_[it]       = seismic_data_[(it*3)+1];
			this->seismic_distance_[it] = seismic_data_[(it*3)+2];
		}

		// Reading Seismic, Distance and ID
//		for ( int i = 0; i < depth; i++ )
//		{
//			for ( int j = 0; j < height; j++ )
//			{
//				for ( int k = 0; k < width; k++ )
//				{
//					uint tmp1 = fread ( &this->seismic_short[i * width * height + j * width + k] , sizeof(unsigned short) , 1 , file );
//					uint tmp2 = fread ( &this->distance[i * width * height + j * width + k] , sizeof(unsigned short) , 1 , file );
//					uint tmp3 = fread ( &this->id[i * width * height + j * width + k] , sizeof(unsigned short) , 1 , file );
//				}
//			}
//		}

		int pixel = this->seismic_short[0];

		auto bounds = std::minmax_element(this->seismic_data_.begin(),this->seismic_data_.end());

		float total = (*bounds.second) - (*bounds.first);

		for ( auto& pixel_iterator: this->seismic_data_ )
		{
			pixel_iterator = pixel_iterator/total;
		}

		for ( int it = 0; it < 10; ++it )
		{
			std::cout << "RGB" << this->seismic_data_[it] << std::endl;
			std::cout << "RGB ifstream" << seismic_data_[it * 3] << std::endl;
		}

		std::cout << "Size " << fileSize << " " << ( 596 * 291 * 297 ) * sizeof(unsigned short) << std::endl;

		std::cout << "bounds " << (*bounds.first) << " - " << (*bounds.second) << std::endl;
	}

	void SeismicVolume::getTopSlice ( )
	{
	}

} /* namespace RRM */
