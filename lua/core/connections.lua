--[[
  lua/core/connections.lua
  helper methods to create new connections
--]]
local json = require 'core.json'

local connections = {}

function connections.dummyConnectionTable()

  return {
    sConnectionID = 'unset',
    sIRCremoteHost = '0.0.0.0',
    sIRCremoteNicks = { 'QtSssSircBot' },
    uiIRCremotePort = 6697,
    sIRCremoteRealname = 'QtSwissalpSbot',
    sIRCremotePassword = '',
    sIRCremoteChannels = {},
    sIRCrawPostLoginLines = {},
    sIRCrawPreLoginLines = {}
  }

end -- connections.dummyConnectionTable

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

