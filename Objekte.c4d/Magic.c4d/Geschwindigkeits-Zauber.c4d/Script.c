/* Geschwindigkeitszauber */

#strict

public func Activate(pCaster) {
   var Opfer = FindObject(0,-200, -200, 400, 400, OCF_HitSpeed2,0,0,0,Opfer);
Sound("Magic*");
CreateParticle("FSpark", 0,0, 1000, RGBa(255,50,50,150));
CreateParticle("PSpark", RandomX(-3,3), RandomX(-4,4), RandomX(-2,2), RandomX(-2,2), RandomX(200,180), RGB(Random(50),Random(115),Random(255)));
CreateParticle("PSpark", RandomX(-3,3), RandomX(-4,4), RandomX(-2,2), RandomX(-2,2), RandomX(200,180), RGB(Random(50),Random(115),Random(255)));
CreateParticle("PSpark", RandomX(-3,3), RandomX(-4,4), RandomX(-2,2), RandomX(-2,2), RandomX(200,180), RGB(Random(50),Random(115),Random(255)));
CreateParticle("PSpark", RandomX(-3,3), RandomX(-4,4), RandomX(-2,2), RandomX(-2,2), RandomX(200,180), RGB(Random(50),Random(115),Random(255)));
  if(Opfer)
  {
  DrawParticleLine("PxSpark", 0, 0, GetX(Opfer)-GetX(), GetY(Opfer)-GetY(), 20, 100, RGBa(255,255,255,100), RGBa(Random(50),Random(115),Random(255),100));
   DrawParticleLine("PxSpark", 0, 0, GetX(Opfer)-GetX(), GetY(Opfer)-GetY(), 20, 100, RGBa(255,255,255,100), RGBa(Random(50),Random(115),Random(255),100));
    DrawParticleLine("PxSpark", 0, 0, GetX(Opfer)-GetX(), GetY(Opfer)-GetY(), 20, 100, RGBa(255,255,255,100), RGBa(Random(50),Random(115),Random(255),100));
     DrawParticleLine("PxSpark", 0, 0, GetX(Opfer)-GetX(), GetY(Opfer)-GetY(), 20, 100, RGBa(255,255,255,100), RGBa(Random(50),Random(115),Random(255),100));
  SetXDir(GetXDir(Opfer,GetXDir(Opfer)),Opfer,1 - GetXDir(Opfer));
  SetYDir(GetYDir(Opfer,GetYDir(Opfer)),Opfer,1 - GetYDir(Opfer));
  if (GetAlive(Opfer))
  {
    SetKiller(GetOwner(pCaster), Opfer); 
    Opfer->SetKillSymbols(Opfer,GetEffect("KillSymbols",Opfer),GetID());
  }
  else
    SetController(GetOwner(pCaster),Opfer);
  AddEffect("Backlash",Opfer,100);
  }
 RemoveObject(this()); 
  return(1);
}

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("556"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

global func FxBacklashStart() { return(1); }