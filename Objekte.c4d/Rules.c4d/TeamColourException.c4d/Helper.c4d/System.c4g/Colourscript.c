#strict 2

#appendto CLNK

//Engine workaround, Recruitment ruft nicht den eigentlichen Besitzer auf. Scheinbar gehoeren dem letzten Spieler zum Zeitpunkt alle Clonks. (ala)
func Initialize() {
 var rule = FindObject(TCEX);
 //Log("Initialize %v %v", this, rule);
 if (rule) ScheduleCall(rule, "CheckRecolorCrew",1,0,this);
 return _inherited(...);
 }
