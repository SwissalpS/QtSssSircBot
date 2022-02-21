--[[
  lua/core/init.lua
  main lua entrance - loads the lua files
  used github.com/rxi/lite.git as guide
--]]
--- Main Lua environment
-- module: core.init
--
require 'core.strict'
local config = require 'core.config'

-- just in case global has not been set
local core = {}

-- keep hash table of { sID = { f = fCallback, m = mData } }
local hDelayedCallbacks = { c = { f = function() end, m = nil } }

--- Maintenance tasks that get done at intervals.
-- Calls itself again after given interval has passed.
-- int: iInterval should be greater than 0
local function repeater(iInterval)
  -- TriggerManager keeps track of requests that users make
  -- need to clean that history from time to time
  local iLast = os.time() - (config.CoreTriggerPurgeOlder or (2 * 3600))
  core.oTriggerManager:purgeRequestsBefore(iLast)

  core.call_later(iInterval, repeater, iInterval)
end

--- Initialize Lua state. Do not call any IRC-api methods yet
-- just set up and be ready for either core.run to take
-- over or C/Cpp side to call hooks defined in core.events
function core.init()
  --print('core.init', EXEDIR)

  -- load trigger code
  local tT = require 'core.triggers'
  core.Trigger = tT[1]
  core.TriggerManager = tT[2]
  -- create a shared TriggerManager-instance
  -- which also tracks how often a user is issueing commands
  core.oTriggerManager = core.TriggerManager()

  -- load shared Notification Manager
  core.oNotificationManager = require 'core.notifications'

  local bErrorPlugins = not core.load_plugins()
  local bErrorUser = not core.try(require, 'user')

  if bErrorPlugins or bErrorUser then
	  core.error('error initializing core')
  end

  core.oNotificationManager:post('core.init.done', nil)

  -- start maintenance cycle
  repeater(config.CoreRepeaterInterval)

end -- core.init


--- Lua based run-loop. This is meant for applications that run Lua on
-- separate thread in it's own run-loop.
function core.run()
  --print('core.run')
  -- loop
	core.poll_event()
	--sleep
  -- end loop
end -- core.run


--- ask for delayed callback.
-- int: iInterval interval in milliseconds -> 1000 ms = 1 second.
-- func: fCallback the function to execute when interval has passed
-- ?bool|int|nil|number|string|table: mData param to pass to fCallback
-- when executing.
function core.call_later(iInterval, fCallback, mData)
  if 0 > iInterval then
    core.error('interval must be 0 or greater')
    return
  end
  if 'function' ~= type(fCallback) then
    core.error('callback must be a function')
    return
  end
  -- make sure we use a unique id
  local sID = 'c'
  while nil ~= hDelayedCallbacks[sID] do
    sID = 'c' .. tostring(system.get_rand())
  end
  hDelayedCallbacks[sID] = { f = fCallback, m = mData }
  system.delayed_callback(sID, iInterval)
end -- core.call_later


--- Executes a delayed callback function.
-- Called by LuaController when a timed callback times-out.
-- There should not be any need to call this manually.
function core.call_delayed_callback(sID)
  local hCallback = hDelayedCallbacks[sID]
  if nil == hCallback then
    core.error('callback does not exist')
    return
  end
  core.try(hCallback.f, hCallback.m)
  hDelayedCallbacks[sID] = nil
end -- core.call_delayed_callback


--- Check for an IRC event and handle it.
-- Called by LuaController when an event is added to event pool.
-- There should not be any need to call this manually, but there is also no
-- harm done in doing so.
function core.poll_event()
  local aEvent = irc.poll_event()
  if aEvent then
    core.handleEvent(aEvent)
  end
end -- core.poll_event


--- Alternative to core.poll_event() should that be insuficient.
-- This one would check for all events and handle all. As long as single
-- method works, we should use that.
-- There should not be any need to call this manually, but there is also no
-- harm done in doing so.
function core.poll_events()
  local aEvent
  local bContinue = true
  while bContinue do
	  aEvent = irc.poll_event()
	  if nil == aEvent then
	    bContinue = false
	  else
	    core.handleEvent(aEvent)
	  end
  end
end -- core.poll_events


