/*-- Spark creator --*/

#strict 2

local timetofunk;

func Initialize()
{
 return 1;
}

public func SparkTime()
{
 if(!Random(5))  WannaSparks(1 + Random(2));
 if(!Random(50)) WannaSparks(1 + Random(2 + GetPlayerCount()));
 ++timetofunk;
 if(timetofunk == 10)
  {
   WannaSparks(GetPlayerCount()+Random(3));
   timetofunk = 0;
  }
}

protected func WannaSparks(int anzahl)
{
 for(var a = 0; a < anzahl;++a)
  {
   var posi = PlaceAnimal(WIPF);
   var x = GetX(posi);
   var y = GetY(posi);
   RemoveObject(posi);
   CreateObject(_SPK, x, y-RandomX(5,20), NO_OWNER);
  }
}
