Extending
=======================

Add more classes that listen to signals and react on them.

Check out the `lua branch <https://github.com/SwissalpS/QtSssSircBot/tree/lua>`_
for an example of how Lua plugins can be added.

Some portions of the code are already prepared for versions with a GUI.

.. code-block:: cpp

    #ifdef SssS_USE_GUI
        // do GUI code
    #else
        // do non-GUI code
    #endif

