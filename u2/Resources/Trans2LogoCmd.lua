
function trans2LogoCommand()
	local father = u2.retrieveContext("ON_Context_Root")
	local logoContext = u2.createContextChild(father
		, Constant.OT_Context, "ON_Context_LogoScene", Constant.ON_Facade_App
		, Constant.OT_CocosViewComponent, "LogoViewComponent", "LogoViewComp.lua")
	u2.pushContextQueueBack(Constant.ON_ContextQueue_Scene, logoContext, Constant.TransType_TT_Overlay)
end


