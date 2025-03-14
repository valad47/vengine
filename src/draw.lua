local shapes = {}

function shapes.Rectangle(obj)
   obj.OnFrame:Fire(obj)
   if obj.Visible then __draw.DrawRectangle(
      obj.Position[1],
      obj.Position[2],
      obj.Size[1],
      obj.Size[2],
      obj.Color[1],
      obj.Color[2],
      obj.Color[3]
   ) end
end

function shapes.Circle()

end

setlocal("draw", function(_instances)
   for i, v in _instances do
      shapes[v.classname](v);
   end
end)
