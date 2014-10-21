/* --- Frostwellenappendto --- */

#strict
#appendto MFWV

func FxFrostwaveNSpellTimer(object pTarget, int iNumber, int iTime) {

  var iRaduis = EffectVar(1,pTarget,iNumber); //20
  // löschen falls schon zu weit
  if(iRaduis >= EffectCall(pTarget,iNumber,"MaxRange")) return(-1);

  // Position
  var iX = EffectVar(2,pTarget,iNumber);
  var iY = EffectVar(3,pTarget,iNumber);
  
  // Kombo?
  var dwSearchOCF = OCF_Alive;
  if (EffectVar(5,pTarget,iNumber)) dwSearchOCF += OCF_Collectible;
  
  // Effekt!
  CreateParticle("FrostWave",iX,iY, RandomX(-1,1),RandomX(-1,1), 400+10*iRaduis,RGB(255,255,255));
  //var obj;
  
  //Bei Gefundener Antilavaschussregel Lavaschüsse töten
  if(ObjectCount(FLSB))
  	{
  		for(var p in FindObjects(Find_ID(_LVS),Find_InRect(-iRaduis-5+iX,-iRaduis-5+iY,iRaduis*2+10,iRaduis*2+10),Find_Not(Find_Owner(GetOwner(pTarget)))))
  			RemoveObject(p);
  	}
  
  var angle;
  
  //Array -   // Alle Gegner wegschleudern
  for(var obj in FindObjects(Find_NoContainer(),Find_InRect(-iRaduis-5+iX,-iRaduis-5+iY,iRaduis*2+10,iRaduis*2+10),
	  Find_OCF(dwSearchOCF),Find_Exclude(EffectVar(4,pTarget,iNumber)))) {
   if(Inside(Distance(GetX(obj), GetY(obj), iX, iY), iRaduis-20, iRaduis+20))
    {   
    angle=Angle(iX, iY, GetX(obj), GetY(obj) );     
    if(GetAlive(obj)) 
    { 
        Fling(obj, Sin(angle, 80)/9, -Cos(angle, 80)/9);
        if(!Frozen(obj)) { Freeze(obj, EffectVar(4,pTarget,iNumber)); 
		 if(GetEffect("KillSymbols",obj)) SetKillSymbols(obj,GetEffect("KillSymbols",obj),MFWV,iNumber);  
		 }
    }
    else 
     { 
        SetXDir(Sin(angle, 50), obj); SetYDir(-Cos(angle, 50), obj);
        SetController(GetController(EffectVar(4,pTarget,iNumber)),obj);
     }
    }
  }
  
  
  // Alle Gegner wegschleudern
  /*
  while(obj = FindObject(0, -iRaduis-5+iX,-iRaduis-5+iY,iRaduis*2+10,iRaduis*2+10, dwSearchOCF, 0,0, NoContainer(), obj))
  {
    if(obj == Contained()) continue;
	if(obj == EffectVar(4,pTarget,iNumber)) continue; //Zauberer trifft sich nicht selbst	
    if(Inside(Distance(GetX(obj), GetY(obj), iX, iY), iRaduis-5, iRaduis+5))
    {   
    angle=Angle(iX, iY, GetX(obj), GetY(obj) );     
    if(GetAlive(obj)) 
    { 
        Fling(obj, Sin(angle, 80)/9, -Cos(angle, 80)/9);
        if(!Frozen(obj)) { Freeze(obj, EffectVar(4,pTarget,iNumber)); 
		 if(GetEffect("KillSymbols",obj)) SetKillSymbols(obj,GetEffect("KillSymbols",obj),MFWV);  
		 }
    }
    else 
     { 
        SetXDir(Sin(angle, 50), obj); SetYDir(-Cos(angle, 50), obj);
        SetController(GetController(EffectVar(4,pTarget,iNumber)),obj);
     }
    }
  } */
    
  EffectVar(1,pTarget,iNumber)+=8;
}