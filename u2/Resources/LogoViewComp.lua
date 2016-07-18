u2.log(Constant.LML_Normal, "===== LogoViewComp.lua start =====")

require "Trans2InfoCmd"


-- ------------------- init start -------------------

function getViewCompName()
	return "LogoViewComponent"
end

function getCsbName()
	return "cmlogo1.png"
end

-- register notifications which interested by script
u2.registerLuaScriptWithInterestedNotification({
"NTF_Predefined_ViewComp_Created", 
})

function pushInitL2CTask()
	-- do preload
	local l2cTask = u2.createL2CTask(Constant.OT_PreloadLuaTask)
	u2.pushL2CTask(l2cTask)
	
	-- set view component ui name
	local l2cTask = u2.createL2CTask(Constant.OT_SetViewCompUiNameLuaTask)
	u2.setLuaTaskField(l2cTask, "UiName", getCsbName())
	u2.setLuaTaskField(l2cTask, "ViewCompName", getViewCompName())
	u2.pushL2CTask(l2cTask)
	
	-- set view component enter action
	local l2cTask = u2.createL2CTask(Constant.OT_SetViewCompEnterActinLuaTask)
	u2.setLuaTaskField(l2cTask, "ViewCompName", getViewCompName())
	u2.setLuaTaskField(l2cTask, "EnterAction", tostring(1))
	u2.pushL2CTask(l2cTask)
	
	-- set view component center of screen
	local l2cTask = u2.createL2CTask(Constant.OT_CenterViewCompLuaTask)
	u2.setLuaTaskField(l2cTask, "ViewCompName", getViewCompName())
	u2.pushL2CTask(l2cTask)
end

pushInitL2CTask()
-- ------------------- init end -------------------



local bPreloadEnd = false
local bViewCompCreated = false

function isTaskOnThisScript(c2lTask)
	local bIsThis = u2.getLuaTaskField(c2lTask, "ViewCompName") == getViewCompName() 
		and u2.getLuaTaskField(c2lTask, "ViewCompCsb") == getCsbName()
	return bIsThis
end

-- process view component created task
function doViewCompCreated(c2lTask)
	-- Is clicked task occured on this view component?
	if not isTaskOnThisScript(c2lTask) then
		return false
	end
	
	bViewCompCreated = true
	return false
end

-- process preload ended task
function doPreloadEnd(c2lTask)
	bPreloadEnd = true
	return false
end

function checkTrans2Info()
	if (bPreloadEnd and bViewCompCreated) then
		trans2InfoCommand()
		return true
	end
end

-- task loop
while true do
	-- get the front task
	local c2lTask = u2.frontC2LTask()
	if c2lTask == nil then
		coroutine.yield()
	else
		local bExitLua = false
		local taskType = u2.getLuaTaskType(c2lTask)
		
		if "OT_C2LTask_ViewCompCreated" == taskType then
			bExitLua = doViewCompCreated(c2lTask)
		elseif "OT_C2LTask_PreloadEnd" == taskType then
			bExitLua = doPreloadEnd(c2lTask)
		else
		end
		
		-- destroy this task
		u2.destroyC2LTask(c2lTask)
		
		-- exit lua
		if bExitLua then
			break
		end
	end
	
	-- check whether we can trans to next view component
	if checkTrans2Info() then
		break
	end
end


u2.log(Constant.LML_Normal, "===== LogoViewComp.lua end =====")
