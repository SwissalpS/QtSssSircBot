-- HTML <title>
title = 'QtSssSircBot LDoc-umentation'
project = 'QtSssSircBot'
description = 'Qt SwissalpS IRC Bot'
full_description = [[
This documentation is about the Lua environment built into QtSssSircBot on
the lua branch.

For more information on QtSssSircBot, you may also want to read the documentation
generated with Sphinx.
]]

-- setting this to true, includes local declarations
all = true

-- exclude files must exist or not be listed :/
file = { '../../lua', exclude = { '../../lua/.lite_workspace.lua' } }

-- use colon syntax: intstead of '@return something' use 'treturn: something'
-- or instead of '@param x int holds position' do 'int: x holds position'
-- and best of all: optional parameters: '?int: x holds position'
colon = true

-- markdown or plain. Can be many others if installed, like discount.
-- Multiple formats can be specified: 'markdown|discount|foo|bar'
format = 'markdown'

-- set to true to skip first comment block in each file
boilerplate = true

-- uncomment to not show links to pretty source files
prettify_files = 'show'

-- default is false
sort = true

-- these two can also be set per module: @set no_summary=true
no_summary = false
no_return_or_parms = false

