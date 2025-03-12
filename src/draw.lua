local shapes = {}

function shapes.Rectangle(obj)
   __draw.DrawRectangle(
      obj.Position[1],
      obj.Position[2],
      obj.Size[1],
      obj.Size[2],
      255,
      255,
      255
   )
end

function shapes.Circle()

end

setlocal("draw", function(_instances)
   for i, v in _instances do
      shapes[v.classname](v);
   end
end)
