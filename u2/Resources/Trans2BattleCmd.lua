
function trans2BattleCommand()
	local father = u2.retrieveContext("ON_Context_Root")
	local battleContext = u2.createContextChild(father
		, Constant.OT_Context, "ON_Context_BattleScene", Constant.ON_Facade_App
		, Constant.OT_CocosViewComponent, "BattleViewComp", "BattleViewComp.lua")
	local masterContext = u2.createContextChild(battleContext
		, Constant.OT_Context, "ON_Context_BattleMaster", Constant.ON_Facade_App
		, Constant.OT_CocosViewComponent, "BattleMasterViewComp", "BattleMasterViewComp.lua")
	local rowContext = u2.createContextChild(battleContext
		, Constant.OT_Context, "ON_Context_RowMaster_1", Constant.ON_Facade_App
		, Constant.OT_CocosViewComponent, "BattleRowViewComp_1", "BattleRowViewComp.lua")
		
	u2.pushContextQueueBack(Constant.ON_ContextQueue_Scene, battleContext, Constant.TransType_TT_OneByOne)
end

