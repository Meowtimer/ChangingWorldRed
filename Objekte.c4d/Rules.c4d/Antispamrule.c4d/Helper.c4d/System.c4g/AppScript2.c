/* --- Antispamappendto --- */

#strict 2

#appendto CLNK

local iAntiSpamBaseEffectID;

protected func Initialize() {
	if(!ObjectCount(ASPR))
		return _inherited();
	AntiSpamCreateBaseEffect();
	return _inherited();
}

public func GetAntiSpamBaseEffectID() { return iAntiSpamBaseEffectID; } //grauenhafte Namensgebung, vermeidet aber mit Sicherheit Kollisionen ;)

public func AntiSpamCreateBaseEffect() {
	iAntiSpamBaseEffectID=AddEffect("AntiSpamBase",this,1,,,ASPR);
}