local function log(sIcon, icon_color, sFormat, ...)

  local sText = string.format(sFormat, ...)

  local tInfo = debug.getinfo(2, "Sl")
  local sAt = string.format("%s:%d", tInfo.short_src, tInfo.currentline)
  local hItem = { sText = sText, iTime = os.time(), sAt = sAt, sIcon = sIcon }

  -- allow plugins and user modules to handle logging
  core.oNotificationManager:post('core.log', hItem)

  print(sText)

  return hItem

end -- log


--- log a message at 'info' level.
-- string: sFormat format or output string
-- ?mixed: optional arguments for string.format(sFormat, ...)
-- see: core.log_quiet, core.error
function core.log(...)
  return log("i", nil, ...) --style.text, ...)
end


--- log a message at 'quiet' level
-- string: sFormat format or output string
-- ?mixed: optional arguments for string.format(sFormat, ...)
-- see: core.log, core.error
function core.log_quiet(...)
  return log(nil, nil, ...)
end


--- log a message at 'error' level
-- string: sFormat format or output string
-- ?mixed: optional arguments for string.format(sFormat, ...)
-- see: core.log_quiet, core.log
function core.error(...)
  return log("!", nil, ...) --style.accent, ...)
end


--- wrap a call in xpcall() for safe execution with debug info.
-- !function: fn what to execute
-- ?mixed: optional arguments to be passed to fn
-- returns: true and result on success. false and error message on failure.
-- On failure the message is sent to logging system.
function core.try(fn, ...)
  local err
  local ok, res = xpcall(fn, function(msg)
	  local item = core.error("%s", msg)
	  --item.info = debug.traceback(nil, 2):gsub("\t", "")
	  err = msg
    end, ...)
  if ok then
	  return true, res
  end
    return false, err
end -- core.try


--- error handler.
-- There should be no need for manually calling this.
-- Maybe you want to override or modify it though.
-- Writes error message and debug traceback int lua_error.txt in the work
-- directory.
function core.on_error(err)
  -- write error to file
  local fp = io.open(EXEDIR .. "/lua_error.txt", "wb")
  if nil ~= fp then
  	fp:write("Error: " .. tostring(err) .. "\n")
  	fp:write(debug.traceback(nil, 4))
  	fp:close()
  end
end -- core.on_error


system.init_rand()
local temp_uid = system.get_rand() % 0xffffffff
local temp_file_prefix = string.format("._temp_%08x", temp_uid)
local temp_file_counter = 0


local function delete_temp_files()
  for _, filename in ipairs(system.list_dir(EXEDIR)) do
  	if filename:find(temp_file_prefix, 1, true) == 1 then
  	  os.remove(EXEDIR .. PATHSEP .. filename)
  	end
  end
end -- delete_temp_files


--- get path to a unique temporary file.
-- ?string: ext extension to use: e.g. '.tmp'
function core.temp_filename(ext)
  temp_file_counter = temp_file_counter + 1
  return EXEDIR .. PATHSEP .. temp_file_prefix
	  .. string.format("%06x", temp_file_counter) .. (ext or "")
end -- core.temp_filename


--- shut down Lua environment
-- called at quit but also when Lua environment is being re-loaded
-- assume that connections stay active or are dead already (when exiting app)
-- save state if needed using override or notification
-- int: iNumber
--              0 < iNumber means reload Lua in iNumber milliseconds.
--              0 >= iNumber means shutdown Lua session.
function core.abort(iNumber)
  -- cleanup
  core.oNotificationManager:post('core.abort', iNumber)
  delete_temp_files()
  -- signal that we are ready for re-load
  irc.abort(iNumber)
end -- core.abort

--- disconnect all connections.
-- Will attempt graceful disconnect with QUIT message on each connection.
function core.disconnectAll()
  local lIDs = irc.connection_ids()
  for _, sConnectionID in ipairs(lIDs) do
    core.log('Disconnecting connection: ' .. sConnectionID)
    irc.send_quit(sConnectionID, config.CoreQuitMessage or 'goodbye :D')
    irc.disconnect(sConnectionID)
  end
end -- core.disconnectAll

--- terminate Lua environment and quit the application.
-- Attempts graceful disconnect and state saving exit.
function core.quit()
  core.disconnectAll()
  core.abort(0)
  core.oNotificationManager:post('core.quit', 0)
  irc.exit(0)
end -- core.quit


