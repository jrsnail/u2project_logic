
function trans2InfoCommand()
	local father = u2.retrieveContext("ON_Context_Root")
	local infoContext = u2.createContextChild(father
		, Constant.OT_Context, "ON_Context_InfoScene", Constant.ON_Facade_App
		, Constant.OT_CocosViewComponent, "InfoViewComponent", "InfoViewComp.lua")
	u2.pushContextQueueBack(Constant.ON_ContextQueue_Scene, infoContext, Constant.TransType_TT_OneByOne)
end


