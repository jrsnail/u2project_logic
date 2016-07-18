u2.log(Constant.LML_Normal, "===== main.lua start =====")


require "Trans2LogoCmd"


trans2LogoCommand()


-- task loop
while true do
	-- get the front task
	local c2lTask = u2.frontC2LTask()
	if c2lTask == nil then
		coroutine.yield()
	else
		local bExitLua = false
		local taskType = u2.getLuaTaskType(c2lTask)
		
		if "OT_C2LTask_ViewComp_Created" == taskType then
		else
		end
		
		-- destroy this task
		u2.destroyC2LTask(c2lTask)
		
		-- exit lua
		if bExitLua then
			break
		end
	end
end


u2.log(Constant.LML_Normal, "===== main.lua end =====")
