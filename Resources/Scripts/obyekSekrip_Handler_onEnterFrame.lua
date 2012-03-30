--------------------------------------------------------------------------------
--  Handler.......... : onEnterFrame
--  Author........... : 
--  Description...... : 
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
function obyekSekrip.onEnterFrame (  )
--------------------------------------------------------------------------------
	
	--
	-- Write your code here, using 'this' as current AI instance.
	-- This handler is called every frame.
	--
	
    --
    --
    local nTouchCnt = kinclong2io.GetTouchInput ( )
    
    if nTouchCnt >= 1 then
        local nID = kinclong2io.GetTouchID ( 0 ) -- ini untuk mendapatkan touch id pada element ke 0 pada array internal saudara plugin
        local nx = kinclong2io.GetTouchx ( nID ) -- posisi x dari finger touch dengan identitas nID
        local ny = kinclong2io.GetTouchy ( nID ) -- posisi y dari finger touch dengan identitas nID
        local nYY
        local delta
        
        delta = ny - this.oldY ( )
        object.translate ( this.getObject ( ), 0, delta, 0, object.kGlobalSpace )        
        this.oldY ( ny )
        
    end
    
--------------------------------------------------------------------------------
end
--------------------------------------------------------------------------------
