# Configuration file for the Sphinx documentation builder.
#
# For a full list of options see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'QtSssSircBot'
copyright = '2020, SwissalpS'
author = 'SwissalpS'

# for groundwork theme
# The short X.Y version
version = '0.0'
# The full version, including alpha/beta/rc tags
release = '0.0.1_master'

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = ['breathe','exhale',
    'sphinx.ext.todo',
    'sphinx.ext.githubpages'
]

# Setup the breathe extension
breathe_projects = {
    "QtSssSircBot": "tmp_doxyoutput/xml"
}
breathe_default_project = "QtSssSircBot"

import textwrap

# Setup the exhale extension
exhale_args = {
    # These arguments are required
    "containmentFolder":     "cppApi",
    "rootFileName":          "cppApiRoot.rst",
    "rootFileTitle":         "Cpp API",
    "doxygenStripFromPath":  "../../../",
    # Suggested optional arguments
    "createTreeView":        True,
    # TIP: if using the sphinx-bootstrap-theme, you need
    # "treeViewIsBootstrap": True,
    "exhaleExecutesDoxygen": True,
    #"exhaleDoxygenStdin": "INPUT = ../../QtSssSircBot/"
    "exhaleDoxygenStdin": textwrap.dedent('''
        INPUT = ../../../
        EXCLUDE = ../../../doc
        # Using `=` instead of `+=` overrides
        RECURSIVE = YES
        FULL_PATH_NAMES = YES
        SHOW_FILES = YES
        HIDE_IN_BODY_DOCS = NO
        # this is set to NO if EXTRACT_ALL is set to YES
        WARN_IF_UNDOCUMENTED = YES
        # sometimes we just want all for valid reasons
        EXTRACT_ALL = NO
        # Allow for rst directives and advanced functions e.g. grid tables
        ALIASES += "rst=\\verbatim embed:rst:leading-asterisk"
        ALIASES += "endrst=\\endverbatim"
    ''')
}

# Tell sphinx what the primary language being documented is.
primary_domain = 'cpp'

# Tell sphinx what the pygments highlight language should be.
highlight_language = 'cpp'

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------

# for groundwork: master_doc and pygments_style
master_doc = 'index'
# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'

# also for groundwork: all of these options
html_theme_options = {
    "sidebar_width": '240px',
    "stickysidebar": True,
    "stickysidebarscrollable": True,
    "contribute": True,
    "github_fork": "SwissalpS/QtSssSircBot",
    "github_user": "SwissalpS",
}

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
#html_theme = 'alabaster'
# to install this theme, do:
# $ pip install groundwork-sphinx-theme --user
html_theme = 'groundwork'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

