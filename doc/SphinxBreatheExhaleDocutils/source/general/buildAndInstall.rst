Build and Install
==============================

Dependencies
--------------

- Qt5 libraries. QtSssSircBot has been built against Qt versions as old as
  Qt5.8 but newer versions are recomended. Not all Qt-modules are required,
  in fact qtbase and qtnetwork should be sufficient.

- There should be SSL/TLS support built in.

With some luck the packages from your distribution work. Otherwise you need
to build them from source.

Download Source
----------------

You can `download a zip from github <https://github.com/SwissalpS/QtSssSircBot/archive/master.zip>`_
and work from that, or better use command line to clone repository:

.. code-block:: sh

    $ git clone https://github.com/SwissalpS/QtSssSircBot.git

Build QtSssSircBot
-------------------

Open QtSssSircBot/QtSssSircBot.pro with QtCreator, setup build environment and
build. Or use command line to do:

.. code-block:: sh

    $ mkdir build_QtSssSircBot
    $ cd build_QtSssSircBot
    $ path/to/Qt_version_of_choice/bin/qmake path/to/QtSssSircBot.pro
    $ make

Then test:

.. code-block:: sh

    $ ./QtSssSircBot -v

If all went well, you should see the version printed above your prompt.

