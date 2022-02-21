Lua Core Initialization
=====================================

`LuaController <../../cppApi/classSwissalpS_1_1QtSssSircBot_1_1LuaController.html>`_
initializes Lua environment. Wrapping it in an `xpcall() <../../cppApi/classSwissalpS_1_1QtSssSircBot_1_1LuaController.html#classSwissalpS_1_1QtSssSircBot_1_1LuaController_1a095666ec897a326e9f9a20e13cee8fb3>`_.
See :ref:`errorHandling`


.. toctree::
   :maxdepth: 2
   :caption: Contents:

   structure
   errorHandling

.. _`errorHandling`:

Error Handling
=================

On syntax or execution errors, the error message is printed to standard out
including debug traceback. Also core.on_error() is called, if hasn't been
disabled.

`core.on_error() <../../_static/LDoc/modules/core.init.html#core.on_error>`_
writes error message and debug traceback to lua_error.txt in :ref:`the workdir`.

In your Lua code, it is advisable to wrap parts, that may cause errors, with
`core.try(fn, ...) <../../_static/LDoc/modules/core.init.html#core.try>`_

