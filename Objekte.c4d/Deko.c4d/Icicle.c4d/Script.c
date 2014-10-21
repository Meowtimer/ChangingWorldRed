/*-- Eiszapfen --*/

#strict

local phase;

func Initialize() {
  SetAction("Hanging");
  SetPhase(phase=Random(4));
  AddEffect("IntSparks", this, 1, 100, this);
}

func Hit() {
  var obj;
//  if(GetAction()ne"Falling") return(1);
  Sound("RockBreak*");
  Sound("Blast2",0,0,50);
  // links oben
  ObjectSetAction(obj=CreateObject(_ICP,-4,-4,-1),"Exist");
  SetPhase(phase*2,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(30)-15,-Random(35)+10,obj);
  // rechts oben
  ObjectSetAction(obj=CreateObject(_ICP,3,-4,-1),"Exist");
  SetPhase(phase*2+1,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(30)-15,-Random(35)+10,obj);
  // unten
  ObjectSetAction(obj=CreateObject(_ICP,0,12,-1),"LameExist");
  SetPhase(phase,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(10)-15,-Random(35)+10,obj);
  CastParticles("PSpark", 5+Random(6), 20, 0, 0, 20, 40, RGB(200,200,200), RGB(200,200,255));
  CastParticles("Smoke", 3+Random(3), 5, 0, 0, 50, 75, RGBa(255, 255, 255, 128), RGBa(255,255,255,255));
  RemoveObject();
}

private func Check() {
  if (GetCategory() == C4D_StaticBack && (!Stuck() || !Random(250)))
    SetCategory(C4D_Object());
  if (GetCategory() == C4D_Object && Stuck())
    SetPosition(GetX(), GetY()+1);
}

private func FxIntSparksTimer() {
  if (Random(3)) return;
  CreateParticle("PSpark", Random(15)-7, Random(26)-25, Random(5)-2, Random(6)+5, 25, RGB(200,200,200+Random(56)));
}