scr = {}

local counter = 0

function scr.update()
  counter = counter + 1
  print('Hello! I was called ' .. counter .. ' times.')
  customFunc(1.337 + counter * counter)
end

return scr