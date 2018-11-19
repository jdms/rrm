#ifndef __QT_POPUP__
#define __QT_POPUP__

#include <string>

void displayNotice( std::string header, std::string info );
void deprecationNotice( std::string deprecated_method, std::string new_method, bool &popup_is_enabled );

#endif

