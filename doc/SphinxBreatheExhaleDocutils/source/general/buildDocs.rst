Build Documentation
====================

You may want a local copy of this documentation, or use similar technique to
make your own. Here is how I got it working for me.

Dependencies
-------------
- Doxygen. Your distro probably has this.
- Python for Sphinx, Exhale, Breathe, DocUtils and company.

For me it was more or less

.. code-block:: sh

    # dnf install python-sphinx doxygen doxygen-wizard

The wizard is optional, I recommend it.

Then as user, installed some python modules

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

    $ cd QtSssSircBot/doc/Doxygen
    $ doxygen

resulting pages can be found in QtSssSircBot/doc/Doxygen/html

