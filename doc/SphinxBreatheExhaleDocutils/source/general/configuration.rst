Configuration
==============

Settings.ini File
-----------------------
Application level settings and defaults for connection.
On first launch of QtSssSircBot, Settings.ini is created and populated with
default values in `the workdir`_. You will need to either:

    - edit these in Settings.ini
    - pass parameters on command line (only single connection)
    - or make a JSON config file and pass it's path on command line or in Settings.ini

Example Settings.ini:

.. code-block:: ini

    [General]
    sIRCconfigPath=config/setB.json
    sIRCremoteChannels="#SwissalpS,#home"
    sIRCremoteHost=irc.bar.foo
    sIRCremoteNicks="QtSwissalpSbot,QtSssSbot"
    sIRCremotePassword=
    sIRCremoteRealname="QtSwissalpSbot"
    sPathFileBackdoor=backdoor
    sPathFilePID=QtSssSircBot.pid
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

Comma separated lists must be in double quotes and should not contain spaces.

Realname may be set without quotes if it does not contain anything but alpha numerics.

Details about :ref:`CLArguments`

.. _`JSONConfigFile`:

JSON Config File
-----------------------

Example JSON config file setting up two connections 'fn' and 'cn':

.. code-block:: json

    [{
        "sConnectionID": "fn",
        "sIRCrawPostLoginLines": [ "PRIVMSG NickServ identify 476ed1sg" ],
        "sIRCremoteChannels": [
            "#lite",
            "#minetest",
            "#pandorabox"
        ],
        "sIRCremoteHost": "irc.freenode.net",
        "sIRCremoteNicks": [
            "AndrewBot",
            "SmithBot"
        ],
        "sIRCremoteRealname": "Qt-IRC-bot-that-wants-jokes",
        "uiIRCremotePort": 6697
    },
    {
        "sConnectionID": "cn",
        "sIRCremoteChannels": [
            "#base"
        ],
        "sIRCremoteHost": "irc.foo.bar",
        "sIRCremoteNicks": [
            "AndrewSmith",
            "Smith"
        ],
        "sIRCremotePassword": "Secret",
        "sIRCremoteRealname": "Andrew Smith"
    }]

(this is not a functional sample)

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

You can save your config.json files anywhere. Suggested place is in :ref:`the workdir`
Example: ~/Documents/QtSssSircBot/config/botC.json.
Prefer absolute paths. Relatvie paths start at `the workdir`_.

.. _`CLArguments`:

Command Line Arguments
-----------------------
You may prefer to set certain settings from command line.

If all you want is a bot that connects to a single network, you can pass all
parameters on command line without needing to make a JSON configuration file.

For a list of supported arguments and brief description of them, do:

.. code-block:: sh

    ./QtSssSircBot -h

Arguments passed on command line override values from Settings.ini.

- --backdoor </path/to/backdoor>
    Defaults to 'backdoor' in :ref:`the workdir`
    Commands can be written to this file `read more... <backdoor.html>`_
- --channels <commaListOfChannels>
    Example: #home,#help,#dev
- --config </path/to/config.json>
    Defaults to none -> uses settings passed by other arguments and falls back
    to whatever is in Settings.ini
    Options (channels, host, nicks, pass, port, realname) SHOULD not be used in
    combination with --config option. The idea is to use either config OR the
    mentioned options.
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

.. _`the workdir`:

Work Directory
---------------

This is where QtSssSircBot keeps settings and other files it may generate.
At launch this location is determined depending on OS. Generally it is in
the users "Documents" directory. It's location can only be changed in Cpp code.

Every time QtSssSircBot is launched, it checks for this directory and creates it
and any missing files and directories in it. Most of these are kept in the .qrc
file.

The code-block bellow shows where and how the work directory is determined.

.. literalinclude:: ../../../../AppSettings.cpp
    :caption: hard code another location in AppSettings.cpp initPaths()
    :start-after: markerInitPaths
    :end-before: markerInitPaths
    :emphasize-lines: 7,8

Following directory structure is created:

.. code-block::

    QtSssSircBot+
                + bin
                + config
                + lua +
                      + core
                      + plugins
                      + plugins_available
                      + user

- **bin** populate with scripts that are available for bot to run.
- **config** populate with variations of connection config JSON files and
  other configuration files your scripts in bin may need.
- **lua** holds the Lua files for Lua environment. :ref:`Lua Directory Structure`

