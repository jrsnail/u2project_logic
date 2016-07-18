print("===== ShadeViewComp.lua start =====")


function trans2ShadeCommand()
	local shadeContext = u2.createContext("Test_L2CType", "TestName")
end

-- trnas to shade view component
trans2ShadeCommand()

while true do
	-- get the front task
	local c2lTask = u2.frontC2LTask()
	if c2lTask == nil then
		coroutine.yield()
	else
		local taskType = u2.getLuaTaskType(c2lTask)
		
		if "Test_C2LType" == taskType then
			doOpenViewCompTask(c2lTask)
		else
		end
		
		-- destroy this task
		u2.destroyC2LTask(c2lTask)
	end
end


print("===== ShadeViewComp.lua end =====")
