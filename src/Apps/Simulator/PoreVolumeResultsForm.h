#ifndef POREVOLUMERESULTSFORM_h
#define POREVOLUMERESULTSFORM_h

#include <QDialog>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QColor>

class PoreVolumeResultsForm: public QDialog
{
	Q_OBJECT
	
	public:
	
        PoreVolumeResultsForm( QWidget* parent = 0 );
		
		void createWindow();


    public slots:


        void setResults( const std::vector< QColor >& colors, const std::vector< double >& values );
		
		
	protected:
	
        QTableWidget *tbw_results;
        QGridLayout* gdl_porevolume;


};



#endif
