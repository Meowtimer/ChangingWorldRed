/* --- Antispamappendto --- */

#strict 2

//Liste der Zauber, die nicht gespammt werden sollen
#appendto MFWV
#appendto GRSG
//#appendto MGSG
#appendto GRSF
#appendto LAVS
#appendto ABLA
#appendto MFWL

public func HasSpamprotect() {return 1;}

protected func Activate(pCaller,pRealCaller) {
	if(!ObjectCount(ASPR)||!pCaller)
		return(_inherited(pCaller,pRealCaller));
	if(this->~IsAimerSpell()) //Bei Zielzaubern wird erst in der ActivateAngle der Spamcheck gemacht sonst kann man sich in der Luft nicht mehr umdrehen
		return _inherited(pCaller,pRealCaller);
	if(EffectCall(pCaller,pCaller->GetAntiSpamBaseEffectID(),"Get",GetID(this))>=AntiSpamMaxSpells()) { //SPAM!11
		Sound("Error",true,,,GetOwner(pCaller)+1);
		PlayerMessage(GetOwner(pCaller),"<c ff0000>Spamschutz</c>",pCaller);
		DoMagicEnergy(GetValue(this),pCaller);
		return 1;
	}
	//EffectCall(pCaller,pCaller->GetAntiSpamBaseEffectID(),"Change",GetID(this));
	AddEffect("AntiSpam",pCaller,2,AntiSpamTime(),,ASPR,GetID(this));
	return _inherited(pCaller,pRealCaller);
}

public func ActivateAngle(pCaller,iAngle) {
	if(!ObjectCount(ASPR))
		return(_inherited(pCaller,iAngle));
	if(EffectCall(pCaller,pCaller->GetAntiSpamBaseEffectID(),"Get",GetID(this))>=AntiSpamMaxSpells()) { //SPAM!11
		Sound("Error",true,,,GetOwner(pCaller)+1);
		PlayerMessage(GetOwner(pCaller),"<c ff0000>Spamschutz</c>",pCaller);
		DoMagicEnergy(GetValue(this),pCaller);
		return 1;
	}
	//EffectCall(pCaller,pCaller->GetAntiSpamBaseEffectID(),"Change",GetID(this));
	AddEffect("AntiSpam",pCaller,2,AntiSpamTime(),,ASPR,GetID(this));
	return _inherited(pCaller,iAngle);
}
	
