Running
========

These examples assume you have the compiled binary QtSssSircBot somewhere on
$PATH. If not replace 'QtSssSircBot' with '/path/to/QtSssSircBot'


In Background
--------------

.. code-block:: sh

    $ QtSssSircBot > /dev/null 2>&1 &


With alternative config.json than defined in Settings.ini
----------------------------------------------------------

.. code-block:: sh

    $ QtSssSircBot --config /path/to/testingC.json


Single Connection
------------------

.. code-block:: sh

    $ QtSssSircBot --pass 1239 \
        --host irc.foo.bar \
        --nicks bot,botA,botB,botABC \
        --realname "Samson the Slow" \
        --channels #home,#sweet,#help

