local signal = {}

local signalmeta = {}
signalmeta.__index = signalmeta

function signal.new()
    local self = {}
    self.connections = {}

    return setmetatable(self, signalmeta)
end

function signalmeta:Connect(f)
    table.insert(self.connections, {func = f})
    local pos = #self.connections
    return function()
        if not pos then return end
        self.connections[pos] = nil
        pos = nil
    end
end

function signalmeta:Once(f)
    table.insert(self.connections, {func = f, once = true})
    local pos = #self.connections
    return function()
        if not pos then return end
        self.connections[pos] = nil
        pos = nil
    end
end

function signalmeta:Fire(...)
    for i, v in pairs(self.connections) do
        v.func(...)
        if v.once then
           self.connections[i] = nil
        end
    end
end

setlocal("Signal", signal)
