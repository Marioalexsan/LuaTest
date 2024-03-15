scr = {}

local counter = 0

function scr.update()
  counter = counter + 1
  print('Hello! I was called ' .. counter .. ' times.')
end

return scr