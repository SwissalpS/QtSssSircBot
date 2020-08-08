--[[
  lua/core/notifications.lua
  infrastructure to manage very simple notifications
--]]
local Object = require 'core.object'

local Manager = Object:extend()

function Manager:new()
	self:reset()
end -- Manager:new


function Manager:reset()
	self.tDB = {}
end -- Manager:reset


-- throw a notification
function Manager:post(sNotification, mData)
  for sKey, lCallbacks in pairs(self.tDB) do
    if sKey == sNotification then
      for _, fCallback in ipairs(lCallbacks) do
        core.try(fCallback, mData)
      end
      return
    end
  end
end -- Manager:post


-- 'listen' for notification
function Manager:subscribe(sNotification, fCallback)
  if 'string' ~= type(sNotification) then
    core.error('Notification name needs to be a string')
    return false
  end
  if 0 == #sNotification then
    core.error('Notification name may not be empty')
    return false
  end
  if 'function' ~= type(fCallback) then
    core.error('callback must to be a function')
    return false
  end

  if nil == self.tDB[sNotification] then
    self.tDB[sNotification] = { fCallback }
  else
    table.insert(self.tDB[sNotification], fCallback)
  end

  return true

end -- Manager:subscribe

local oSharedManager = Manager()

return oSharedManager

