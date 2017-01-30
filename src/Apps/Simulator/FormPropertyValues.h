/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef FORMPROPERTYVALUES_H
#define FORMPROPERTYVALUES_H

#include <vector>

#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QInputDialog>
#include <QFrame>

using namespace std;

class FormPropertyValues: public QDialog
{
     Q_OBJECT



    public:

        enum PROPERTY_TYPE{ REGIONS, WELL };

        FormPropertyValues(  QWidget *parent = 0 );
        FormPropertyValues( PROPERTY_TYPE t, QWidget *parent = 0  );


        void create();

        void createRegionPropertiesForm( const int np, const std::vector< double >& values , const std::vector< double >& perm, const std::vector< double >& poros, const std::vector< double >& visc );
        void createRegionPropertiesForm( const int np );


        void createWellForm( const int nw );
        void createWellForm( const int nw, const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign );


        inline int getNumberofRegions(){ return number_of_regions; }
        inline int getNumberofWells(){ return number_of_wells; }



        inline void setTypeofProperty( PROPERTY_TYPE t ){ type = t; }
        inline PROPERTY_TYPE getTypeofProperty() const{ return type; }



        void viewRegionPropertiesForm();
        void viewWellForm();


        std::vector< double > getRegionPositions();
        std::vector< double > getRegionPermeability();
        std::vector< double > getRegionPorosity();
        std::vector< double > getRegionViscosity();



        std::vector< unsigned int > getWellTypes();
        std::vector< double > getWellValues();
        std::vector< int > getWellSigns();


    protected:

        void createButton();


    protected slots:

        void on_accept_values();
        void on_reject_values();


    public slots:

        void reset();


    private:

        QGridLayout *gl_layout;
        QDialogButtonBox *btb_buttons;


        std::vector< QLineEdit* > vector_positions_values;
        std::vector< QLineEdit* > vector_perm_values;
        std::vector< QLineEdit* > vector_poros_values;
        std::vector< QLineEdit* > vector_visc_values;


        std::vector< QLineEdit* >vector_type_values;
        std::vector< QLineEdit* >vector_valuewell_values;
        std::vector< QLineEdit* >vector_sign_values;


        PROPERTY_TYPE type;

        int number_of_regions;
        int number_of_wells;


};

#endif // FORMPROPERTYVALUES_H
