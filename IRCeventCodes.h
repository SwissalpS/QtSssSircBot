#ifndef SwissalpS_QtSssSircBot_IRCEVENTCODES_H
#define SwissalpS_QtSssSircBot_IRCEVENTCODES_H

#include <qglobal.h>



namespace SwissalpS { namespace QtSssSircBot {



namespace IRCeventCodes {



const quint8 Abort = 'a';         // "socket-message"
const quint8 ChannelMessage = 'C';
const quint8 DirectMessage = 'D';
const quint8 IRCcommand = 'I';
const quint8 Joined = 'J';
const quint8 LoggedIn = 'L';
const quint8 NickList = 'N';
const quint8 Ping = 'P';
const quint8 Quit = 'Q';
const quint8 Connected = 'c';     // socket-message
const quint8 Disconnected = 'd';  // socket-message
const quint8 RawIn = '<';         // "socket-message"
const quint8 RawOut = '>';        // "socket-message"



} // namespace IRCeventCodes



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_IRCEVENTCODES_H
