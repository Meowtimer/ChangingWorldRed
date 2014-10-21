/*-- Blitz --*/

#strict

/* Locals */

local iAdvX, iVarX, // Bewegung / Abweichung X
      iAdvY, iVarY; // Bewegung / Abweichung Y
local pCaster;

func Initialize() {
  //Killtrace Effect
  AddEffect("KillTraceHelper",this(),175,0,this(),GetID(this()));
  }
	  
/* Aktivierung */

public func Activate(iStartX, iStartY, iAdvanceX, iRandomX, iAdvanceY, iRandomY)
{
  // Startposition
  AddVertex(iStartX,iStartY);
  // Bewegungsverhalten
  iAdvX=iAdvanceX; iVarX=iRandomX;
  iAdvY=iAdvanceY; iVarY=iRandomY;
  // Aktion setzen
  SetAction("Advance");
  // Geräusch
  if (!Random(5))   Sound("Inflame");
  return(1);
}

public func Launch(pClonk, iDir, iX, iY, iAngle)
{
  pCaster=pClonk;
  var XDir;
  XDir = Cos(iAngle, 10, 10);
//  if (iDir == DIR_Left()) XDir = -Cos(iAngle, 10, 10);
//  else (XDir = +Cos(iAngle, 10, 10));
  Activate(iX, iY, XDir, 0, -Sin(iAngle, 10, 10));
  return(1);
}

private func Timer()
{
  
  var i=0;
  var pObject = FindObject(0,-2,-2,4,4,OCF_Inflammable);
  if(pObject) 
   if(pObject != pCaster) { 
      //Nicht benötigt, macht Query Catch Blow
      //if(GetEffect("KillSymbols",pObject)) SetKillSymbols(pObject,GetEffect("KillSymbols",pObject),DFLM,GetEffect("KillTraceHelper",this()));	
	Incinerate(pObject);
	}
  // Partikel-Effekt
  while(i<GetVertexNum()-1)
  {
    DrawParticleLine ("LightningSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(), 6, 60, RGB(250,100,0));
    DrawParticleLine ("LightningSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(),  3, 25, RGB(250,050,025));
    i++;
 
 //CreateParticle("PSpark", RandomX(-3,4), RandomX(-1,2), RandomX(-2,1), RandomX(-1,2), RandomX(10,20), RGB(250,020,010));
 //CreateParticle("PSpark", RandomX(-3,3), RandomX(-2,2), RandomX(-2,2), RandomX(-2,2), RandomX(20,50), RGB(250,050,020));

 
  }
}

/* Bewegung */

private func Advance()
{
  // Einschlag
  var iVtx = GetVertexNum()-1, iVtxX = GetVertex(iVtx, 0), iVtxY = GetVertex(iVtx, 1);
  if (GBackSolid(iVtxX-GetX(), iVtxY-GetY() ))
    return(Remove());

  // Neuen Eckpunkt
  if (!AddVertex( iVtxX+iAdvX+RandomX(-5,5), iVtxY+iAdvY+RandomX(-5,5)))
    return(Remove());

  SetPosition(GetVertex(GetVertexNum()), GetVertex(GetVertexNum(), 1));
  
  // Im Wasser?
  if (GBackLiquid(iVtxX-GetX(), iVtxY-GetY() ))
  {
	  var obj;
	  // Alle Lebewesen in der Nähe bekommen was ab
	  while (obj = FindObject( 0, iVtxX-GetX()-400, iVtxY-GetY()-400, 800, 800, OCF_Alive(), 0,0, NoContainer(),obj  ) )
    	  {
      	    if(!PathFree(iVtxX, iVtxY, GetX(obj), GetY(obj))) continue;
	    if(!GBackLiquid(GetX(obj)-GetX(), GetY(obj)-GetY())) continue;
  	    obj->~LightningStrike();
  	    Punch(obj, 20);
		//Regelt Query Catch Blow
                //if(GetEffect("KillSymbols",obj)) SetKillSymbols(obj,GetEffect("KillSymbols",obj),_LVS);
            
        
           DrawParticleLine ("LightningSpark2", iVtxX-GetX(), iVtxY-GetY(), 
		    GetX(obj)-GetX(), GetY(obj)-GetY(), 6, 60, RGB(255,010,055));
            DrawParticleLine ("LightningSpark2", iVtxX-GetX(), iVtxY-GetY(), 
		    GetX(obj)-GetX(), GetY(obj)-GetY(),  3, 25, RGB(250,050,055));
          }
	  return(Remove());
  }
  
  // Objektanziehung
  iVtx = GetVertexNum()-1; iVtxX = GetVertex(iVtx, 0); iVtxY = GetVertex(iVtx, 1);
  var obj;
  if (iVtx>7)
    while (obj = FindObject( 0, iVtxX-GetX()-25, iVtxY-GetY()-25, 50, 50, OCF_Alive(), 0,0, NoContainer(),obj  ) )
    {
	  if(obj==pCaster) continue;
      if(!PathFree(iVtxX, iVtxY, GetX(obj), GetY(obj))) continue;
      Attraction(obj);
      break;
    }
  
  Timer();
  // Weiter	
  return();
}

/* Effekte */

private func Sparkle()
{
  Timer();
  var iVtx = GetVertexNum()-1, iVtxX = GetVertex(iVtx, 0), iVtxY = GetVertex(iVtx, 1);
 
  return(1);
}

/* Anziehung */

private func Attraction(obj)
{
  // Einschlag in Objekt
  AddVertex( GetVertex(0,0,obj)+GetX(obj),
             GetVertex(0,1,obj)+GetY(obj) );
  obj->~LightningStrike();
  SetAction("Connect");
  Punch(obj, 20);
  Incinerate(obj);
  //Übernimmt QueryCatchBlow
  //if(GetEffect("KillSymbols",obj)) SetKillSymbols(obj,GetEffect("KillSymbols",obj),_LVS,GetEffect("KillTraceHelper",this()));  
}

/* Ende */

private func Remove()
{
  var i=0;
  // Nachleuchten
  while(i<GetVertexNum()-1)
  {
    Explode(2,this());
    DrawParticleLine ("Blast", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(), 6, 60, RGB(255,100,000));
    DrawParticleLine ("Blast", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(),  3, 25, RGB(250,150,155));
    i++;
  }
  
  RemoveObject();
}
