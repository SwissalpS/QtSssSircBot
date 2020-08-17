#ifndef SwissalpS_QtSssSircBot_COMMANDEVENTCODES_H
#define SwissalpS_QtSssSircBot_COMMANDEVENTCODES_H

// abstracted event codes for interfaces.
// once for events from IRC-client(-controller) to external interfaces
// but also for events from external interfaces for AppController etc.
// FileCommandInterface is one and on lua branch there is also lua which
// sends events using these codes

#include <qglobal.h>



namespace SwissalpS { namespace QtSssSircBot {



namespace CommandEventCodesodes {



const quint8 Abort = 'a';         // "f/l-interface-message"
const quint8 ChannelMessage = 'C';
const quint8 DirectMessage = 'D';
const quint8 Exit = 'e';          // quit application f/l-interface-message too
const quint8 IRCcommand = 'I';
const quint8 Joined = 'J';        // JOIN when "f/l-interface-message"
const quint8 LoggedIn = 'L';
const quint8 NickList = 'N';      // nick change request when "f/l-interface-message"
const quint8 Part = '-';          // "interface-message"
const quint8 Ping = 'P';
const quint8 Quit = 'Q';
const quint8 Connected = 'c';     // socket-message or (re)connect f/l-interface-message
const quint8 Disconnected = 'd';  // socket-message or disconnect f/l-interface-message
const quint8 RawIn = '<';         // "socket-message"
const quint8 RawOut = '>';        // "socket-message"
const quint8 ReloadConnections = 'r'; // f/l-interface-message



} // namespace CommandEventCodes



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_COMMANDEVENTCODES_H
