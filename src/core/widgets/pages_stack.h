#ifndef PAGESSTACK_H
#define PAGESSTACK_H

#include <QStackedWidget>


#include "ui_volume_resize_widget.h"
#include "ui_object_properties.h"

class PagesStack: public QStackedWidget
{
    Q_OBJECT

    public:

        PagesStack();

        void createVolumeActions();
        void clear();

        double getWidth();
        double getHeight();
        double getLenght();


    public slots:

        void changeVolumeSize( double width, double height, double depth );
        void changeRangeSize( double width, double height, double depth );
        void setEnabledVolumeResize( bool status );


        void setVolumeWidth( double w );
        void setVolumeHeight( double h );
        void setVolumeDepth( double d );

        void loadObjectInformation( const std::string& name_, const std::string& text_ );

        void checkLowResolution();
        void checkMediumResolution();
        void checkHighResolution();

    signals:

        void widthVolumeChanged( int value );
        void heightVolumeChanged( int value );
        void depthVolumeChanged( int value );
        void saveText( const QString& text_ );

        void setLowResolution();
        void setMediumResolution();
        void setHighResolution();



    protected:

        void createVolumePropertiesPage();
        void createObjectPropertiesPage();
        void createResolutionActions();

    private:

        Ui::fm_volume wd_volume_resize;
        Ui::Form wd_object_properties;

        const int MAX_WIDTH = 10000;
        const int MAX_HEIGHT = 10000;
        const int MAX_DEPTH = 10000;

        const int SINGLE_STEP = 10;

        const bool VOLUME_RESIZING = true;
};

#endif // PAGESSTACK_H
