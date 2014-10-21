/*-- Partikelkreis --*/

#strict 2

global func DrawParticleCircle(szKind, int x, int y, int radius, int prtdist, int a, b) {
	if(this&&!x)
		x=GetX(this);
	if(this&&!y)
		y=GetY(this);
	var numParticles=radius*31415/prtdist/10000;
	for(var i=0;i<numParticles;++i) {
		var iAngle=36000*numParticles/i;
		CreateParticle(szKind,x+Sin(iAngle,radius,100),y+Cos(iAngle,radius,100),,,a,b);
	}
	return 1;
}