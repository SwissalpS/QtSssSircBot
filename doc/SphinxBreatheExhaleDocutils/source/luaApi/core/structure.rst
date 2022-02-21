.. _Lua Directory Structure:

Directory Structure
====================

In :ref:`the workdir`
  - **core** main modules, content should be changed only by maintainers
  - **plugins** any file in here is loaded as a Lua module. Populate with
    your selection of plugins or symbolic links to some.
  - **plugins_available** holds default plugins supplied with binary.
    QtSssSircBot does nothing with these files except create them if they
    are missing at launch. They are only used if linked to from plugins
    directory, or loaded manually by another plugin/user module.
  - **user** User specific code. init.lua is loaded after core has prepared
    environment and loaded all plugins. Source maintainers do not change
    anything in here.

Plugins vs User modules
--------------------------
Plugins solve general purpose issues. They can easily be used in other projects
without needing to modify code. They may need configuration via core.config
table. When being loaded, plugins don't care if or when other plugins are
loaded.

User modules contain sensitive information and code that only works for a
specific situation. Example you want control over the order in which other
modules are loaded.

If you are new to Lua scripts, start with user/init.lua. When it works in there,
move the code to user/myModule.lua and add `require 'user.myModule'`
in user/init.lua, as replacement.

When it works as user module you can then work on making it a plugin and submit
a pull request on github to have it added to the collection in plugins_available.

