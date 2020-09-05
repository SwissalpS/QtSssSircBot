--[[
  lua/core/connections.lua
  helper methods to create new connections
--]]
---
-- helper methods to create new connections.
-- module: core.connections

local json = require 'core.json'

local connections = {}

---
-- get a dummy table with all available fields set.
-- You need to change values for it to be of any use.
function connections.dummyConnectionTable()

  return {
    sConnectionID = 'unset',               -- mandatory unique alphanumeric
    sIRCremoteHost = '0.0.0.0',            -- mandatory hostname or IP
    sIRCremoteNicks = { 'QtSssSircBot' },  -- minimum one
    uiIRCremotePort = 6697,                -- mandatory default to 6697
    sIRCremoteRealname = 'QtSwissalpSbot', -- optional, good to set
    sIRCremotePassword = '',               -- optional depending on network etc.
    sIRCremoteChannels = {},               -- optional auto-join these channels
    sIRCrawPostLoginLines = {},            -- optional send these lines after login
    sIRCrawPreLoginLines = {}              -- optional send these lines before login
  }

end -- connections.dummyConnectionTable

---
-- add a new connection and connect to it.
-- Calls AppController::addConnection() via IRC API IRC.add_connection()
-- !table: tConfig configuration table for the connection.
-- treturn: ?nil|true
function connections.addConnection(tConfig)

  local lConnectionIDs = IRC.connection_ids()

  for _, sConnectionID in ipairs(lConnectionIDs) do
    if sConnectionID == tConfig.sConnectionID then
      core.error('attempting to add a connection with existing connection ID')
      return nil
    end
  end

  local bOK, mRes = core.try(json.encode, tConfig)
  if not bOK then
    core.error('error encoding json: ' .. mRes)
    return nil
  end

  local bRes, sErr = IRC.add_connection(mRes)
  if bRes then return true end

  core.error('json error when attempting to add new connection: ' .. sErr)
  return nil

end -- connections.addConnection

return connections