--- load active plugins.
-- This is called by core.run() there should be no need to manually called.
-- Results can be disapointing when called manually.
function core.load_plugins()
  local bNoErrors = true
  local lFiles = system.list_dir(EXEDIR .. '/lua/plugins')
  if nil == lFiles then return bNoErrors end

  local sModname, bOK
  for _, sFilename in ipairs(lFiles) do
	  sModname = 'plugins.' .. sFilename:gsub('.lua$', '')
	  bOK = core.try(require, sModname)
	  if bOK then
	    core.log_quiet('Loaded plugin %q', sModname)
	  else
	    bNoErrors = false
	  end
  end

  return bNoErrors
end -- core.load_plugins


--- clean given nick.
-- Removes any op-symbol from nick
-- string: sNick nick to clean
-- treturn: string cleaned nick
function core.cleanNick(sNick)
  return sNick:gsub('^[~&@%+]', '')
end


--- handle IRC event.
-- can also be used to inject events for simulations and other reasons.
-- !list: aEvent list of event strings.
-- <sConnectionID> <sInterfaceCommand> <sParam0>[ ...<sParamN>]
function core.handleEvent(aEvent)

  local iEvent = #aEvent
  if 3 > iEvent then
	  core.error('invalid event length')
  	return
  end

  local sID = aEvent[1]

  local sInterfaceCommand = aEvent[2]
  local sParam0 = aEvent[3]
  local lParams = {}
  local i

  if 'a' == sInterfaceCommand then
	  core.events.abort(sID, tonumber(sParam0))
  elseif 'C' == sInterfaceCommand then
  	if 5 <= iEvent then
  	  core.events.channelMessage(sID, sParam0, aEvent[4], aEvent[5])
  	end
  elseif 'D' == sInterfaceCommand then
  	if 4 <= iEvent then
  	  core.events.directMessage(sID, sParam0, aEvent[4])
  	end
  elseif 'I' == sInterfaceCommand then
  	for i = 4, iEvent, 1 do
  	  table.insert(lParams, aEvent[i])
  	end
  	core.events.IRCcommand(sID, sParam0, lParams)
  elseif 'J' == sInterfaceCommand then
  	if 4 <= iEvent then
  	  core.events.joined(sID, sParam0, aEvent[4])
  	end
  elseif 'L' == sInterfaceCommand then
	  core.events.loggedIn(sID, sParam0)
  elseif 'N' == sInterfaceCommand then
  	for i = 4, iEvent, 1 do
  	  table.insert(lParams, aEvent[i])
  	end
  	core.events.nickList(sID, sParam0, lParams)
  elseif 'P' == sInterfaceCommand then
	  core.events.ping(sID, sParam0)
  elseif 'Q' == sInterfaceCommand then
  	if 4 <= iEvent then
  	  core.events.quit(sID, sParam0, aEvent[4])
  	end
  elseif 'c' == sInterfaceCommand then
	  core.events.connected(sID, sParam0)
  elseif 'd' == sInterfaceCommand then
	  core.events.disconnected(sID)
  elseif '<' == sInterfaceCommand then
	  core.events.rawIn(sID, sParam0)
  elseif '>' == sInterfaceCommand then
	  core.events.rawOut(sID, sParam0)
  else
	  core.log('OO:unknown interface command: ' .. sInterfaceCommand)
  end

end -- core.handleEvent


--- core.events.
-- Events are designed to be overridden by plugins.
-- Alternatively you can use the notification system, as each of the event
-- functions post a notification with all the information.
core.events = {}

--- abort.
-- Posts notification '**core.events.abort**', { sConnectionID, iCode }.
-- This signal is issued mainly by socket errors.
--
-- Not to be confused with @{core.abort}(iNumber) (although they may get related down
-- the line, since command file interface currently causes immediate shutdown
-- of Lua environment)
-- string: sConnectionID
-- int: iCode
function core.events.abort(sConnectionID, iCode)
  core.oNotificationManager:post('core.events.abort', {
    sConnectionID = sConnectionID, iCode = iCode
  })
end
--- private message to channel on which bot is part of was received.
-- Posts notification '**core.events.channelMessage**', { sConnectionID, sChannel,
-- sFromNick, sMessage }.
-- string: sConnectionID
-- string: sChannel
-- string: sFromNick
-- string: sMessage
function core.events.channelMessage(sConnectionID, sChannel, sFromNick, sMessage)
  core.oTriggerManager:checkTriggers(sConnectionID, sChannel, sFromNick, sMessage)
  core.oNotificationManager:post('core.events.channelMessage', {
    sConnectionID = sConnectionID, sChannel = sChannel,
    sFromNick = sFromNick, sMessage = sMessage
  })
