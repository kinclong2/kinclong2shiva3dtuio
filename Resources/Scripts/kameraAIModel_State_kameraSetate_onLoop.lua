--------------------------------------------------------------------------------
--  State............ : kameraSetate
--  Author........... : 
--  Description...... : 
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
function kameraAIModel.kameraSetate_onLoop ( )
--------------------------------------------------------------------------------
	
	--
	-- Write your code here, using 'this' as current AI instance.
	--
	
    object.rotateAround ( this.getObject ( ), 0, 0, 0, 0, 1, 0, object.kGlobalSpace )
    object.lookAt ( this.getObject ( ), 0, 0, 0, object.kGlobalSpace, 1 )
--------------------------------------------------------------------------------
end
--------------------------------------------------------------------------------