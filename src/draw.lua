local shapes = {}

function shapes.Rectangle()

end

function shapes.Circle()

end

setlocal("draw", function(_instances)
   for i, v in _instances do
      shapes[v.classname]();
   end
end)
