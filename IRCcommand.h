#ifndef SwissalpS_QtSssSircBot_IRCCOMMAND_H
#define SwissalpS_QtSssSircBot_IRCCOMMAND_H

#include <QString>



namespace SwissalpS { namespace QtSssSircBot {



namespace IRCcommand {



const QString Password = "PASS";
const QString Nick = "NICK";
const QString User = "USER";
const QString Operation = "OPER";
const QString Service = "SERVICE";
const QString Quit = "QUIT";
const QString ServerQuit = "SQUIT";

const QString Join = "JOIN";
const QString Part = "PART";
const QString Mode = "MODE";
const QString Topic = "TOPIC";
const QString Names = "NAMES";
const QString List = "LIST";
const QString Invite = "INVITE";
const QString Kick = "KICK";

const QString PrivateMessage = "PRIVMSG";
const QString Notice = "NOTICE";

const QString MessageOfTheDay = "MOTD";
const QString ListUsers = "LUSERS";
const QString Version = "VERSION";
const QString Stats = "STATS";
const QString Links = "LINKS";
const QString Time = "TIME";
const QString Command = "CONNECT";
const QString Trace = "TRACE";
const QString Admin = "ADMIN";
const QString Info = "INFO";

const QString ServerList = "SERVLIST";
const QString ServerQuery = "SQUERY";

const QString Who = "WHO";
const QString WhoIs = "WHOIS";
const QString WhoWas = "WHOWAS";

const QString Kill = "KILL";
const QString Ping = "PING";
const QString Pong = "PONG";
const QString Error = "ERROR";

const QString Away = "AWAY";
const QString Rehash = "REHASH";
const QString Die = "DIE";
const QString Restart = "RESTART";
const QString Summon = "SUMMON";
const QString Users = "USERS";
const QString OperatorWall = "OPERWALL";
const QString UserHost = "USERHOST";
const QString IsOn = "ISON";



} // namespace IRCcommand



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_IRCCOMMAND_H
