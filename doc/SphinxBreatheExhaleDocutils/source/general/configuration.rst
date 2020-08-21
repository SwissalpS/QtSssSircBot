Configuration
==============

Command Line Arguments
-----------------------
You may prefer to set certain settings from command line.

If all you want is a bot that connectst to a single network, you can pass all
parametrs on command line without needing to make a JSON configuration file.

For a list of supported arguments and brief description of them, do:

.. code-block:: sh

    ./QtSssSircBot -h

Arguments passed on command line override values from Settings.ini.

- --backdoor </path/to/backdoor>
    Defaults to ~/Documents/QtSssSircBot/backdoor (OS dependant)
    Commands can be written to this file `read more... <backdoor.html>`_
- --channels <commaListOfChannels>
    Example: #home,#help,#dev
- --config </path/to/config.json>
    Defaults to none -> uses settings passed by other arguments and falls back
    to whatever is in Settings.ini
- --host <host>
    Remote host to use. Example: irc.freenode.net
- --nicks <nicks>
    Example: MyFav,MyAlt,MyOhWell
    QtSssSircBot will attempt to login is 'MyFav', if that is not available, it
    will try 'MyAlt', then 'MyOhWell' and if that fails it will append
    underscores: 'MyOhWell\_', 'MyOhWell\_\_', ...
- --pass <password>
    Network password to use.
- --port <port>
    Remote port to use for secure connection (SSL/TLS)
- --realname <realname>
    Realname to use


JSON Config File
-----------------------

Example JSON config file setting up two connections:

.. code-block:: json

    [{
        "sConnectionID": "fn",
        "sIRCrawPostLoginLines": [],
        "sIRCrawPreLoginLines": [],
        "sIRCremoteChannels": [
            "#SwissalpS",
            "#SwissalpS.mix",
            "#lite",
            "#minetest",
            "#pandorabox"
        ],
        "sIRCremoteHost": "irc.freenode.net",
        "sIRCremoteNicks": [
            "QtSssSircBot",
            "QtSwissalpSbot"
        ],
        "sIRCremotePassword": "",
        "sIRCremoteRealname": "Qt-IRC-bot written by SwissalpS",
        "uiIRCremotePort": 6697
    },
    {
        "sConnectionID": "cn",
        "sIRCrawPostLoginLines": [],
        "sIRCrawPreLoginLines": [],
        "sIRCremoteChannels": [
            "#home"
        ],
        "sIRCremoteHost": "irc.foo.bar",
        "sIRCremoteNicks": [
            "QtSssSircBot",
            "QtSwissalpSbot"
        ],
        "sIRCremotePassword": "Secret",
        "sIRCremoteRealname": "QtSwissalpSbot",
        "uiIRCremotePort": 6697
    }]

Most field labels are the same as used in Settings.ini.

- **sConnectionID** keep ID short and alpha(-numeric)
  this string identifies the connection, thus needs to be *unique*.
- **sIRCrawPostLoginLines** array of strings holding raw IRC lines to send to host after login.
  Example authenticate with nick server.
- **sIRCrawPreLoginLines** array of strings holding raw IRC lines to send to host before login
  right after connection has been established.
- **sIRCremoteChannels** array of strings of channel names to join after login.
- **sIRCremoteHost** the host to connect to: IP or FQDN
- **sIRCremoteNicks** array of strings to use as nicks. (Will use leftmost then fall back on next in list)
- **sIRCremotePassword** network's password
- **sIRCremoteRealname** realname to use when loggin on
- **uiIRCremotePort** port to use for connection. Defaults to 6697

Settings.ini File
-----------------------
On first launch of QtSssSircBot, Settings.ini is created and populated with
default values. You will need to edit these, pass on command line or make a
JSON config file.

Example Settings.ini::

    sIRCconfigPath=~/Documents/QtSssSircBot/config/setB.json
    sIRCremoteChannels="#SwissalpS,#home"
    sIRCremoteHost=irc.freenode.net
    sIRCremoteNicks="QtSwissalpSbot,QtSssSbot"
    sIRCremotePassword=
    sIRCremoteRealname=QtSwissalpSbot
    sPathFileBackdoor=~/Documents/QtSssSircBot/backdoor
    sPathFilePID=~/Documents/QtSssSircBot/QtSssSircBot.pid
    uiBackdoorIntervalMS=1800
    uiIRCremotePort=6697

- **sIRCconfigPath** is overridden by ``--config``
- **sIRCremoteChannels** is overridden by ``--channels``
- **sIRCremoteHost** is overridden by ``--host``
- **sIRCremoteNicks** is overridden by ``--nicks``
- **sIRCremotePassword** is overridden by ``--pass``
- **sIRCremoteRealname** is overridden by ``--realname``
- **sPathFileBackdoor** is overridden by ``--backdoor``
- **sPathFilePID** QtSssSircBot's PID is written into this file
- **uiBackdoorIntervalMS** defaults to 3 minutes, how often backdoor file is checked
- **uiIRCremotePort** is overridden by ``--port`` Defaults to 6697

