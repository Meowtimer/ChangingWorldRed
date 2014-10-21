/* Funken */

#strict

local time;

protected func Initialize() { return(SetAction("Fall")); }
protected func Hit() 				{ return(SetAction("Sparkle")); }
protected func Removal() 		{ return(RemoveObject()); }

protected func CastObject()
{
 Sound("Boing",0,0,50);
 var pObj=CreateObject(RndObjectID(),0,0,-1);
 if(pObj->~IsArrowPack()) pObj->~DoPackCount(-RandomX(3,6));
 return(1);
}

protected func RndObjectID()
{
	var rID = [];
    var arrows = [];	
	
	arrows = [ARWP,ARWP,ARWP,XARP,XARP,FARP];
	// Normale Objekte
	rID = [FLNT,SFLN,TFLN,STFN,FBMP,SWOR,WBRL,AXE1,SPER,GUNP,_CBL,_CCL,WOOD];
	if(!Random(17))
		rID = arrows;
	if(!Random(25))
		rID = [CATA,CCAN];
	
	return(rID[Random(GetLength(rID))]);
}

protected func Sparkle()
{
	if(Stuck())
		SetPosition(GetX(), GetY()-2);
	
	CreateParticle("PSpark",0,0,0,0,200,RGB(50,50,255),this(),1);
}

protected func Removing()
{
	if(time > 5) SetAction("Sparkle");
	time++;
	return(1);
}
