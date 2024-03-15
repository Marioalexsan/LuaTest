scr = {}

local frame = 0

function scr.update()
  frame = frame + 1
  print('Frame' .. frame)
  draw_color(128, 128, 128)
  draw_rect(frame / 10, frame / 10, 30, 30)
  draw_color(0, 255, 0)
  draw_text(20, 20, 12, "Currently on frame " .. frame)
end

return scr