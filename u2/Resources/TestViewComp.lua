u2.log(Constant.LML_Normal, "===== LogoViewComp.lua start =====")


-- ------------------- init start -------------------
function getViewCompName()
	return "InfoViewComponent"
end

function getCsbName()
	return "HomePage.csb"
end

function trans2InfoCommand()
	local father = u2.retrieveContext("ON_Context_Root")
	local infoContext = u2.createContextChild(father
		, Constant.OT_Context, "ON_Context_InfoScene", Constant.ON_Facade_App
		, Constant.OT_CocosViewComponent, getViewCompName(), getCsbName())
	u2.pushContextQueueBack(Constant.ON_ContextQueue_Scene, infoContext, Constant.TransType_TT_Overlay)
end

-- trnas to shade view component
trans2InfoCommand()

-- register notifications which interested by script
u2.registerLuaScriptWithInterestedNotification({
"NTF_Predefined_ViewComp_Created", 
"NTF_Predefined_Button_Clicked", 
"NTF_Predefined_Touches_Began", 
"NTF_Predefined_Touches_Moved", 
"NTF_Predefined_Touches_Ended", 
"NTF_Predefined_Touches_Cancelled", 
"NTF_Predefined_KeyPressed", 
"NTF_Predefined_KeyReleased"
})
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
	
	--u2.registerViewCompEventListenerWithFixedPri(getViewCompName(), Constant.EventListenerType_TouchAllAtOnce, -256)
end

-- process button clicked task
function doButtonClickedTask(c2lTask)
	-- Is clicked task occured on this view component?
	if not isTaskOnThisScript(c2lTask) then
		return
	end
	
	-- process every clicked task occured on this view component
	local szWidgetName = u2.getLuaTaskField(c2lTask, "WidgetName")
	if "btn_home" == szWidgetName then
		local l2cTask = u2.createL2CTask(Constant.OT_CreateLuaScriptLuaTask)
		u2.setLuaTaskField(l2cTask, "FileName", "BattleViewComp.lua")
		u2.setLuaTaskField(l2cTask, "FacadeName", Constant.ON_Facade_App)
		u2.pushL2CTask(l2cTask)
	else
	end
end

-- process touches bagan task
function doTouchesBeganTask(c2lTask)
	-- Is clicked task occured on this view component?
	if not isTaskOnThisScript(c2lTask) then
		return
	end
	
	-- process touches bagan task
	local nCount = tonumber(u2.getLuaTaskField(c2lTask, "TouchesCount"))
	for i = 1, nCount, 1 do 
		local vec2 = {}
		vec2.x = tonumber(u2.getLuaTaskField(c2lTask, "x" .. i))
		vec2.y = tonumber(u2.getLuaTaskField(c2lTask, "y" .. i))
	end
	
	local l2cTask = u2.createL2CTask("Test_L2CType", "TestName")
	u2.pushL2CTask(l2cTask)
	u2.log(Constant.LML_Trivial, "===== doTouchesBeganTask =====")
end

while true do
	-- get the front task
	local c2lTask = u2.frontC2LTask()
	if c2lTask == nil then
		coroutine.yield()
	else
		local taskType = u2.getLuaTaskType(c2lTask)
		
		if "OT_C2LTask_ViewComp_Created" == taskType then
			doViewCompCreated(c2lTask)
		elseif "OT_C2LTask_ButtonCliecked" == taskType then
			doButtonClickedTask(c2lTask)
		elseif "OT_C2LTask_TouchesBegan" == taskType then
			doTouchesBeganTask(c2lTask)
		else
		end
		
		-- destroy this task
		u2.destroyC2LTask(c2lTask)
	end
end


u2.log(Constant.LML_Normal, "===== LogoViewComp.lua end =====")
