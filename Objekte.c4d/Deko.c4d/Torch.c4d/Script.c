/*-- Fackel --*/

#strict 2

func Initialize() {
 Activate();
 }

public func Activate()
{
  if(GetAction() == "Burning") return Extinguishsh();
  SetAction("Burning");
  AddEffect("IntSparks", this, 1, 50, this);
  return this ;
}
  
private func Extinguishsh()
{
  SetAction("Idle");
  Sound("Pshshsh");
  return 1;
}

private func FxIntSparksTimer() {
  if (Random(3)) return;
  CreateParticle("Fire2", Random(3)-1, Random(5)-7, Random(15)-7, Random(15)-7, Random(11)+25, RGB(255, Random(255)));
}
