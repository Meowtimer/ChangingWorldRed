/*-- Neues Script --*/

#strict
#appendto MAGE

func FxCatchBlowHelperTimer() { return(-1); }   

//Objekte die sofort töten brauchenQueryCatchBlow   
func QueryCatchBlow(object pBy) {
  _inherited();

  //Nicht mehrmals das gleiche Objekt in kurzer Zeit
  if(GetEffect("CatchBlowHelper",pBy)) return(0);
  else AddEffect("CatchBlowHelper",pBy,175,105,this(),GetID(this())); //3 Sekunden, dann zählt mal
  
  //Log("%i",GetID(pBy));
  
  var iID=GetID(pBy);
  //Projektile auf Zauberanzeige setzen
  if(iID==GRBB) iID=GRSF;
  if(iID==GRBL) iID=GRSG;
 
 //Rückschlag erkennung 
  if(GetEffect("Backlash",pBy))
    if(GetEffect("KillSymbols",this())) SetKillSymbols(this(),GetEffect("KillSymbols",this()),FHRT);
  if(GetEffect("KillSymbols",this())) 
	SetKillSymbols(this(),GetEffect("KillSymbols",this()),iID);
 
  return(0);
 }


 