#ifndef PROGRESSCOUNTER_H
#define PROGRESSCOUNTER_H

#include <QObject>

class ProgressCounter : public QObject
{
    Q_OBJECT

    public:

        explicit ProgressCounter( QObject *parent = 0 ){}

        inline void start( const unsigned int& min, const unsigned int& max ){ emit startProcess( min, max ); }
        inline void update( const unsigned int& value ){ emit updateProcess( value ); }


    signals:

        void startProcess( const unsigned int& min, const unsigned int& max );
        void updateProcess( const unsigned int& value );




};

#endif // PROGRESSCOUNTER_H
