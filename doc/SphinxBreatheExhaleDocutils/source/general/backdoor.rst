Backdoor Interface
===================

Let's assume QtSssSircBot is running and the backdoor path is stored in ``$BDPATH``
To terminate QtSssSircBot we can now do:

.. code-block:: sh

    echo "- e -" > $BDPATH;

Basic Command Format
--------------------
Each line in backdoor file is treated as a command. Lines that do not have at
least 3 parts are ignored.

``<sConnectionID> <ubCommand> <param0>[ ... <paramN>]``

Implemented Commands
---------------------
- Channel Message = 'C'::

  <sConnectionID> C <sChannel> <sMessage>

  send sMessage on sChannel of connection sConnectionID
- Direct Message = 'D'::

  <sConnectionID> D <sNick> <sMessage>

  send sMessage to sNick of connection sConnectionID
- Exit = 'e'::

  - e -

  Terminate QtSssSircBot
- Join = 'J'::

  <sConnectionID> J <sChannel>

  join sChannel on connection sConnectionID
- NickList = 'N'::

  <sConnectionID> N <sNewNick>

  request to change nick to sNewNick on connection sConnectionID
- Part = '-'::

  <sConnectionID> - <sChannel(s)>[ <sMessage>]

  leave channel(s) of connection sConnectionID with optional sMessage
- Quit = 'Q'::

  <sConnectionID> Q <sMessage>

  leave network of sConnectionID with sMessage
- (Re)connected = 'c'::

  <sConnectionID> c -

  (re)connect connection of sConnectionID
- Disconnect = 'd'::

  <sConnectionID> d -

  disconnect connection of sConnectionID
- Reload Connections = 'r'::

  - r -

  Reload all connections from config file. Disconnects any existing connections
  and rebuilds them according to config. This is same as re-launching
  QtSssSircBot but with benifit of keeping other parts of the application
  running.
- Raw Out = '>' TODO: implement this functionality::

  <sConnectionID> > <sRawIRCline>

  send sRawIRCline (adding termination crlf for you) to connection sConnectionID

These commands are a subset of commands found in
`CommandEventCodes.h <../cppApi/namespace_SwissalpS__QtSssSircBot__CommandEventCodes.html>`_

