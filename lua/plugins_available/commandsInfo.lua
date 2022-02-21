--[[
	lua/plugins/commandsInfo.lua
	plugin that provides info command
--]]
--- plugin that provides info command.
-- Demonstrates how notifications can be used to run code only after everything
-- is loaded. Also shows how triggers can be used.
-- module: plugins.commandsInfo

---
-- Showing off Colon mode.
-- If you hate @ tags, you can use colons. However, you need to specify colon
-- mode explicitly -C or --colon, or `colon=true` in the config.ld. Be careful
-- not to use a colon followed by a space for any other purpose!
--
-- So the incantation in this case is `ldoc -C colon.lua`.
-- string: info
-- [[

---
-- short. longer text
function p() end

--- first useless function.
-- Optional type specifiers are allowed in this format.
-- As an extension, '?T' is short for '?nil|T'.
-- Note how these types are rendered!
-- string: name
-- int: age
-- ?person3: options
-- treturn: ?table|string
function one (name,age,options)
end

--- implicit table can always use colon notation.
person2 = {
    id=true, -- string: official ID number
    sex=true, -- string: one of 'M', 'F' or 'N'
    spouse=true, -- ?person3: wife or husband
}

--- explicit table in colon format.
-- Note how '!' lets you use a type name directly.
-- string: surname
-- string: birthdate
-- !person2: options
-- table: person3


---
-- for real, document this
local sendDM = irc.send_direct_message


---
-- Given an array where all elements are strings or numbers, returns
-- `table[i]..sep..table[i+1] ... sep..table[j]`. The default value for
-- `sep` is the empty string, the default for `i` is 1, and the default for
-- `j` is the length of the table. If `i` is greater than `j`, returns the
-- empty string.
function shoe.concat(table , sep , i , j) end

---
-- Inserts element `value` at position `pos` in `table`, shifting up
-- other elements to open space, if necessary. The default value for `pos` is
-- `n+1`, where `n` is the length of the table (see §2.5.5), so that a call
-- `table.insert(t,x)` inserts `x` at the end of table `t`.
function table.insert(table, pos, value) end

---
-- thisoeu. osuth o ua seuahsouatho
-- atuhoesostuh
local function info(tP)
	local sOut = 'Qt-SwissalpS-IRC-bot+lua ' .. VERSION
		.. ' https://SwissalpS.github.io/QtSssSircBot'
	sendDM(tP.sConnectionID, tP.sNick, sOut)
end -- info

--- sumume uoth o.
-- thath ethuas u ath husat hthueathuthesuthaet
-- tuatheut hetuhtohut hotuto.
local function init()
	local oTrigger = core.Trigger({
		lTriggerRxs = { '^!info$' },
		fCallback = info,
		bIncludeInHelp = true,
		sDescription = '!info\n  lists some information about the bot'
	})
	oTrigger:makeDMonly()

	core.oTriggerManager:addTrigger(oTrigger)
end -- init

-- not important for this plugin
core.oNotificationManager:subscribe('core.init.done', init)
--]]
return true

