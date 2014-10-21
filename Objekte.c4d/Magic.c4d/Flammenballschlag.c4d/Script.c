/*-- Feuerball --*/

#strict

local pClonk;
local iAimAngle;

func Activate(pCaller, pRealCaster)
{
  // Zauber
  Sound("Magic*");
  var iX, iY;
  iX=AbsX(GetX(pCaller)-15+30*GetDir(pCaller));
  iY=AbsY(GetY(pCaller));
  // Zielen wenn möglich
  if (!pRealCaster) pRealCaster = pCaller;
  if (pRealCaster->~DoSpellAim(this(), pCaller)) return(1);
    // Zielen wenn möglich
  if (pCaller->~DoSpellAim(this())) return(1);
  // Clonk kann nicht zielen:
  ActivateAngle(pCaller, GetDir(pCaller)*180-90);
  return(1);
  RemoveObject();
  return(1);
}

public func ActivateAngle(object pCaller, int iAngle)
  {
  SetPosition(GetX(pCaller), GetY(pCaller));
  pClonk = pCaller;
  iAimAngle = iAngle;
  ScheduleCall(this(), "CastRock", 1, 0, pClonk, iAimAngle+RandomX(10, -10)); 
  // Objekt löschen
  return(Schedule("RemoveObject()", 100, 0, this()));
  }
  
  func CastRock(pCaller, iAngle)
{
   var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
 
  var pRock = CreateContents(GRBB,pCaller);

  // Austrittsgeschwindigkeit ermitteln 
  if(ObjectCount(RMGS))
  {
  	iOutXDir = GetXDir(pCaller,10)+Sin(iAngle, 100);
  	iOutYDir = GetYDir(pCaller,10)-Cos(iAngle, 100);
  }
  else
  {
  	iOutXDir = +Sin(iAngle, 100);
  	iOutYDir = -Cos(iAngle, 100);
  }
  // Austrittsposition... 
  iOutX = Sin(iAngle, 25 * GetCon() / 100);
  iOutY = -Cos(iAngle, 25 * GetCon() / 100); 
  // Drehung 
  iOutR = iAngle; 
  if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  
  // Besitzer des Projektils setzen
  pRock->SetOwner(pCaller->GetOwner());
  // Stein abfeuern
  Exit(pRock, iOutX,iOutY,iOutR,,,iOutRDir );
  pRock->SetXDir(iOutXDir);
  pRock->SetYDir(iOutYDir);
  //pRock->SetXDir(iOutXDir); pRock->SetYDir(iOutYDir);
}


public func IsAimerSpell() { return(1); }

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("336"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
