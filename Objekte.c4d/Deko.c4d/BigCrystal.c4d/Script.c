/*-- Groﬂer Kristall --*/

#strict

protected func Initialize() 
{
  var rand=1;
  if(Random(2) == 1)rand=-1;
  SetObjDrawTransform(1000*rand,0,0,0,1000);
  AddEffect("IntSparks", this, 1, 60, this);
}

private func FxIntSparksTimer() {
  if (Random(3)) return;
  CreateParticle("PSpark", Random(31)-15, Random(46)-22, Random(9)-4, Random(6)-5, 20+Random(21), RGB(150+Random(51),150+Random(51),200+Random(56)));
}