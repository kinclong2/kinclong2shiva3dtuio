--------------------------------------------------------------------------------
--  State............ : kameraSetate
--  Author........... : 
--  Description...... : 
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
function kameraAIModel.kameraSetate_onEnter ( )
--------------------------------------------------------------------------------
	
	--
	-- Write your code here, using 'this' as current AI instance.
	--
	object.setTranslation ( this.getObject ( ), 10, 10, 10,  object.kGlobalSpace )
    object.lookAt ( this.getObject ( ), 0, 0, 0, object.kGlobalSpace, 1 )
    
--------------------------------------------------------------------------------
end
--------------------------------------------------------------------------------
