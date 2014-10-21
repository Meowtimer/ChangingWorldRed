/*-- Deko-Kristall --*/

#strict 2

local party, phase, richtung; //richtung == 0 rechts, sonst links


func Set(int iphase, int size, int rotation, int dir, int color, bool bparty)
{
	SetAction("Be");
	SetPhase(iphase);
	SetCon(size);
	SetR(rotation);
	SetDir(dir);
	SetClrModulation(color);
	party = bparty;
	phase = iphase;
	return true;
}

//klappt momentan nur für blau
protected func Timer(){
  if(!party) return false;
  //++zaehler;
  //if((zaehler % 36) == 0) Set(Random(10), GetCon(), GetR(), GetDir(), GetClrModulation(), 1);
  if(!richtung){
    if((GetClrModulation() % 65536) >= 65536 - 2*256|| (GetClrModulation() % 16777216) >= 16777216 - 2*65536) richtung = 1;
    Set(phase, GetCon(), GetR(), GetDir(), GetClrModulation() + 1*256 + 1*65536, true);
  }
  else {
    if((GetClrModulation() % 65536) <= 2*256 || (GetClrModulation() % 16777216) <= 2*65536) richtung = 0;
    Set(phase, GetCon(), GetR(), GetDir(), GetClrModulation() - 1*256 - 1*65536, true);
  }
	return true;
}
