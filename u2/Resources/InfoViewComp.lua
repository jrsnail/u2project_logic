u2.log(Constant.LML_Normal, "===== InfoViewComp.lua start =====")

require "Trans2BattleCmd"


-- ------------------- init start -------------------
function getViewCompName()
	return "InfoViewComponent"
end

function getCsbName()
	return "HomePage.csb"
end

-- register notifications which interested by script
u2.registerLuaScriptWithInterestedNotification({
"NTF_Predefined_ViewComp_Created", 
"NTF_Predefined_Button_Clicked", 
--"NTF_Predefined_Touches_Began", 
--"NTF_Predefined_Touches_Moved", 
--"NTF_Predefined_Touches_Ended", 
--"NTF_Predefined_Touches_Cancelled", 
--"NTF_Predefined_KeyPressed", 
--"NTF_Predefined_KeyReleased"
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
		return false
	end
end

-- process button clicked task
function doButtonClickedTask(c2lTask)
	-- Is clicked task occured on this view component?
	if not isTaskOnThisScript(c2lTask) then
		return false
	end
	
	-- process every clicked task occured on this view component
	local szWidgetName = u2.getLuaTaskField(c2lTask, "WidgetName")
	if "btn_home" == szWidgetName then
		--[[
		local l2cTask = u2.createL2CTask(Constant.OT_CreateLuaScriptLuaTask)
		u2.setLuaTaskField(l2cTask, "LuaFileName", "BattleViewComp.lua")
		u2.setLuaTaskField(l2cTask, "FacadeName", Constant.ON_Facade_App)
		u2.pushL2CTask(l2cTask)
		--]]
		trans2BattleCommand()
		return true
	else
		return false
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
		elseif "OT_C2LTask_ButtonCliecked" == taskType then
			bExitLua = doButtonClickedTask(c2lTask)
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


u2.log(Constant.LML_Normal, "===== InfoViewComp.lua end =====")
