/*-- Timer --*/

#strict 2

local zeit;  //Zeit in Sekunden

func Initialize() {
	zeit = 9; //Zeit in Sekunden
  return true;
}

protected func Timer(){
  CustomMessage(Format("Verbleibende Zeit: %d", --zeit), 0, NO_OWNER, 0, -200);
	if(!zeit)     {GameCall("TimerFunktion"); RemoveObject();}
	return true;
}

protected func Fertig(){
	zeit = 1;
	return true;
}
