/*-- Antispamrule --*/

#strict 2

protected func Activate(iByPlayer) {
	MessageWindow(GetDesc(), iByPlayer);
}

//Konfiguration
global func AntiSpamMaxSpells() {return 2;}
global func AntiSpamTime() {return 190;} //Zauber max. 2 Mal in 175 Frames erlaubt


//Teilantispameffektinformationen

public func FxAntiSpamStart(object pTarget, int iEffectNumber, int iTemp, id var1) {
	if(iTemp) return; //nur temporär hinzugefügt, rest des Skripts wird ignoriert
	EffectVar(0,pTarget,iEffectNumber)=var1; //idSpell
	EffectCall(pTarget,pTarget->GetAntiSpamBaseEffectID(),"Change",var1);
	return 1;
}

public func FxAntiSpamTimer(object pTarget,int iNum) {
	//Effekt wird entfernt
	EffectCall(pTarget,pTarget->GetAntiSpamBaseEffectID(),"Change",EffectVar(0,pTarget,iNum),true);
	//RemoveEffect("AntiSpam",pTarget,iNum);
	return -1;
}


/* ---################## Basiseffekt ##################--- */

public func FxAntiSpamBaseStart(object pTarget,int iIndex,bool bTemp) {
	if(bTemp) return;
	EffectVar(0,pTarget,iIndex)=0;
}

public func FxAntiSpamBaseChange(object pTarget,int iIndex,id idSpell, bool bRemove) {
	for(var i=1;i<=EffectVar(0,pTarget,iIndex);++i) {
		if(EffectVar(2*i-1,pTarget,iIndex)==idSpell) { //Zauber wurde bereits gesprochen
			if(bRemove)
				EffectVar(2*i,pTarget,iIndex)--;
			else
				EffectVar(2*i,pTarget,iIndex)++;
			return 1;
		}
	}
	//Sind wir hier angekommen? Dann wurde der Zauber noch nie gesprochen, neuen Eintag anlegen
	EffectVar(0,pTarget,iIndex)++;
	EffectVar(2*EffectVar(0,pTarget,iIndex)-1,pTarget,iIndex)=idSpell;
	EffectVar(2*EffectVar(0,pTarget,iIndex),pTarget,iIndex)=1; //Wenn der Zauber zum ersten Mal verwendet wird, wird sein spamwert 1 sein
	return 1;
}
public func FxAntiSpamBaseGet(object pTarget,int iIndex,id idSpell) { //Wird zur Abfrage der Effektanzahl eines bestimmten Zaubers aufgerufen
	for(var i=1;i<=EffectVar(0,pTarget,iIndex);++i) {
		if(EffectVar(2*i-1,pTarget,iIndex)==idSpell)
			return EffectVar(2*i,pTarget,iIndex);
	}
	return 0;
}