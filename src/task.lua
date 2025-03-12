local wait_poll = {}
local jobs = {}
local new_jobs = {}
local jobs_count = 0
local total_jobs = 0

task = {}
    task.CLOSE_WHEN_NO_JOBS = true

function task.step()
    for i, v in pairs(jobs) do
        if coroutine.status(v) == "dead" then
            jobs[i] = nil
            jobs_count = jobs_count - 1
            print("Deleted", v)
        end
    end

    for i, v in pairs(new_jobs) do
        table.insert(jobs, v[1])
        new_jobs[i] = nil
        jobs_count = jobs_count + 1
        total_jobs = total_jobs + 1
        local pass, err = coroutine.resume(table.unpack(v))
        if not pass then
           print("Error in thread ["..tostring(v[1]).."]:\n", err)
        end
    end

    for i, v in pairs(wait_poll) do
        if v <= GetTime() then
            wait_poll[i] = nil
            local pass, err = coroutine.resume(i)
        if not pass then
           print("Error in thread ["..tostring(i).."]:\n", err)
        end
        end
    end
end

function task.spawn(f, ...)
   local thread;
    if type(f) == "function" then
       thread = coroutine.create(f)
    elseif type(f) == "thread" then
       thread = f
    else
       error("Provided invalid value")
    end

    print("Created new", thread);

    table.insert(new_jobs, {thread, ...})
end

function task.wait(time)
    local current = GetTime()
    local thread, main = coroutine.running()

    wait_poll[thread] = if time then time + current else current

    coroutine.yield()
    return (GetTime() - current)
end


function task.loop()
    while true do
        task.step()

        if task.CLOSE_WHEN_NO_JOBS and jobs_count <= 0 then
            return
        end
    end
end

return task
