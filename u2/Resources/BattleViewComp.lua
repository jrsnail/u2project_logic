u2.log(Constant.LML_Normal, "===== BattleViewComp.lua start =====")


-- ------------------- init start -------------------

-- create c battle
u2.createBattle()


function getViewCompName()
	return "BattleViewComp"
end

function getCsbName()
	return "bgani_01.png"
end

-- register notifications which interested by script
u2.registerLuaScriptWithInterestedNotification({
"OT_C2LTask_ViewComp_Created"
})

function pushInitL2CTask()
	-- set view component ui name
	local l2cTask = u2.createL2CTask(Constant.OT_SetViewCompUiNameLuaTask)
	u2.setLuaTaskField(l2cTask, "UiName", getCsbName())
	u2.setLuaTaskField(l2cTask, "ViewCompName", getViewCompName())
	u2.pushL2CTask(l2cTask)
end

pushInitL2CTask()
-- ------------------- init end -------------------




function isTaskOnThisScript(c2lTask)
	local bIsThis = u2.getLuaTaskField(c2lTask, "ViewCompName") == getViewCompName() 
		and u2.getLuaTaskField(c2lTask, "ViewCompCsb") == getCsbName()
	return bIsThis
end

-- process view component created task
function doViewCompCreated(c2lTask)
	-- Is clicked task occured on this view component?
	if not isTaskOnThisScript(c2lTask) then
		return
	end
	
	u2.log(Constant.LML_Normal, "===== BattleViewComp.lua doViewCompCreated =====")
end


while true do
	-- get the front task
	local c2lTask = u2.frontC2LTask()
	if c2lTask == nil then
		coroutine.yield()
	else
		local taskType = u2.getLuaTaskType(c2lTask)
		
		if "OT_C2LTask_ViewCompCreated" == taskType then
			doViewCompCreated(c2lTask)
		else
		end
		
		-- destroy this task
		u2.destroyC2LTask(c2lTask)
	end
end


u2.log(Constant.LML_Normal, "===== BattleViewComp.lua end =====")
