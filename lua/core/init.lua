--[[
  lua/core/init.lua
  main lua entrance - loads the lua files
  used github.com/rxi/lite.git as guide
--]]
require 'core.strict'
local config = require 'core.config'

-- just in case global has not been set
local core = {}

-- init lua state, do not call any IRC-api methods yet
-- just set up and be ready for either core.run to take
-- over or C/Cpp side to call hooks defined in core.events
function core.init()
  print('core.init', EXEDIR)

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
  local bErrorUser = not core.try(require, "user")

  if bErrorPlugins or bErrorUser then
	  core.error('error initializing core')
  end

  core.oNotificationManager:post('core.init.done', nil)

end -- core.init


-- this is meant for applications that run lua on seperate thread in it's own
-- run-loop
function core.run()
  print('core.run')
  -- loop
	core.poll_event()
	--sleep
  -- end loop
end -- core.run


function core.poll_event()
  local aEvent = IRC.poll_event()
  if aEvent then
    core.handleEvent(aEvent)
  end
end -- core.poll_event


function core.poll_events()
  local aEvent
  local bContinue = true
  while bContinue do
	aEvent = IRC.poll_event()
	if nil == aEvent then
	  bContinue = false
	else
	  core.handleEvent(aEvent)
	end
  end
end -- core.poll_events


local function log(icon, icon_color, fmt, ...)
  local text = string.format(fmt, ...)

  local info = debug.getinfo(2, "Sl")
  local at = string.format("%s:%d", info.short_src, info.currentline)
  local item = { text = text, time = os.time(), at = at }

  -- for now let's just print it, later we can append to log file
  print(text)

  return item
end -- log


function core.log(...)
  return log("i", nil, ...) --style.text, ...)
end


function core.log_quiet(...)
  return log(nil, nil, ...)
end


function core.error(...)
  return log("!", nil, ...) --style.accent, ...)
end


function core.try(fn, ...)
  local err
  local ok, res = xpcall(fn, function(msg)
	local item = core.error("%s", msg)
	item.info = debug.traceback(nil, 2):gsub("\t", "")
	err = msg
  end, ...)
  if ok then
	return true, res
  end
  return false, err
end -- core.try


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


function core.temp_filename(ext)
  temp_file_counter = temp_file_counter + 1
  return EXEDIR .. PATHSEP .. temp_file_prefix
	  .. string.format("%06x", temp_file_counter) .. (ext or "")
end -- core.temp_filename


-- called at quit but also when lua env is being re-loaded
-- assume that connections stay active or are dead already (when exiting app)
-- save state if needed using override
function core.abort(iRes)
  -- cleanup
  delete_temp_files()
  -- signal that we are ready for re-load
  IRC.abort(iRes)
end -- core.abort

function core.disconnectAll()
  local lIDs = IRC.connection_ids()
  for _, sConnectionID in ipairs(lIDs) do
    print(sConnectionID)
    IRC.send_quit(sConnectionID, config.CoreQuitMessage or 'goodbye :D')
    IRC.disconnect(sConnectionID)
  end
end -- core.disconnectAll

function core.quit()
  core.disconnectAll()
  core.abort(0)
  IRC.exit(0)
end -- core.quit


function core.load_plugins()
  local no_errors = true
  local files = system.list_dir(EXEDIR .. "/lua/plugins")
  if nil == files then return no_errors end
  for _, filename in ipairs(files) do
	local modname = "plugins." .. filename:gsub(".lua$", "")
	local ok = core.try(require, modname)
	if ok then
	  core.log_quiet("Loaded plugin %q", modname)
	else
	  no_errors = false
	end
  end
  return no_errors
end -- core.load_plugins


function core.cleanNick(sNick)
  return sNick:gsub('^[~&@%+]', '')
end


function core.handleEvent(aEvent)

  local iEvent = #aEvent
  if 3 > iEvent then
	core.log_quiet('invalid event length')
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
	core.log_quiet('OO:unknown interface command: ' .. sInterfaceCommand)
  end

end -- core.handleEvent


-- events are designed to be overridden by plugins
-- alternatively you can use the notification system
core.events = {}

function core.events.abort(sConnectionID, iCode)
  -- TODO: not sure we want to quit on this signal
  -- it is issued mainly by socket errors
  --core.quit()
  core.oNotificationManager:post('core.events.abort', {
    sConnectionID = sConnectionID, iCode = iCode
  })
end
-- private message to channel on which bot is part of was received
function core.events.channelMessage(sConnectionID, sChannel, sFromNick, sMessage)
  core.oTriggerManager:checkTriggers(sConnectionID, sChannel, sFromNick, sMessage)
  core.oNotificationManager:post('core.events.channelMessage', {
    sConnectionID = sConnectionID, sChannel = sChannel,
    sFromNick = sFromNick, sMessage = sMessage
  })
end
-- private message to bot received
function core.events.directMessage(sConnectionID, sNick, sMessage)
  core.oTriggerManager:checkTriggers(sConnectionID, nil, sNick, sMessage)
  core.oNotificationManager:post('core.events.directMessage', {
    sConnectionID = sConnectionID, sFromNick = sNick, sMessage = sMessage
  })
end

function core.events.IRCcommand(sConnectionID, sCommand, lParams)
  core.oNotificationManager:post('core.events.IRCcommand', {
    sConnectionID = sConnectionID, sCommand = sCommand, lParams = lParams
  })
end
-- a user has joined a channel
function core.events.joined(sConnectionID, sNick, sChannel)
  core.oNotificationManager:post('core.events.', {
    sConnectionID = sConnectionID, sNick = sNick, sChannel = sChannel
  })
end
-- bot has logged in
function core.events.loggedIn(sConnectionID, sNick)
  core.oNotificationManager:post('core.events.loggedIn', {
    sConnectionID = sConnectionID, sNick = sNick
  })
end
-- list of nicks in channel received
function core.events.nickList(sConnectionID, sChannel, lNicks)
  core.oNotificationManager:post('core.events.nickList', {
    sConnectionID = sConnectionID, sChannel = sChannel, lNicks = lNicks
  })
end
-- ping received (pong has already been sent back)
function core.events.ping(sConnectionID, sMessage)
  core.oNotificationManager:post('core.events.ping', {
    sConnectionID = sConnectionID, sMessage = sMessage
  })
end
-- a user has quit
function core.events.quit(sConnectionID, sNick, sMessage)
  core.oNotificationManager:post('core.events.quit', {
    sConnectionID = sConnectionID, sNick = sNick, sMessage = sMessage
  })
end
-- socket is connected
function core.events.connected(sConnectionID, sIP)
  core.oNotificationManager:post('core.events.connected', {
    sConnectionID = sConnectionID, sIP
  })
end
-- socket has disconnected
function core.events.disconnected(sConnectionID)
  core.oNotificationManager:post('core.events.disconnected', {
    sConnectionID = sConnectionID
  })
end
-- raw line received
function core.events.rawIn(sConnectionID, sLine)
  core.oNotificationManager:post('core.events.rawIn', {
    sConnectionID = sConnectionID, sLine = sLine
  })
end
-- raw output bot has sent
function core.events.rawOut(sConnectionID, sLine)
  core.oNotificationManager:post('core.events.rawOut', {
    sConnectionID = sConnectionID, sLine = sLine
  })
end


return core

