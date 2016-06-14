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

using namespace std;

class FormPropertyValues: public QDialog
{
     Q_OBJECT



    public:

        enum PROPERTY_TYPE{ SINGLEVALUE, WELL, BOUNDARY, TOFANDTRACER };

        FormPropertyValues(  QWidget *parent = 0 );
        FormPropertyValues( PROPERTY_TYPE t, std::string name, QWidget *parent = 0  );

        void create();

        inline void setNumberofValues( int nv ){ number_of_values = nv; }
        inline int getNumberofValues() const{ return number_of_values; }

        inline void setNameofProperty( std::string name ){ name_of_property = name; }
        inline std::string getNameofProperty() const{ return name_of_property; }

        inline void setTypeofProperty( PROPERTY_TYPE t ){ type = t; }
        inline PROPERTY_TYPE getTypeofProperty() const{ return type; }

        void setValues( vector< double >& v );
        void setValues( vector< int >& v );

        inline vector< double > getValues() const{ return vector_values; }
        vector< int > getValues();


        void viewSingleValueForm();
        void viewWellForm();
        void viewBoundaryForm();
        void viewTofandTracerForm();

        inline void sendValues(){ if( vector_values.empty() == true ) return;
                                  sendPropertyValues( number_of_values, vector_values );  }


    protected:

        void createSingleValueForm();
        void createWellForm();
        void createBoundaryForm();
        void createTofandTracerForm();

        void createButton();


    protected slots:

        void on_accept_values();
        void on_reject_values();


    public slots:

        void reset();

    signals:

        void sendPropertyValues( int , vector< double >& );


    private:

        QGridLayout *gl_layout;
        QVBoxLayout *vb_layout_toftracer;
        QHBoxLayout *hb_layout_toftracer;
        QDialogButtonBox *btb_buttons;

        int number_of_values;
        std::string name_of_property;
        PROPERTY_TYPE type;

        vector< QLineEdit*> vector_edt_values;
        vector< double > vector_values;

};

#endif // FORMPROPERTYVALUES_H
