/*--- Der Stein ---*/

#strict

protected func Hit()
{
  Sound("RockHit*");
  return(1);
}

local alpha;

private func FadeOut()
{
  alpha+=10;
  if(alpha>240) RemoveObject();
  SetClrModulation(RGBa(255,255,255,alpha));
}
