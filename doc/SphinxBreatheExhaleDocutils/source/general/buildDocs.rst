Build Documentation
====================

You may want a local copy of this documentation, or use similar technique to
make your own. Here is how I got it working for me.

Dependencies
-------------
- Doxygen. Your distro probably has this.
- Python for Sphinx, Exhale, Breathe, DocUtils and company.
- LDoc for Lua documentation.

For me it was more or less:

.. code-block:: sh

    # dnf install python-sphinx doxygen doxygen-wizard lua-ldoc

The wizard is optional, I recommend it.

Then as user, installed some python modules:

.. code-block:: sh

    $ python -m pip install --user docutils
    $ pip install breathe --user
    $ pip install exhale --user
    $ pip install groundwork-sphinx-theme --user


Running Sphinx build
-----------------------

Now running:

.. code-block:: sh

    $ QtSssSircBot/doc/SphinxBreatheExhaleDocutils/build.sh

should give you built documentation in
QtSssSircBot/doc/SphinxBreatheExhaleDocutils/build/html

Running Doxygen build
-----------------------

For a better cross-liked version of the C++ code reference:

.. code-block:: sh

    $ QtSssSircBot/doc/Doxygen/build.sh

resulting pages can be found in QtSssSircBot/doc/Doxygen/html

Running LDoc build
--------------------

While we don't have a better way of including Lua documentation into the
documentation built with Sphinx-chain, we include a built version with LDoc
as static html content for Sphinx to pick up.

.. code-block:: sh

    $ QtSssSircBot/doc/LDoc/build.sh

resulting pages can be found in QtSssSircBot/doc/LDoc/html

Running combined build
---------------------------

This script builds with LDoc first. Moves the result to static for Sphinx
and executes the Sphinx-build-chain.

.. code-block:: sh

    $ QtSssSircBot/doc/build.sh

resulting pages can be found in QtSssSircBot/doc/SphinxBreatheExhaleDocutils/build/html

Updating Version Information
------------------------------

There is QtSssSircBot/doc/updateVersionTag.sh that parses project file for
version number and asks git for current branch. It then updates the configuration
files using template files.

Run this after checking out a branch and before building documentation. You can
add the script to build steps in QtCreator, or to git hooks.

