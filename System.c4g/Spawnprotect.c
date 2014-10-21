#strict 2
#appendto MAGE

public func ExecMagic(id idSpell) {
	//if(GetEffect("JustSpawned",this)) Log("%i %d",idSpell,GetTime());
	if(GetEffect("JustSpawned",this)&&DefinitionCall(idSpell,"HasSpawnprotect"))
		return(PlayerMessage(GetController(), "<c ff0000>Spawnschutz</c>", this));
	return(_inherited(idSpell));
}