end
--- private message to bot received.
-- Posts notification 'core.events.directMessage', { sConnectionID, sFromNick,
-- sMessage }.
-- string: sConnectionID
-- string: sNick
-- string: sMessage
function core.events.directMessage(sConnectionID, sNick, sMessage)
  core.oTriggerManager:checkTriggers(sConnectionID, nil, sNick, sMessage)
  core.oNotificationManager:post('core.events.directMessage', {
    sConnectionID = sConnectionID, sFromNick = sNick, sMessage = sMessage
  })
end
--- an IRC command event has occured.
-- Posts notification 'core.events.IRCcommand', { sConnectionID, sCommand,
-- lParams }.
-- string: sConnectionID
-- string: sCommand
-- !list: lParams
function core.events.IRCcommand(sConnectionID, sCommand, lParams)
  core.oNotificationManager:post('core.events.IRCcommand', {
    sConnectionID = sConnectionID, sCommand = sCommand, lParams = lParams
  })
end
--- a user has joined a channel.
-- Posts notification 'core.events.joined', { sConnectionID, sNick, sChannel }.
-- string: sConnectionID
-- string: sNick
-- string: sChannel
function core.events.joined(sConnectionID, sNick, sChannel)
  core.oNotificationManager:post('core.events.joined', {
    sConnectionID = sConnectionID, sNick = sNick, sChannel = sChannel
  })
end
--- bot has logged in.
-- Posts notification 'core.events.loggedIn', { sConnectionID, sNick }.
-- string: sConnectionID
-- string: sNick the nick bot is using
function core.events.loggedIn(sConnectionID, sNick)
  core.oNotificationManager:post('core.events.loggedIn', {
    sConnectionID = sConnectionID, sNick = sNick
  })
end
--- list of nicks in channel received.
-- Posts notification 'core.events.nickList', { sConnectionID, sChannel, lNicks }.
-- string: sConnectionID
-- string: sChannel
-- !list: lNicks
function core.events.nickList(sConnectionID, sChannel, lNicks)
  core.oNotificationManager:post('core.events.nickList', {
    sConnectionID = sConnectionID, sChannel = sChannel, lNicks = lNicks
  })
end
--- ping received (pong has already been sent back).
-- Posts notification 'core.events.ping', { sConnectionID, sMessage }.
-- string: sConnectionID
-- string: sMessage
function core.events.ping(sConnectionID, sMessage)
  core.oNotificationManager:post('core.events.ping', {
    sConnectionID = sConnectionID, sMessage = sMessage
  })
end
--- a user has quit.
-- Posts notification 'core.events.quit', { sConnectionID, sNick, sMessage }.
-- string: sConnectionID
-- string: sNick
-- string: sMessage
function core.events.quit(sConnectionID, sNick, sMessage)
  core.oNotificationManager:post('core.events.quit', {
    sConnectionID = sConnectionID, sNick = sNick, sMessage = sMessage
  })
end
--- socket is connected.
-- Posts notification 'core.events.connected', { sConnectionID, sIP }.
-- string: sConnectionID
-- string: sIP
function core.events.connected(sConnectionID, sIP)
  core.oNotificationManager:post('core.events.connected', {
    sConnectionID = sConnectionID, sIP = sIP
  })
end
--- socket has disconnected.
-- Posts notification 'core.events.disconnected', { sConnectionID }.
-- string: sConnectionID
function core.events.disconnected(sConnectionID)
  core.oNotificationManager:post('core.events.disconnected', {
    sConnectionID = sConnectionID
  })
end
--- raw line received.
-- Posts notification 'core.events.rawIn', { sConnectionID, sLine }.
-- string: sConnectionID
-- string: sLine
function core.events.rawIn(sConnectionID, sLine)
  core.oNotificationManager:post('core.events.rawIn', {
    sConnectionID = sConnectionID, sLine = sLine
  })
end
--- raw output bot has sent.
-- Posts notification 'core.events.rawOut', { sConnectionID, sLine }.
-- string: sConnectionID
-- string: sLine
function core.events.rawOut(sConnectionID, sLine)
  core.oNotificationManager:post('core.events.rawOut', {
    sConnectionID = sConnectionID, sLine = sLine
  })
end


return core

