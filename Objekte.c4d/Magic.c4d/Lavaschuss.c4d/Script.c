/*-- Blitz-Zauber --*/

#strict

func Activate(pCaster,pRealcaster) {
  var pClonk;
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;

  var iResult;
  if (iResult = CheckEffect("Blast", 0, 125)) return(iResult!=-1 && RemoveObject());
  
  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this())) return(1);

  // Blitz erzeugen
  var obj = CreateObject(_LVS, 0, 0, GetOwner(pClonk));
  obj->Launch(pClonk, GetDir(pClonk), GetX(pClonk), GetY(pClonk), 0);
  Sound("Inflame");
  RemoveObject();
  return(1);
}

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle)
  {
  // Blitz erzeugen
  var obj = CreateObject(_LVS, 0, 0, GetOwner(pCaller));
  obj->Launch(pCaller,GetDir(pCaller), GetX(pCaller), GetY(pCaller), (900-iAngle*10));
  Sound("Inflame");

  // Objekt löschen
  RemoveObject();
  return(1);
  }

// Zielspruch (für Artefaktzauber)
public func IsAimerSpell() { return(1); }

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("445"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
