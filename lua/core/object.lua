--[[
  lua/core/object.lua
  base object structure
  useful for some plugins and other mechanics
  taken as is from github.com/rxi/lite.git adding this disclaimer and the
  LDoc module description below and some other LDoc comments.
--]]
--- Base class object structure.
-- Useful for some plugins and other mechanics.
-- Taken as is from github.com/rxi/lite.git adding this disclaimer and this
-- LDoc module description and some other LDoc comments.
-- Usage examples are core.notifications and core.logging.
-- module: core.object

local Object = {}
Object.__index = Object


--- make new instance of Object.
-- Use; ``local o = Object()``
function Object:new()
end


--- extend Object aka sub-class.
-- Use; ``local MyObject = Object:extend()``
function Object:extend()
  local cls = {}
  for k, v in pairs(self) do
	if k:find("__") == 1 then
	  cls[k] = v
	end
  end
  cls.__index = cls
  cls.super = self
  setmetatable(cls, self)
  return cls
end


function Object:implement(...)
  for _, cls in pairs({...}) do
	for k, v in pairs(cls) do
	  if self[k] == nil and type(v) == "function" then
		self[k] = v
	  end
	end
  end
end


function Object:is(T)
  local mt = getmetatable(self)
  while mt do
	if mt == T then
	  return true
	end
	mt = getmetatable(mt)
  end
  return false
end


function Object:__tostring()
  return "Object"
end


function Object:__call(...)
  local obj = setmetatable({}, self)
  obj:new(...)
  return obj
end


return Object

