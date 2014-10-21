#strict 
local target;

public func Activate(caster1, caster2) {
  // Tats�chliches Ziel ermitteln
  target = caster1;

  // caster2 auf den Container vom Zauberer setzen falls nicht �bergeben
  if (!caster2) caster2 = Contained(caster1);

  // Wirklichen Caster �berpr�fen
  if (caster2) {
    // Container lebt?
    if (GetOCF(caster2) & OCF_Alive()) {
      // Zielobjekt = Container
      target = caster2;
    }
    // Sonst Fehlermeldung
    else 
      return(CastError("$NotInFree$", target));
  } 
  // Einen Frame verz�gert springen
  SetAction("Jump");
  
  // Fertig
  return(1);
}
/*
protected func DoJump() {
  // Sound
  Sound("ToadJump");
  SetPosition(GetX(target), GetY(target) - 1, target);

  // Aktivit�t setzen
  ObjectSetAction(target, "Jump");
  
  // Richtung rauskriegen
  var dir = -1;
  if (GetDir(target) == 1) dir = +1;

  // X/Y-Dir setzen
  SetSpeed(35 * dir, -70, target); // Einheitlicher Sprung

  // Zerst�ren
  RemoveObject();
} */

//Realistischer Kr�tensprung automatisch aktiviert
protected func DoJump() {
  // Sound
  Sound("ToadJump");
  SetPosition(GetX(target), GetY(target) - 1, target);

  // Aktivit�t setzen
  ObjectSetAction(target, "Jump");
  
  // Richtung rauskriegen
  var dir = -1;
  if (GetDir(target) == 1) dir = +1;
	var x,y;
	//if(ObjectCount(RMGT)) {
		x=35 * dir * GetPhysical("Jump",,target)/40000;
		y=-70 * GetPhysical("Jump",,target)/40000;
	/*}
	else {
		x=35 * dir;
		y=-70;
	} */
  // X/Y-Dir setzen
  	SetSpeed(x,y, target); // Einheitlicher Sprung

  // Zerst�ren
  RemoveObject();
}

protected func CastError(mString, caster, silent) {
  // Meldung ausgeben
  if (!silent) Message(mString, caster, GetName(caster));
  // Sound
  if (!silent) Sound("Error");
  // Action zur�cksetzen
  if (GetAction(caster) eq "Magic") ObjectSetAction(caster, "Walk");
  // Selbstzerst�rung
  RemoveObject();
  // Fehlschlag
  return 0;
}
public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("52"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
