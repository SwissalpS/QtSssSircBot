# QtSssSircBot
A base 'template' project for IRC bots.

The base application manages multiple secure connections to IRC networks.

[Most information will be here: SwissalpS.github.io/QtSssSircBot/](https://SwissalpS.github.io/QtSssSircBot/index.html)

The lua branch offers a Lua environment. The application run loop is managed by Qt so there are events and delayed callback infrastructure.

## Dependencies
- [Qt-libraries](https://www.qt.io/download) download with your package manager or build from source. Oldest version that was built against was Qt5.8
- [OpenSSL](https://www.openssl.org) the one provided with your distro is probably fine, important part is that Qt-libs support it.

## Credits
QtSssSircBot was inspired by many projects such as
- [minetest](https://github.com/minetest/minetest)
- [rxi/lite](https://github.com/rxi/lite) QtSssSircBot's Lua environment is based on the one used here.
- [civitweb](https://github.com/civetweb/civetweb)
- [Lua](https://www.lua.org) scripting language built into QtSssSircBot on lua branch (Luav5.2)
- [qtirc](https://github.com/jacob3141/qtirc) used some files from this project for parsing incomming messages.


## Licence
GPL-3.0 License
Basically: keep free/libre and open
