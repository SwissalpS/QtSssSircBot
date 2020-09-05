--[[
  lua/core/notifications.lua
  infrastructure to manage very simple notifications
--]]
--- Class to manage very simple notifications.
-- Use; ```
--   local oM = require 'core.notifications'
--   oM:post('xyz')
-- ```
-- returns a reference to the shared Manager instance.
-- module: core.notifications
--
local Object = require 'core.object'

--- Class to manage notifications.
local Manager = Object:extend()

--- Create Manager instance.
-- 'protected function'
-- Use; local oM = Manager() oM:post('xyz')
function Manager:new()
	self:reset()
end -- Manager:new


--- Reset Manager.
-- 'protected function'
-- You shouldn't need to call this.
function Manager:reset()
	self.tDB = {}
end -- Manager:reset


--- throw a notification.
-- 'public function'
-- string: sNotification the identifier
-- ?nil|boolean|int|number|string|table: mData what to deliver to subscribers
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


--- 'listen' for notification.
-- 'public function'
-- string: sNotification the identifier that triggers this subscribtion
-- !function: fCallback the function to call when a matching notification is posted.
--   It should expect to be called with one parameter. The type and format of
--   this parameter depends on the notification. Some don't use any at all.
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

