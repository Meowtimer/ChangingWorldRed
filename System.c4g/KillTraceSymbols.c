#strict

//AddEffect("KillSymbols",pClonk,50);

//SetKillSymbols(pTarget,GetEffect("KillSymbols",pTarget),id);

global func SetKillSymbols(object pTarget, int iEffectNumber, id idSymbol, int iEffectOrigin) {
 if(iEffectOrigin) {
  //Log("%d",iEffectOrigin);
  //Nicht mehrmals den gleichen Effekt in kurzer Zeit
  if(GetEffect(Format("%s%d","EffectBlowHelper",iEffectOrigin),pTarget)) {
   return(1);
   }
  else {
   var iOriginNumber=AddEffect(Format("%s%d","EffectBlowHelper",iEffectOrigin),pTarget,175);
   Schedule(Format("RemoveEffect(\"%s%d\",Object(%d),%d)","EffectBlowHelper",iEffectOrigin,ObjectNumber(pTarget),iOriginNumber),105);
   }
  }

 //Log("1%i",idSymbol);

 var temp;
 for(var i=0;i<5;i++) {
  if(!EffectVar(i,pTarget,iEffectNumber)) {
    EffectVar(i,pTarget,iEffectNumber)=idSymbol;
	//10 Sekunden speicher
	//Schedule(Format("RegroupEffectVars(Object(%d),%d)",ObjectNumber(pTarget),iEffectNumber),175*2);
	break;
	}
  //Neusetzen wenn voll
  if(i==4) {
   //Löscht den ältesten Eintrag, und schafft Platz für einen neuen  
   DeleteEffectVar(0,pTarget,iEffectNumber);
   EffectVar(i,pTarget,iEffectNumber)=idSymbol;
   //Schedule(Format("RegroupEffectVars(Object(%d),%d)",ObjectNumber(pTarget),iEffectNumber),175*2);   //10 Sekunden
   break;
   }  
  }
 
 return(1);
 } 

//Löscht einen Eintrag, und schafft Platz für einen neuen
global func DeleteEffectVar(int i,object pTarget,int iEffectNumber) {
  if(EffectVar(i,pTarget,iEffectNumber)) {
   EffectVar(i,pTarget,iEffectNumber)=0;
   RegroupEffectVars(pTarget,iEffectNumber);
   return(1);
   }
  else return(0);
 }

//Waere wohl im Nachhinein mit Arrays alles einfacher gewesen - die Funktion hier setzt hartgecodet leere EffectVars aus und fuellt sie mit den folgenden.
global func RegroupEffectVars(object pTarget,int iEffectNumber) {
  //Log("%i%i%i",EffectVar(0,pTarget,iEffectNumber),EffectVar(1,pTarget,iEffectNumber),EffectVar(2,pTarget,iEffectNumber));
  if(!pTarget) return(0);
  if(!GetAlive(pTarget)) return(0);
  
  for(var i=0;i<=4;i++) {
   if(!EffectVar(i,pTarget,iEffectNumber)) {
    if(EffectVar(i+1,pTarget,iEffectNumber)) EffectVar(i,pTarget,iEffectNumber)=EffectVar(i+1,pTarget,iEffectNumber);
    else EffectVar(i,pTarget,iEffectNumber)=0;
    if(EffectVar(i+2,pTarget,iEffectNumber)) EffectVar(i+1,pTarget,iEffectNumber)=EffectVar(i+2,pTarget,iEffectNumber);	
	else EffectVar(i+1,pTarget,iEffectNumber)=0;
    if(EffectVar(i+3,pTarget,iEffectNumber)) EffectVar(i+2,pTarget,iEffectNumber)=EffectVar(i+3,pTarget,iEffectNumber);	
	else EffectVar(i+2,pTarget,iEffectNumber)=0;
    if(EffectVar(i+4,pTarget,iEffectNumber)) EffectVar(i+3,pTarget,iEffectNumber)=EffectVar(i+4,pTarget,iEffectNumber);	
	else EffectVar(i+3,pTarget,iEffectNumber)=0;
	if(!EffectVar(i+4,pTarget,iEffectNumber)) EffectVar(i+4,pTarget,iEffectNumber)=0;
	}
   }
 return(1); 
 }
 
global func FxKillSymbolsTimer(object pTarget, int iEffectNumber, int iEffectTime) {
 //verzögertes Löschen - mit Hilfsvariable, nötig weil CatchBlow erst nach dem Effektende aufgerufen wird.
 if(EffectVar(10,pTarget,iEffectNumber)) {
  EffectVar(10,pTarget,iEffectNumber)++;
  if(EffectVar(10,pTarget,iEffectNumber)>2) return(-1); 
  }  
 
 //Walk, jump, Magic, JumpMagic, Swim - loesen Frostwelle und Windstoss Killverfolgung auf.
 if(GetAction(pTarget) S= "Walk" || GetAction(pTarget) S= "Jump" ||
    /* GetAction(pTarget) S= "Magic" || GetAction(pTarget) S= "JumpMagic" || */
    GetAction(pTarget) S= "Swim")
    for(var i;i<=4;i++) {
	 if(EffectVar(i,pTarget,iEffectNumber) == ABLA ||
        EffectVar(i,pTarget,iEffectNumber) == MFWV)
		  if(!Frozen(pTarget)) DeleteEffectVar(i,pTarget,iEffectNumber);
		  //else Log("!!");
	 }
 
 //Bei voller Energie: Reset
 if(GetEnergy(pTarget)==100) {
    for(var i;i<=4;i++)
	 EffectVar(i,pTarget,iEffectNumber)=0;
  }
 
 return(1);
 }
/*
global func FxKillSymbolsDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause) {
 //über Zusatzparameter dann
 if(GetEffect("KillSymbolsDamageHelperTimer",pTarget)) return(iDmgEngy);
 else AddEffect("KillSymbolsDamageHelperTimer",pTarget,50,15);
 return(iDmgEngy);
 } 
 
global func FxKillSymbolsDamageHelperTimer() { return(-1); } */
 
global func FxKillSymbolsStop(object pTarget, int iEffectNumber, int iReason) {
 if(iReason==4 || EffectVar(10,pTarget,iEffectNumber)) {
   //Löschen, aber erst verzögert
   if(!EffectVar(10,pTarget,iEffectNumber)) {
    EffectVar(10,pTarget,iEffectNumber)=1;
    return(-1);
	}
    //-> OnClonkDeath()
   //Wir sind bereits in der Verzögerung, also Killmessage ausgeben.
   /*
   else {
    var s="";
    for(var i=0;EffectVar(i,pTarget,iEffectNumber);i++) {
     if(GetType(EffectVar(i,pTarget,iEffectNumber)) == C4V_C4ID())
       s=Format("{{%i}}%s",EffectVar(i,pTarget,iEffectNumber),s);
     }
     Log("%s",s);
    } */
  }
 return(1);
 }