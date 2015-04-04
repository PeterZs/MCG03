/* ////////////////////////////////////////////////////////////

YS Class Library
Copyright (c) 2014 Soji Yamakawa.  All rights reserved.

YS Class Library is an open-source class-library project since 1999.
It has been used and battle-tested in commercial, research, and free programs.
Please visit the following URL for more details.

http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

File Name: ysshellutil.cpp
//////////////////////////////////////////////////////////// */

#include "ysclass.h"

YSRESULT YsShellMakeQuadFromEdge
    (YsShellVertexHandle quad[4],YsShellPolygonHandle plHd[2],
     const YsShell &shl,const YsShellVertexHandle vtHd1,const YsShellVertexHandle vtHd2)
{
	YsShellVertexHandle edVtHd[2];
	int nFoundPlHd;
	const YsShellPolygonHandle *foundPlHd;
	YsShellVertexHandle triVtHd[2][3];
	const YsShellSearchTable *search;

	search=shl.GetSearchTable();
	if(search!=NULL)
	{
		edVtHd[0]=vtHd1;
		edVtHd[1]=vtHd2;

		if(search->FindPolygonListByEdge(nFoundPlHd,foundPlHd,shl,edVtHd[0],edVtHd[1])==YSOK && nFoundPlHd==2)
		{
			plHd[0]=foundPlHd[0];
			plHd[1]=foundPlHd[1];
			if(shl.GetNumVertexOfPolygon(plHd[0])==3 && shl.GetNumVertexOfPolygon(plHd[1])==3)
			{
				shl.GetVertexListOfPolygon(triVtHd[0],3,plHd[0]);
				shl.GetVertexListOfPolygon(triVtHd[1],3,plHd[1]);

				int k;
				// 3 vertices from tri[0][k];
				for(k=0; k<3; k++)
				{
					YsShellVertexHandle trEdVtHd[2];
					trEdVtHd[0]=triVtHd[0][ k     ];
					trEdVtHd[1]=triVtHd[0][(k+1)%3];
					if((trEdVtHd[0]==edVtHd[0] && trEdVtHd[1]==edVtHd[1]) ||
					   (trEdVtHd[1]==edVtHd[0] && trEdVtHd[0]==edVtHd[1]))
					{
						quad[0]=triVtHd[0][(k+1)%3];
						quad[1]=triVtHd[0][(k+2)%3];
						quad[2]=triVtHd[0][(k+3)%3];
						break;
					}
				}

				// 1 vertex from tri[1][k];
				for(k=0; k<3; k++)
				{
					if(triVtHd[1][k]!=edVtHd[0] && triVtHd[1][k]!=edVtHd[1])
					{
						quad[3]=triVtHd[1][k];
						break;
					}
				}

				if(quad[0]==quad[1] || quad[0]==quad[2] || quad[0]==quad[3] ||
				   quad[1]==quad[2] || quad[1]==quad[3] ||
				   quad[2]==quad[3])
				{
					return YSERR;
				}
				return YSOK;
			}
		}
	}
	else
	{
		YsErrOut("YsShellMakeQuadForSwapping()\n");
		YsErrOut("  This function needs search table.\n");
	}
	return YSERR;
}


YSRESULT YsShellMakeConnectedPolygonList
    (YsArray <YsShellPolygonHandle> &plHdList,
     const YsShell &shl,YsShellPolygonHandle initPlHd,YSBOOL takeNonManifold,int hashSize)
{
	YsArray <YsShellPolygonHandle,256> todoList;
	YsHashTable <YsShellPolygonHandle> doneList(hashSize);
	const YsShellSearchTable *search;

	plHdList.Set(0,NULL);
	search=shl.GetSearchTable();
	if(search!=NULL)
	{
		todoList.Append(initPlHd);
		doneList.AddElement(shl.GetSearchKey(initPlHd),initPlHd);
		while(todoList.GetN()>0)
		{
			int i,nPlVt;
			YsShellVertexHandle edVtHd[2];
			const YsShellVertexHandle *plVtHd;

			int nSharedPlHd;
			const YsShellPolygonHandle *sharedPlHd;

			plHdList.Append(todoList[0]);

			nPlVt=shl.GetNumVertexOfPolygon(todoList[0]);
			plVtHd=shl.GetVertexListOfPolygon(todoList[0]);
			edVtHd[1]=plVtHd[nPlVt-1];
			for(i=0; i<nPlVt; i++)
			{
				edVtHd[0]=edVtHd[1];
				edVtHd[1]=plVtHd[i];

				if(search->FindPolygonListByEdge(nSharedPlHd,sharedPlHd,shl,edVtHd[0],edVtHd[1])==YSOK &&
				   nSharedPlHd>0)
				{
					if(takeNonManifold!=YSTRUE && nSharedPlHd!=2)
					{
						goto NEXTI;
					}
					int j;
					for(j=0; j<nSharedPlHd; j++)
					{
						YsShellPolygonHandle verifyPlHd;
						if(sharedPlHd[j]!=todoList[0] &&
						   doneList.FindElement(verifyPlHd,shl.GetSearchKey(sharedPlHd[j]))!=YSOK)
						{
							todoList.Append(sharedPlHd[j]);
							doneList.AddElement(shl.GetSearchKey(sharedPlHd[j]),sharedPlHd[j]);
						}
					}
				}
			NEXTI:
				;
			}
			todoList.DeleteBySwapping(0);
		}
		return YSOK;
	}
	else
	{
		YsErrOut("YsShellMakeConnectedPolygonList()\n");
		YsErrOut("  This function needs search table.\n");
	}
	return YSERR;
}



static YSRESULT YsCheckInitialState
    (int &state,YsShellVertexHandle edVtHd[2],YsVec2 &prjPos,
     int nPlVt,const YsVec2 prjPlg[],const YsShellVertexHandle plVtHd[]);

YSRESULT YsSlidePointOnShell
    (YsVec3 &newPos,YsVec3 &lastDir,YsShellPolygonHandle &newPlHd,
     YSBOOL &reachedDeadEnd,
     YsArray <YsVec3,16> *pathPnt,YsArray <YsShellPolygonHandle,16> *pathPlHd,
     const YsShell &shl,const YsVec3 &oldPos,const YsVec3 &displacement,YsShellPolygonHandle oldPlHd)
{
	int i,nPlVt;
	const YsShellVertexHandle *plVtHd;
	YsShellPolygonHandle currentPlHd;
	const YsShellSearchTable *search;
	YsVec3 nom,pos,cen,currentPos;
	YsArray <YsVec2,16> prjPlg;
	YsVec2 prjPos,prjDir;

	search=shl.GetSearchTable();
	if(search==NULL)
	{
		YsErrOut("YsSlidePointOnShell()\n");
		YsErrOut("  Search table is needed for this function.\n");
		return YSERR;
	}

	if(pathPnt!=NULL)
	{
		pathPnt->Set(0,NULL);
	}
	if(pathPlHd!=NULL)
	{
		pathPlHd->Set(0,NULL);
	}

	reachedDeadEnd=YSFALSE;



	// Set up (If the point is outside of the initial polygon, move it to the point on the polygon.)
	currentPlHd=oldPlHd;
	shl.GetVertexListOfPolygon(nPlVt,plVtHd,currentPlHd);
	shl.GetNormalOfPolygon(nom,currentPlHd);
	if(nom==YsOrigin())
	{
		YsErrOut("YsSlidePointOnShell()\n");
		YsErrOut("  Normal vectors of the polygons must be set for this function.\n");
		return YSERR;
	}

	YsMatrix3x3 tfm;
	double h,p;
	p=asin(nom.y());
	h=atan2(-nom.x(),nom.z());
	tfm.RotateZY(-p);
	tfm.RotateXZ(-h);

	prjPlg.Set(nPlVt,NULL);
	cen=YsOrigin();
	for(i=0; i<nPlVt; i++)
	{
		shl.GetVertexPosition(pos,plVtHd[i]);
		cen+=pos;
		prjPlg[i].GetXY(tfm*pos);
	}
	prjPos.GetXY(tfm*oldPos);
	cen/=(double)nPlVt;
	cen=tfm*cen;


	int state;   // 0:Inside the polygon    1:On an edge of the polygon    2:On a vertex of the polygon
	YsShellVertexHandle currentEdVtHd[2];
	if(YsCheckInitialState(state,currentEdVtHd,prjPos,nPlVt,prjPlg,plVtHd)!=YSOK)
	{
		YsErrOut("YsSlidePointOnShell()\n");
		YsErrOut("  Cannot calculate the initial state.\n");
		return YSERR;
	}

	currentPos.SetXY(prjPos);
	currentPos.SetZ(cen.z());
	tfm.MulInverse(currentPos,currentPos);

	if(pathPnt!=NULL)
	{
		pathPnt->Append(currentPos);
	}
	if(pathPlHd!=NULL)
	{
		pathPlHd->Append(currentPlHd);
	}



	// Move until it runs out of distance.
	double distRemain;
	YsVec3 currentDir;
	YSBOOL first;

	distRemain=displacement.GetLength();
	currentDir=displacement;
	tfm.Mul(currentDir,currentDir);
	currentDir.SetZ(0.0);
	tfm.MulInverse(currentDir,currentDir);
	currentPos=oldPos;

	first=YSTRUE;

	while(distRemain>YsTolerance)
	{
		YsShellVertexHandle nextEdVtHd[2];
		double nextEdParam=0.0,moveDist;

		shl.GetVertexListOfPolygon(nPlVt,plVtHd,currentPlHd);
		shl.GetNormalOfPolygon(nom,currentPlHd);
		if(nom==YsOrigin())
		{
			YsErrOut("YsSlidePointOnShell() (initial)\n");
			YsErrOut("  Normal vectors of the polygons must be set for this function.\n");
			return YSERR;
		}

		YsMatrix3x3 tfm;
		double h,p;
		p=asin(nom.y());
		h=atan2(-nom.x(),nom.z());
		tfm.RotateZY(-p);
		tfm.RotateXZ(-h);

		prjPlg.Set(nPlVt,NULL);
		for(i=0; i<nPlVt; i++)
		{
			shl.GetVertexPosition(pos,plVtHd[i]);
			prjPlg[i].GetXY(tfm*pos);
		}
		prjPos.GetXY(tfm*currentPos);
		prjPlg.Append(prjPlg[0]);


		// currentDir=tfm*currentDir;              Let's assume nom is already normalized.
		// currentDir.SetZ(0.0);
		// prjDir.GetXY(currentDir);
		// tfm.MulInverse(currentDir,currentDir);
		prjDir.GetXY(tfm*currentDir);
		currentDir=currentDir-nom*(nom*currentDir);


		moveDist=YsSqr(YsInfinity);
		nextEdVtHd[0]=NULL;
		nextEdVtHd[1]=NULL;

		for(i=0; i<nPlVt; i++)
		{
			YsVec2 edVtPos[2],np;

			if(state==1)
			{
				if((plVtHd[i]==currentEdVtHd[0] && plVtHd[(i+1)%nPlVt]==currentEdVtHd[1]) ||
				   (plVtHd[i]==currentEdVtHd[1] && plVtHd[(i+1)%nPlVt]==currentEdVtHd[0]))
				{
					goto NEXTEDGE;
				}
			}
			else if(state==2)
			{
				if(plVtHd[i]==currentEdVtHd[0] || plVtHd[(i+1)%nPlVt]==currentEdVtHd[0])
				{
					goto NEXTEDGE;
				}
			}


			edVtPos[0]=prjPlg[i];
			edVtPos[1]=prjPlg[i+1];

			if(prjDir*(edVtPos[0]-prjPos)>=0.0 &&
			   YsGetNearestPointOnLine2(np,prjPos,prjPos+prjDir,edVtPos[0])==YSOK &&
			   np==edVtPos[0] &&
			   (prjPos-np).GetSquareLength()<moveDist)
			{
				nextEdVtHd[0]=plVtHd[i];
				nextEdVtHd[1]=plVtHd[i];
				moveDist=(prjPos-np).GetSquareLength();
				nextEdParam=0.0;
			}
			else if(prjDir*(edVtPos[1]-prjPos)>=0.0 &&
			        YsGetNearestPointOnLine2(np,prjPos,prjPos+prjDir,edVtPos[1])==YSOK &&
			        np==edVtPos[1] &&
			        (prjPos-np).GetSquareLength()<moveDist)
			{
				nextEdVtHd[0]=plVtHd[(i+1)%nPlVt];
				nextEdVtHd[1]=plVtHd[(i+1)%nPlVt];
				moveDist=(prjPos-np).GetSquareLength();
				nextEdParam=0.0;
			}
			else if(YsGetLineIntersection2(np,prjPos,prjPos+prjDir,edVtPos[0],edVtPos[1])==YSOK &&
			        prjDir*(np-prjPos)>=0.0 &&
			        YsCheckInBetween2(np,edVtPos[0],edVtPos[1])==YSTRUE &&
			        (prjPos-np).GetSquareLength()<moveDist)
			{
				if(np==edVtPos[0])
				{
					nextEdVtHd[0]=plVtHd[i];
					nextEdVtHd[1]=plVtHd[i];
					moveDist=(prjPos-np).GetSquareLength();
					nextEdParam=0.0;
				}
				else if(np==edVtPos[1])
				{
					nextEdVtHd[0]=plVtHd[(i+1)%nPlVt];
					nextEdVtHd[1]=plVtHd[(i+1)%nPlVt];
					moveDist=(prjPos-np).GetSquareLength();
					nextEdParam=0.0;
				}
				else
				{
					double d1,d2;
					nextEdVtHd[0]=plVtHd[i];
					nextEdVtHd[1]=plVtHd[(i+1)%nPlVt];
					moveDist=(prjPos-np).GetSquareLength();

					d1=(np-edVtPos[0]).GetLength();
					d2=(edVtPos[1]-edVtPos[0]).GetLength();
					nextEdParam=d1/d2;
				}
			}

		NEXTEDGE:
			;
		}
		moveDist=sqrt(moveDist);

		// Reached the end?
		if(first==YSTRUE && nextEdVtHd[0]==NULL && (state==1 || state==2))
		{
			// Only at the first time, it allow zero movement for tolerating
			// going-out initial condition.  (Like when a vertex is on an edge and
			// the moving direction is outbound.)

			if(state==1)
			{
				YsVec3 p1,p2;
				shl.GetVertexPosition(p1,currentEdVtHd[0]);
				shl.GetVertexPosition(p2,currentEdVtHd[1]);
				moveDist=0.0;
				nextEdVtHd[0]=currentEdVtHd[0];
				nextEdVtHd[1]=currentEdVtHd[1];
				nextEdParam=(currentPos-p1).GetLength()/(p2-p1).GetLength();
			}
			else if(state==2)
			{
				moveDist=0.0;
				nextEdVtHd[0]=currentEdVtHd[0];
				nextEdVtHd[1]=currentEdVtHd[1];
				nextEdParam=0.0;
			}

			if(pathPnt!=NULL)  // 2004/09/23 
			{
				pathPnt->Set(0,NULL);
			}
			if(pathPlHd!=NULL)
			{
				pathPlHd->Set(0,NULL);
			}
		}
		else if(nextEdVtHd[0]!=NULL && distRemain<moveDist)
		{
			currentDir.Normalize();
			currentPos+=currentDir*distRemain;
			distRemain=0.0;
			break;
		}
		else
		{
			distRemain-=moveDist;
		}

		// Transition to the next polygon
		YsShellPolygonHandle prevPlHd;
		prevPlHd=currentPlHd;
		if(nextEdVtHd[0]==NULL)
		{
			reachedDeadEnd=YSTRUE;
			break;
		}
		else if(nextEdVtHd[0]!=nextEdVtHd[1]) // Moving across an edge
		{
			YsVec3 p1,p2,nextNom;
			int nFoundPlHd;
			const YsShellPolygonHandle *foundPlHd;

			shl.GetVertexPosition(p1,nextEdVtHd[0]);
			shl.GetVertexPosition(p2,nextEdVtHd[1]);
			currentPos=p1*(1.0-nextEdParam)+p2*nextEdParam;
			currentEdVtHd[0]=nextEdVtHd[0];
			currentEdVtHd[1]=nextEdVtHd[1];

			if(search->FindPolygonListByEdge(nFoundPlHd,foundPlHd,shl,nextEdVtHd[0],nextEdVtHd[1])==YSOK &&
			   nFoundPlHd==2)
			{
				if(foundPlHd[0]==currentPlHd)
				{
					currentPlHd=foundPlHd[1];
				}
				else
				{
					currentPlHd=foundPlHd[0];
				}
			}
			else
			{
				// Non-manifold edge.  Cannot go any farther.  Just terminate.
				reachedDeadEnd=YSTRUE;
				break;
			}

			shl.GetNormalOfPolygon(nextNom,currentPlHd);
			if(nextNom==YsOrigin())
			{
				YsErrOut("YsSlidePointOnShell() (transition across an edge)\n");
				YsErrOut("  Normal vectors of the polygons must be set for this function.\n");
				return YSERR;
			}
			YsRotation rotation;
			YsVec3 rotAxis;
			double rotAngle;
			rotAxis=nom^nextNom;
			rotAngle=asin(rotAxis.GetLength());
			if(rotAxis.Normalize()==YSOK)  // Otherwise, no rotation is needed.
			{
				rotation.Set(rotAxis,rotAngle);
				rotation.RotatePositive(currentDir,currentDir);
			}

			state=1;
		}
		else // Moving across a vertex
		{
			int nFoundPlHd;
			const YsShellPolygonHandle *foundPlHd;
			int nPlVt;
			const YsShellVertexHandle *plVtHd;
			YsVec3 nextNom,nextDir;
			YsShellPolygonHandle nextPlHd;

			nextPlHd=NULL;

			shl.GetVertexPosition(currentPos,nextEdVtHd[0]);
			currentEdVtHd[0]=nextEdVtHd[0];
			currentEdVtHd[1]=nextEdVtHd[1];

			if(search->FindPolygonListByVertex(nFoundPlHd,foundPlHd,shl,nextEdVtHd[0])==YSOK && nFoundPlHd>0)
			{
				int i,j;
				for(i=0; i<nFoundPlHd && nextPlHd==NULL; i++)
				{
					if(foundPlHd[i]!=currentPlHd)
					{
						shl.GetNormalOfPolygon(nextNom,foundPlHd[i]);
						if(nextNom==YsOrigin())
						{
							YsErrOut("YsSlidePointOnShell() (transition across a vertex)\n");
							YsErrOut("  Normal vectors of the polygons must be set for this function.\n");
							return YSERR;
						}

						YsRotation rotation;
						YsVec3 rotAxis;
						double rotAngle;
						rotAxis=nom^nextNom;
						rotAngle=asin(rotAxis.GetLength());
						if(rotAxis.Normalize()==YSOK)  // Otherwise, no rotation is needed.
						{
							rotation.Set(rotAxis,rotAngle);
							rotation.RotatePositive(nextDir,currentDir);
						}
						else
						{
							nextDir=currentDir;
						}

						YsArray <YsVec3,4> plVtPos;
						nPlVt=shl.GetNumVertexOfPolygon(foundPlHd[i]);
						plVtHd=shl.GetVertexListOfPolygon(foundPlHd[i]);
						shl.GetVertexListOfPolygon(plVtPos,foundPlHd[i]);
						for(j=0; j<nPlVt; j++)
						{
							if(plVtHd[j]==nextEdVtHd[0])
							{
								if(YsVectorPointingInside(plVtPos.GetN(),plVtPos,nextNom,j,nextDir)==YSTRUE)
								{
									nextPlHd=foundPlHd[i];
								}
								break;
							}
						}
					}
				}
			}


			if(nextPlHd==NULL) // 2004/10/25
			{
				// Think about the possibility that the point moved across a vertex, and landed on an edge.
				YsArray <YsShellVertexHandle,16> connVtHd;
				if(search->GetConnectedVertexList(connVtHd,shl,nextEdVtHd[0])==YSOK)
				{
					int i;
					YsVec3 edVtPos[2],edVec,vecMax;
					YsShellVertexHandle toVtHd;
					double dotMax;

					shl.GetVertexPosition(edVtPos[0],nextEdVtHd[0]);
					toVtHd=NULL;
					dotMax=0.707107;  // cos(45deg)
					for(i=0; i<connVtHd.GetN(); i++)
					{
						shl.GetVertexPosition(edVtPos[1],connVtHd[i]);
						edVec=edVtPos[1]-edVtPos[0];
						if(edVec*currentDir>YsTolerance)
						{
							edVec.Normalize();
							if(edVec*currentDir>dotMax)
							{
								toVtHd=connVtHd[i];
								dotMax=edVec*currentDir;
								vecMax=edVec;
							}
						}
					}
					if(toVtHd!=NULL)
					{
						int nFoundPlHd;
						const YsShellPolygonHandle *foundPlHd;

						if(search->FindPolygonListByEdge(nFoundPlHd,foundPlHd,shl,nextEdVtHd[0],toVtHd)==YSOK &&
						   nFoundPlHd>0)
						{
							nextPlHd=foundPlHd[0];
						}
					}
				}
			}


			if(nextPlHd==NULL)
			{
				reachedDeadEnd=YSTRUE;
				break;
			}
			else
			{
				currentPlHd=nextPlHd;
				currentDir=nextDir;
			}

			state=2;
		}



		if(moveDist<YsTolerance && currentPlHd==prevPlHd)
		{
			reachedDeadEnd=YSTRUE;
			break;
		}



		if(pathPnt!=NULL)
		{
			pathPnt->Append(currentPos);
		}
		if(pathPlHd!=NULL)
		{
			pathPlHd->Append(currentPlHd);
		}

		first=YSFALSE;
	}

	if(pathPnt!=NULL)
	{
		pathPnt->Append(currentPos);
	}
	if(pathPlHd!=NULL)
	{
		pathPlHd->Append(currentPlHd);
	}

	newPos=currentPos;
	lastDir=currentDir;
	newPlHd=currentPlHd;
	return YSOK;
}

static YSRESULT YsCheckInitialState(
    int &state,YsShellVertexHandle edVtHd[2],YsVec2 &prjPos,
    int nPlVt,const YsVec2 prjPlg[],const YsShellVertexHandle plVtHd[])
{
	if(YsCheckInsidePolygon2(prjPos,nPlVt,prjPlg)==YSINSIDE)
	{
		state=0;
		return YSOK;
	}
	else
	{
		int i,idxEdge,idxVtx;
		double distEdge,distVtx;
		YsVec2 np;

		distEdge=YsSqr(YsInfinity);
		idxEdge=-1;
		distVtx=YsSqr(YsInfinity);
		idxVtx=-1;

		for(i=0; i<nPlVt; i++)
		{
			double d;
			d=(prjPlg[i]-prjPos).GetSquareLength();
			if(d<distVtx)
			{
				distVtx=d;
				idxVtx=i;
			}
			if(YsGetNearestPointOnLine2(np,prjPlg[i],prjPlg[(i+1)%nPlVt],prjPos)==YSOK &&
			   (d=(np-prjPos).GetSquareLength())<distEdge &&
			   YsCheckInBetween2(np,prjPlg[i],prjPlg[(i+1)%nPlVt])==YSTRUE) // 2005/10/03
			{
				distEdge=d;
				idxEdge=i;
			}
		}

		// 2008/12/08 Changed the order of the check.
		//   Old:  On-edge condition was checked before on-vertex condition.
		//   New:  On-vertex condition is checked first.
		//   Why?  When vertex-to-iniPos distance is less than tolerance, it should be on-vertex initial condition.
		if((distVtx<distEdge || distVtx<YsToleranceSqr) && idxVtx>=0)
		{
			prjPos=prjPlg[idxVtx];
			edVtHd[0]=plVtHd[idxVtx];
			edVtHd[1]=plVtHd[idxVtx];
			state=2;
			return YSOK;
		}
		else if(distEdge<distVtx && idxEdge>=0)
		{
			YsGetNearestPointOnLine2(prjPos,prjPlg[idxEdge],prjPlg[(idxEdge+1)%nPlVt],prjPos);
			edVtHd[0]=plVtHd[idxEdge];
			edVtHd[1]=plVtHd[(idxEdge+1)%nPlVt];
			state=1;
			return YSOK;
		}

		return YSERR;
	}
}

YSBOOL YsVectorPointingInside(YSSIZE_T np,const YsVec3 plg[],const YsVec3 &nom,int fromIdx,const YsVec3 &dir)
{
	YSSIZE_T i;
	YsArray <YsVec2,16> prjPlg;
	YsVec2 prjDir;

	YsMatrix3x3 tfm;
	double h,p;
	p=asin(nom.y());
	h=atan2(-nom.x(),nom.z());
	tfm.RotateZY(-p);
	tfm.RotateXZ(-h);

	prjPlg.Set(np,NULL);
	for(i=0; i<np; i++)
	{
		prjPlg[i].GetXY(tfm*plg[i]);
	}
	prjPlg.Append(prjPlg[0]);
	prjDir.GetXY(tfm*dir);


	YSBOOL found;
	double nearDist;
	YsVec2 nearItsc,from,tst;
	nearDist=YsSqr(YsInfinity);
	from=prjPlg[fromIdx];
	found=YSFALSE;
	for(i=0; i<np; i++)
	{
		if(i!=fromIdx && (i+1)%np!=fromIdx)
		{
			if(YsGetNearestPointOnLine2(tst,from,from+prjDir,prjPlg[i])==YSOK &&
			   tst==prjPlg[i] &&
			   (tst-from)*prjDir>=0 &&
			   (tst-from).GetSquareLength()<nearDist)
			{
				nearItsc=tst;
				nearDist=(tst-from).GetSquareLength();
				found=YSTRUE;
			}
			else if(YsGetLineIntersection2(tst,from,from+prjDir,prjPlg[i],prjPlg[i+1])==YSOK &&
			        YsCheckInBetween2(tst,prjPlg[i],prjPlg[i+1])==YSTRUE &&
			        (tst-from)*prjDir>=0.0 &&
			        (tst-from).GetSquareLength()<nearDist)
			{
				nearItsc=tst;
				nearDist=(tst-from).GetSquareLength();
				found=YSTRUE;
			}
		}
	}

	if(found==YSTRUE)
	{
		if(YsCheckInsidePolygon2((nearItsc+from)/2.0,prjPlg.GetN(),prjPlg)==YSINSIDE)
		{
			return YSTRUE;
		}
	}

	return YSFALSE;
}

YSRESULT YsShellVectorFunction::GetVectorOnPolygon(YsVec3 &,const YsShell &,const YsShellPolygonHandle,const YsVec3 &) const
{
	return YSERR;
}

YSRESULT YsShellVectorFunction::GetVectorOnVertex(YsVec3 &,const YsShell &,const YsShellVertexHandle) const
{
	return YSERR;
}

YSBOOL YsShellCrawlStopperFunction::IsStopperVertex(const YsShell &,YsShellVertexHandle) const
{
	return YSFALSE;
}

YSBOOL YsShellCrawlStopperFunction::IsStopperEdge(const YsShell &,const YsShellVertexHandle [2]) const
{
	return YSFALSE;
}

YSBOOL YsShellCrawlStopperFunction::IsStopperPolygon(const YsShell &,const YsShellPolygonHandle) const
{
	return YSFALSE;
}

YsShellCrawler::YsShellCrawler()
{
	crawlDirFunc=NULL;
	crawlStopFunc=NULL;

	crawlingMode=0;  // Follow the curvature
	goal=YsOrigin();
	constPlane.Set(YsOrigin(),YsZVec());

	Initialize();
}

void YsShellCrawler::Initialize(void)
{
	currentPlHd=NULL;
	currentPos=YsOrigin();
	currentDir=YsOrigin();

	currentState=0;
	currentEdVtHd[0]=NULL;
	currentEdVtHd[1]=NULL;

	reachedDeadEnd=YSFALSE;

	pathPnt.Set(0,NULL);
	pathPlHd.Set(0,NULL);

	firstStep=YSTRUE;
}

// Memo: Even when crawlDirFunc is given, startDir is still needed because crawlDirFunc
//       may give a reverse direction from the intended direction.
YSRESULT YsShellCrawler::Start(
    const YsShell &shl,const YsVec3 &startPos,const YsVec3 &startDir,YsShellPolygonHandle startPlHd)
{
	int i,nPlVt;
	const YsShellVertexHandle *plVtHd;
	const YsShellSearchTable *search;
	YsVec3 nom,pos,cen;

	Initialize();

	search=shl.GetSearchTable();
	if(search==NULL)
	{
		YsErrOut("YsShellCrawler\n");
		YsErrOut("  Search table is needed for this function.\n");
		return YSERR;
	}

	pathPnt.Set(0,NULL);
	pathPlHd.Set(0,NULL);

	reachedDeadEnd=YSFALSE;


	// Set up (If the point is outside of the initial polygon, move it to the point on the polygon.)
	currentPlHd=startPlHd;
	shl.GetVertexListOfPolygon(nPlVt,plVtHd,currentPlHd);
	shl.GetNormalOfPolygon(nom,currentPlHd);
	if(nom==YsOrigin())
	{
		YsErrOut("YsShellCrawler\n");
		YsErrOut("  Normal vectors of the polygons must be set for this function.\n");
		return YSERR;
	}

	YsMatrix3x3 tfm(YSFALSE),tfmInv(YSFALSE);
//	double h,p;
//	tfm.Initialize();
//	p=asin(nom.y());
//	h=atan2(-nom.x(),nom.z());
//	tfm.RotateZY(-p);
//	tfm.RotateXZ(-h);
//
//	tfmInv=tfm;         // 2007/01/16
//	tfmInv.Transpose();

	// 2007/01/16 >> Equivalent
	YsVec3 a1,a2;
	a1=nom.GetArbitraryPerpendicularVector();
	a1.Normalize();
	a2=a1^nom;
	tfmInv.Set(1,3,nom.x());
	tfmInv.Set(2,3,nom.y());
	tfmInv.Set(3,3,nom.z());
	tfmInv.Set(1,2,a2.x());
	tfmInv.Set(2,2,a2.y());
	tfmInv.Set(3,2,a2.z());
	tfmInv.Set(1,1,a1.x());
	tfmInv.Set(2,1,a1.y());
	tfmInv.Set(3,1,a1.z());

	tfm=tfmInv;
	tfm.Transpose();
	// 2007/01/16 << Equivalent



	prjPlg.Set(nPlVt,NULL);
	cen=YsOrigin();
	for(i=0; i<nPlVt; i++)
	{
		shl.GetVertexPosition(pos,plVtHd[i]);
		cen+=pos;
		// prjPlg[i].GetXY(tfm*pos);
		prjPlg[i].SetX(tfm.v(1,1)*pos.x()+tfm.v(1,2)*pos.y()+tfm.v(1,3)*pos.z());
		prjPlg[i].SetY(tfm.v(2,1)*pos.x()+tfm.v(2,2)*pos.y()+tfm.v(2,3)*pos.z());
	}
	prjPos.GetXY(tfm*startPos);
	cen/=(double)nPlVt;
	cen=tfm*cen;


	if(YsCheckInitialState(currentState,currentEdVtHd,prjPos,nPlVt,prjPlg,plVtHd)!=YSOK)
	{
		YsErrOut("YsShellCrawler\n");
		YsErrOut("  Cannot calculate the initial state.\n");
		return YSERR;
	}

	currentPos.SetXY(prjPos);
	currentPos.SetZ(cen.z());
	tfmInv.Mul(currentPos,currentPos);


	if(crawlingMode==2)
	{
		YsVec3 ref;
		ref=nom^constPlane.GetNormal();
		if(ref.Normalize()!=YSOK)
		{
			return YSERR;
		}
		if(ref*startDir>0.0)
		{
			currentDir=ref;
		}
		else
		{
			currentDir=-ref;
		}
	}
	else
	{
		tfm.Mul(currentDir,startDir);
		currentDir.SetZ(0.0);
		tfmInv.Mul(currentDir,currentDir);
	}


	pathPnt.Append(currentPos);
	pathPlHd.Append(currentPlHd);

	firstStep=YSTRUE;

	return YSOK;
}

YSRESULT YsShellCrawler::Crawl(const YsShell &shl,const double &dist,YSBOOL watch)
{
	int i,nPlVt;
	const YsShellVertexHandle *plVtHd;
	const YsShellSearchTable *search;

	double noImproveCtr; // Used when crawlingMode==1 (Head-to-goal mode)
	double prevGoalDist; // Used when crawlingMode==1 (Head-to-goal mode)

	noImproveCtr=0;
	prevGoalDist=0.0;

	search=shl.GetSearchTable();
	if(search==NULL)
	{
		YsErrOut("YsShellCrawler\n");
		YsErrOut("  Search table is needed for this function.\n");
		return YSERR;
	}


	YsVec3 nom,pos,cen;


	// Move until it runs out of distance.
	double distRemain;
	if(fabs(dist)>YsTolerance)
	{
		distRemain=dist;
	}
	else
	{
		distRemain=YsInfinity;
	}

	if(watch==YSTRUE)
	{
		printf("Current State %d\n",currentState);
		printf("Current Pl Key %d\n",shl.GetSearchKey(currentPlHd));
		printf("Current Pos %s\n",currentPos.Txt());
		printf("Current Dir %s\n",currentDir.Txt());
	}


	if(crawlingMode==1)
	{
		prevGoalDist=(goal-currentPos).GetLength();
	}

	while(distRemain>YsTolerance)
	{
		YsShellVertexHandle nextEdVtHd[2];
		double nextEdParam,moveDist;

		if(watch==YSTRUE)
		{
			printf("Dist Remain %lf  State %d  PlId %d\n",
			    distRemain,
			    currentState,
			    shl.GetPolygonIdFromHandle(currentPlHd));
		}

		shl.GetVertexListOfPolygon(nPlVt,plVtHd,currentPlHd);
		shl.GetNormalOfPolygon(nom,currentPlHd);
		if(nom==YsOrigin())
		{
			YsErrOut("YsShellCrawler() (initial)\n");
			YsErrOut("  Normal vectors of the polygons must be set for this function.\n");
			return YSERR;
		}


		CrawlCalculateProjectedPolygonAndDirection(shl,prjDir,prjPlg,nPlVt,plVtHd,nom);


		if(watch==YSTRUE)
		{
			YsPrintf("Dir %s (%s)\n",currentDir.Txt(),(currentDir-nom*(nom*currentDir)).Txt());
		}
		currentDir=currentDir-nom*(nom*currentDir);



		moveDist=YsSqr(YsInfinity);
		nextEdVtHd[0]=NULL;
		nextEdVtHd[1]=NULL;

		for(i=0; i<nPlVt; i++)
		{
			YsShellVertexHandle tstEdVtHd[2];
			tstEdVtHd[0]=plVtHd[i];
			tstEdVtHd[1]=plVtHd[(i+1)%nPlVt];

			if(currentState==1)
			{
				if(YsSameEdge(tstEdVtHd[0],tstEdVtHd[1],currentEdVtHd[0],currentEdVtHd[1])==YSTRUE)
				{
					goto NEXTEDGE;
				}
			}
			else if(currentState==2)
			{
				if(tstEdVtHd[0]==currentEdVtHd[0] || tstEdVtHd[1]==currentEdVtHd[0])
				{
					goto NEXTEDGE;
				}
			}

			if(crawlingMode!=2)
			{
				YsVec2 np;
				const YsVec2 &edVtPos0=prjPlg[i];    // edVtPos[0]=prjPlg[i];
				const YsVec2 &edVtPos1=prjPlg[i+1];  // edVtPos[1]=prjPlg[i+1];
				double dist;

				if(prjDir*(edVtPos0-prjPos)>=0.0 &&
				   YsGetNearestPointOnLine2(np,prjPos,prjPos+prjDir,edVtPos0)==YSOK &&
				   np==edVtPos0 &&
				   (dist=(prjPos-np).GetSquareLength())<moveDist)
				{
					if(watch==YSTRUE)
					{
						printf("Vertex %d \n",shl.GetVertexIdFromHandle(tstEdVtHd[0]));
					}

					nextEdVtHd[0]=tstEdVtHd[0];
					nextEdVtHd[1]=tstEdVtHd[0];
					moveDist=dist;
					nextEdParam=0.0;
				}
				else if(prjDir*(edVtPos1-prjPos)>=0.0 &&
				        YsGetNearestPointOnLine2(np,prjPos,prjPos+prjDir,edVtPos1)==YSOK &&
				        np==edVtPos1 &&
				        (dist=(prjPos-np).GetSquareLength())<moveDist)
				{
					if(watch==YSTRUE)
					{
						printf("Vertex %d \n",shl.GetVertexIdFromHandle(tstEdVtHd[1]));
					}

					nextEdVtHd[0]=tstEdVtHd[1];
					nextEdVtHd[1]=tstEdVtHd[1];
					moveDist=dist;
					nextEdParam=0.0;
				}
				else if(YsGetLineIntersection2(np,prjPos,prjPos+prjDir,edVtPos0,edVtPos1)==YSOK &&
				        prjDir*(np-prjPos)>=0.0 &&
				        YsCheckInBetween2(np,edVtPos0,edVtPos1)==YSTRUE &&
				        (dist=(prjPos-np).GetSquareLength())<moveDist)
				{
					if(watch==YSTRUE)
					{
						printf("Edge %d %d\n",
						    shl.GetVertexIdFromHandle(tstEdVtHd[0]),
						    shl.GetVertexIdFromHandle(tstEdVtHd[1]));
					}

					if(np==edVtPos0)
					{
						nextEdVtHd[0]=tstEdVtHd[0];
						nextEdVtHd[1]=tstEdVtHd[0];
						moveDist=dist;
						nextEdParam=0.0;
					}
					else if(np==edVtPos1)
					{
						nextEdVtHd[0]=tstEdVtHd[1];
						nextEdVtHd[1]=tstEdVtHd[1];
						moveDist=dist;
						nextEdParam=0.0;
					}
					else
					{
						double d1,d2;
						nextEdVtHd[0]=tstEdVtHd[0];
						nextEdVtHd[1]=tstEdVtHd[1];
						moveDist=dist;

						d1=(np-edVtPos0).GetLength();
						d2=(edVtPos1-edVtPos0).GetLength();
						nextEdParam=d1/d2;
					}
				}
			}
			else // if(crawlingMode==2)
			{
				YsVec3 edVtPos[2],np;
				double dist;

				shl.GetVertexPosition(edVtPos[0],tstEdVtHd[0]);
				shl.GetVertexPosition(edVtPos[1],tstEdVtHd[1]);

				if(watch==YSTRUE)
				{
					printf("%d\n",constPlane.GetIntersection(np,edVtPos[0],edVtPos[1]-edVtPos[0]));
					printf("%s %s\n",edVtPos[0].Txt(),edVtPos[1].Txt());
					printf("%s\n",np.Txt());
					printf("%lf\n",currentDir*(np-currentPos));
					printf("%d\n",YsCheckInBetween3(np,edVtPos[0],edVtPos[1]));
					printf("%lf\n",(dist=(currentPos-np).GetSquareLength()));
				}

				if(currentDir*(edVtPos[0]-currentPos)>=0.0 &&
				   constPlane.GetSideOfPlane(edVtPos[0])==0 &&
				   (dist=(currentPos-edVtPos[0]).GetSquareLength())<moveDist)
				{
					if(watch==YSTRUE)
					{
						printf("Vertex %d \n",shl.GetVertexIdFromHandle(tstEdVtHd[0]));
					}

					nextEdVtHd[0]=tstEdVtHd[0];
					nextEdVtHd[1]=tstEdVtHd[0];
					moveDist=dist;
					nextEdParam=0.0;
				}
				else if(currentDir*(edVtPos[1]-currentPos)>=0.0 &&
				        constPlane.GetSideOfPlane(edVtPos[1])==0 &&
				        (dist=(currentPos-edVtPos[1]).GetSquareLength())<moveDist)
				{
					if(watch==YSTRUE)
					{
						printf("Vertex %d \n",shl.GetVertexIdFromHandle(tstEdVtHd[1]));
					}

					nextEdVtHd[0]=tstEdVtHd[1];
					nextEdVtHd[1]=tstEdVtHd[1];
					moveDist=dist;
					nextEdParam=0.0;
				}
				else if(constPlane.GetIntersection(np,edVtPos[0],edVtPos[1]-edVtPos[0])==YSOK &&
				        currentDir*(np-currentPos)>=0.0 &&
				        YsCheckInBetween3(np,edVtPos[0],edVtPos[1])==YSTRUE &&
				        (dist=(currentPos-np).GetSquareLength())<moveDist)
				{
					if(np==edVtPos[0])
					{
						nextEdVtHd[0]=tstEdVtHd[0];
						nextEdVtHd[1]=tstEdVtHd[0];
						moveDist=dist;
						nextEdParam=0.0;
					}
					else if(np==edVtPos[1])
					{
						nextEdVtHd[0]=tstEdVtHd[1];
						nextEdVtHd[1]=tstEdVtHd[1];
						moveDist=dist;
						nextEdParam=0.0;
					}
					else
					{
						double d1,d2;
						nextEdVtHd[0]=tstEdVtHd[0];
						nextEdVtHd[1]=tstEdVtHd[1];
						moveDist=dist;

						d1=(np-edVtPos[0]).GetLength();
						d2=(edVtPos[1]-edVtPos[0]).GetLength();
						nextEdParam=d1/d2;
					}
				}
			}

		NEXTEDGE:
			if(watch==YSTRUE)
			{
				printf("NEXTEDGE\n");
			}
		}
		moveDist=sqrt(moveDist);

		if(watch==YSTRUE)
		{
			printf("Moved %lf %d %d\n",moveDist,shl.GetSearchKey(nextEdVtHd[0]),shl.GetSearchKey(nextEdVtHd[1]));
		}

		// Reached the end?
		if(firstStep==YSTRUE && nextEdVtHd[0]==NULL && (currentState==1 || currentState==2))
		{
			// Only at the first time, it allow zero movement for tolerating
			// going-out initial condition.  (Like when a vertex is on an edge and
			// the moving direction is outbound.)

			if(watch==YSTRUE)
			{
				printf("First step\n");
			}

			if(currentState==1)
			{
				YsVec3 p1,p2;
				shl.GetVertexPosition(p1,currentEdVtHd[0]);
				shl.GetVertexPosition(p2,currentEdVtHd[1]);
				moveDist=0.0;
				nextEdVtHd[0]=currentEdVtHd[0];
				nextEdVtHd[1]=currentEdVtHd[1];
				nextEdParam=(currentPos-p1).GetLength()/(p2-p1).GetLength();
			}
			else if(currentState==2)
			{
				moveDist=0.0;
				nextEdVtHd[0]=currentEdVtHd[0];
				nextEdVtHd[1]=currentEdVtHd[1];
				nextEdParam=0.0;
			}

			pathPnt.Set(0,NULL);  // 2004/09/23 
			pathPlHd.Set(0,NULL);  // 2004/09/23 
		}
		else
		{
			if(nextEdVtHd[0]!=NULL && distRemain<moveDist)
			{
				if(watch==YSTRUE)
				{
					printf("Run out of distance\n");
				}

				currentDir.Normalize();
				currentPos+=currentDir*distRemain;
				currentState=0;
				distRemain=0.0;
				break;
			}
			else if(nextEdVtHd[0]!=NULL && crawlingMode==1)  // This if statement has been added 2008/05/01
			{
				double prjGoalDist;
				currentDir.Normalize();
				prjGoalDist=(goal-currentPos)*currentDir;
				if(prjGoalDist<moveDist)
				{
					if(watch==YSTRUE)
					{
						printf("Passed near point of the goal.\n");
					}

					currentPos+=currentDir*prjGoalDist;
					currentState=0;
					distRemain=0.0;
					break;
				}
			}

			distRemain-=moveDist;
		}

		// Transition to the next polygon
		YsShellPolygonHandle prevPlHd;
		prevPlHd=currentPlHd;
		if(nextEdVtHd[0]==NULL)
		{
			reachedDeadEnd=YSTRUE;
			break;
		}
		else if(nextEdVtHd[0]!=nextEdVtHd[1]) // Moving across an edge
		{
			if(MoveAcrossEdge(shl,search,nom,nextEdVtHd,nextEdParam)!=YSOK)
			{
				if(reachedDeadEnd==YSTRUE)
				{
					break;
				}
				else
				{
					return YSERR;
				}
			}
		}
		else // Moving across a vertex
		{
			if(MoveAcrossVertex(shl,search,nom,nextEdVtHd[0])!=YSOK)
			{
				if(reachedDeadEnd==YSTRUE)
				{
					break;
				}
				else
				{
					return YSERR;
				}
			}
		}

		if(crawlingMode==1)  // 2008/05/01
		{
			if(currentPos==goal)
			{
				break;
			}

			double goalDist;
			goalDist=(currentPos-goal).GetLength();
			if(prevGoalDist-goalDist<YsTolerance)
			{
				noImproveCtr++;
				if(noImproveCtr>=2)
				{
					reachedDeadEnd=YSTRUE;
					break;
				}
			}
			else
			{
				prevGoalDist=goalDist;
			}
		}

		if(crawlStopFunc!=NULL && crawlStopFunc->IsStopperPolygon(shl,currentPlHd)==YSTRUE)
		{
			reachedDeadEnd=YSTRUE;
			break;
		}

		if(moveDist<YsTolerance && currentPlHd==prevPlHd)
		{
			reachedDeadEnd=YSTRUE;
			break;
		}

		pathPnt.Append(currentPos);
		pathPlHd.Append(currentPlHd);

		firstStep=YSFALSE;
		if(fabs(dist)<YsTolerance)  // 2005/02/02  For per polygon crawling.
		{
			return YSOK;
		}
	}

	pathPnt.Append(currentPos);
	pathPlHd.Append(currentPlHd);

	return YSOK;
}

double YsShellCrawler::GetTotalLength(void) const
{
	int i;
	double len;
	len=0.0;
	for(i=1; i<pathPnt.GetN(); i++)
	{
		len+=(pathPnt[i]-pathPnt[i-1]).GetLength();
	}
	return len;
}

void YsShellCrawler::CrawlCalculateProjectedPolygonAndDirection(
    const YsShell &shl,
    YsVec2 &prjDir,YsArray <YsVec2,32> &prjPlg,int nPlVt,const YsShellVertexHandle plVtHd[],const YsVec3 &nom)
{
	int i;
	YsVec3 pos;

	// YsMatrix3x3 tfm(YSFALSE);  2009/04/01 Commented Out

	// double h,p;
	// tfm.Initialize()
	// p=asin(nom.y());
	// h=atan2(-nom.x(),nom.z());
	// tfm.RotateZY(-p);
	// tfm.RotateXZ(-h);

	// 2007/01/16 >> Equivalent Computation
	// YsVec3 a1,a2;
	// a1=nom.GetArbitraryPerpendicularVector();
	// a1.Normalize();
	// a2=a1^nom;
	// tfm.Set(1,3,nom.x());
	// tfm.Set(2,3,nom.y());
	// tfm.Set(3,3,nom.z());
	// tfm.Set(1,2,a2.x());
	// tfm.Set(2,2,a2.y());
	// tfm.Set(3,2,a2.z());
	// tfm.Set(1,1,a1.x());
	// tfm.Set(2,1,a1.y());
	// tfm.Set(3,1,a1.z());
	// tfm.Transpose();
	// 2007/01/16 << Equivalent Computation

	// 2009/04/01 >> Equivalent Computation
	YsVec3 a1,a2;
	a1=nom.GetArbitraryPerpendicularVector();
	a1.Normalize();
	a2=a1^nom;
	// tfm.Set(3,1,nom.x()); Irrelevant in this computation
	// tfm.Set(3,2,nom.y()); Irrelevant in this computation
	// tfm.Set(3,3,nom.z()); Irrelevant in this computation
	// tfm.Set(2,1,a2.x());  2009/04/01 Commented Out
	// tfm.Set(2,2,a2.y());  2009/04/01 Commented Out
	// tfm.Set(2,3,a2.z());  2009/04/01 Commented Out
	// tfm.Set(1,1,a1.x());  2009/04/01 Commented Out
	// tfm.Set(1,2,a1.y());  2009/04/01 Commented Out
	// tfm.Set(1,3,a1.z());  2009/04/01 Commented Out
	// 2009/04/01 << Equivalent Computation



	prjPlg.Set(nPlVt+1,NULL);
	for(i=0; i<nPlVt; i++)
	{
		shl.GetVertexPosition(pos,plVtHd[i]);
		// prjPlg[i].GetXY(tfm*pos);

		// Equals...
		// prjPlg[i].SetX(tfm.v(1,1)*pos.x()+tfm.v(1,2)*pos.y()+tfm.v(1,3)*pos.z());
		// prjPlg[i].SetY(tfm.v(2,1)*pos.x()+tfm.v(2,2)*pos.y()+tfm.v(2,3)*pos.z());

		// Equals...
		prjPlg[i].SetX(a1*pos);
		prjPlg[i].SetY(a2*pos);
	}
	prjPlg[nPlVt]=prjPlg[0];


	// prjPos.GetXY(tfm*currentPos);

	// Equals...
	//prjPos.SetX(tfm.v(1,1)*currentPos.x()+tfm.v(1,2)*currentPos.y()+tfm.v(1,3)*currentPos.z());
	//prjPos.SetY(tfm.v(2,1)*currentPos.x()+tfm.v(2,2)*currentPos.y()+tfm.v(2,3)*currentPos.z());

	// Equals...
	prjPos.SetX(a1*currentPos);
	prjPos.SetY(a2*currentPos);



	// currentDir=tfm*currentDir;              Let's assume nom is already normalized.
	// currentDir.SetZ(0.0);
	// prjDir.GetXY(currentDir);
	// tfm.MulInverse(currentDir,currentDir);
	if(crawlDirFunc!=NULL)
	{
		YsVec3 dir;
		if(crawlDirFunc->GetVectorOnPolygon(dir,shl,currentPlHd,currentPos)==YSOK)
		{
			if(currentDir*dir>0.0)
			{
				currentDir=dir;
			}
			else
			{
				currentDir=-dir;
			}
		}
	}

	// prjDir.GetXY(tfm*currentDir);

	// Equals...
	//prjDir.SetX(tfm.v(1,1)*currentDir.x()+tfm.v(1,2)*currentDir.y()+tfm.v(1,3)*currentDir.z());
	//prjDir.SetY(tfm.v(2,1)*currentDir.x()+tfm.v(2,2)*currentDir.y()+tfm.v(2,3)*currentDir.z());

	// Equals...
	prjDir.SetX(a1*currentDir);
	prjDir.SetY(a2*currentDir);
}

YSRESULT YsShellCrawler::MoveAcrossEdge(
    const YsShell &shl,const YsShellSearchTable *search,const YsVec3 &nom,const YsShellVertexHandle nextEdVtHd[2],const double &nextEdParam)
{
	YsVec3 p1,p2,nextNom;
	int nFoundPlHd;
	const YsShellPolygonHandle *foundPlHd;

	if(crawlStopFunc!=NULL && crawlStopFunc->IsStopperEdge(shl,nextEdVtHd)==YSTRUE)
	{
		reachedDeadEnd=YSTRUE;
		return YSERR;
	}


	shl.GetVertexPosition(p1,nextEdVtHd[0]);
	shl.GetVertexPosition(p2,nextEdVtHd[1]);
	currentPos=p1*(1.0-nextEdParam)+p2*nextEdParam;
	currentEdVtHd[0]=nextEdVtHd[0];
	currentEdVtHd[1]=nextEdVtHd[1];

	if(search->FindPolygonListByEdge(nFoundPlHd,foundPlHd,shl,nextEdVtHd[0],nextEdVtHd[1])==YSOK &&
	   nFoundPlHd==2)
	{
		if(foundPlHd[0]==currentPlHd)
		{
			currentPlHd=foundPlHd[1];
		}
		else
		{
			currentPlHd=foundPlHd[0];
		}
	}
	else
	{
		// Non-manifold edge.  Cannot go any farther.  Just terminate.
		reachedDeadEnd=YSTRUE;
		return YSERR;
	}

	shl.GetNormalOfPolygon(nextNom,currentPlHd);
	if(nextNom==YsOrigin())
	{
		YsErrOut("YsShellCrawler::Crawl() (transition across an edge)\n");
		YsErrOut("  Normal vectors of the polygons must be set for this function.\n");
		return YSERR;
	}

	switch(crawlingMode)
	{
	case 0:
		{
			YsVec3 rotAxis;
			double rotAngle;
			rotAxis=nom^nextNom;
			rotAngle=asin(rotAxis.GetLength());
			if(rotAxis.Normalize()==YSOK)  // Otherwise, no rotation is needed.
			{
				YsRotation rotation;
				rotation.SetUnit(rotAxis,rotAngle);
				rotation.RotatePositive(currentDir,currentDir);
			}
		}
		break;
	case 2:  // Crawler on a plane  See research note 2008/07/04
		{
			YsVec3 rotAxis;
			double rotAngle;
			YsVec3 nextDir;

			nextDir=nextNom^constPlane.GetNormal();
			if(nextDir.Normalize()!=YSOK)
			{
				reachedDeadEnd=YSTRUE;
			}
			else
			{
				rotAxis=nom^nextNom;
				rotAngle=asin(rotAxis.GetLength());
				if(rotAxis.Normalize()==YSOK)  // Otherwise, no rotation is needed.
				{
					// Next normal is one of nextDir or -nextDir

					double tst1,tst2;
					tst1=(rotAxis^currentDir)*nom;
					tst2=(rotAxis^nextDir)*nextNom;
					if(tst1*tst2>=0.0)
					{
						currentDir=nextDir;
					}
					else
					{
						currentDir=-nextDir;
					}
				}
				else
				{
					if(nextDir*currentDir>0.0)
					{
						currentDir=nextDir;
					}
					else
					{
						currentDir=-nextDir;
					}
				}
			}
		}
		break;
	case 1:
		{
			currentDir=goal-currentPos;
			if(currentDir.Normalize()!=YSOK)
			{
				reachedDeadEnd=YSTRUE;
			}
		}
		break;
	}

	currentState=1;
	return YSOK;
}

YSRESULT YsShellCrawler::MoveAcrossVertex(const YsShell &shl,const YsShellSearchTable *search,const YsVec3 &nom,YsShellVertexHandle nextEdVtHd)
{
	int nFoundPlHd;
	const YsShellPolygonHandle *foundPlHd;
	int nPlVt;
	const YsShellVertexHandle *plVtHd;
	YsVec3 nextNom,nextDir,nextDirCan;
	YsShellPolygonHandle nextPlHd;

	if(crawlStopFunc!=NULL && crawlStopFunc->IsStopperVertex(shl,nextEdVtHd)==YSTRUE)
	{
		reachedDeadEnd=YSTRUE;
		return YSERR;
	}

	nextPlHd=NULL;

	shl.GetVertexPosition(currentPos,nextEdVtHd);
	currentEdVtHd[0]=nextEdVtHd;
	currentEdVtHd[1]=nextEdVtHd;

	if(search->FindPolygonListByVertex(nFoundPlHd,foundPlHd,shl,nextEdVtHd)==YSOK)
	{
		int i,j;
		YsArray <YsVec3,4> plVtPos;

		// Change 2005/11/28
		//   Test all the polygons, and take the one that are most parallel to the
		//   moving direction (currentDir) from the ones that can be a next polygon.

		double cosMaxDirNomAngle,cosDirNomAngle;
		cosMaxDirNomAngle=1.0;

		for(i=0; i<nFoundPlHd; i++)
		{
			if(foundPlHd[i]!=currentPlHd)
			{
				shl.GetNormalOfPolygon(nextNom,foundPlHd[i]);
				if(nextNom==YsOrigin())
				{
					YsErrOut("YsShellCrawler::Crawl() (transition across a vertex)\n");
					YsErrOut("  Normal vectors of the polygons must be set for this function.\n");
					return YSERR;
				}

				cosDirNomAngle=fabs(nextNom*currentDir);                // 2005/11/28
				if(nextPlHd!=NULL && cosMaxDirNomAngle<cosDirNomAngle)  // 2005/11/28
				{
					continue;  // Next i
				}

				switch(crawlingMode)
				{
				case 0:
				case 2:
					{
						YsVec3 rotAxis;
						double rotAngle;
						rotAxis=nom^nextNom;
						rotAngle=asin(rotAxis.GetLength());
						if(rotAxis.Normalize()==YSOK)  // Otherwise, no rotation is needed.
						{
							YsRotation rotation;
							rotation.Set(rotAxis,rotAngle);
							rotation.RotatePositive(nextDirCan,currentDir);
						}
						else
						{
							nextDirCan=currentDir;
						}

						if(crawlingMode==2)  // Crawl on a plane
						{
							YsVec3 ref;
							ref=nextNom^constPlane.GetNormal();
							if(ref.Normalize()!=YSOK)
							{
								reachedDeadEnd=YSTRUE;
							}
							else
							{
								if(currentDir*ref>0.0)
								{
									currentDir=ref;
								}
								else
								{
									currentDir=-ref;
								}
							}
							nextDirCan=currentDir; // 2007/03/08
						}
					}
					break;
				case 1:
					{
						nextDirCan=goal-currentPos;
						if(nextDirCan.Normalize()!=YSOK)
						{
							reachedDeadEnd=YSTRUE;
						}
					}
					break;
				}

				nPlVt=shl.GetNumVertexOfPolygon(foundPlHd[i]);
				plVtHd=shl.GetVertexListOfPolygon(foundPlHd[i]);
				shl.GetVertexListOfPolygon(plVtPos,foundPlHd[i]);
				for(j=0; j<nPlVt; j++)
				{
					if(plVtHd[j]==nextEdVtHd)
					{
						if(YsVectorPointingInside(plVtPos.GetN(),plVtPos,nextNom,j,nextDirCan)==YSTRUE)
						{
							nextPlHd=foundPlHd[i];
							nextDir=nextDirCan;
							cosMaxDirNomAngle=cosDirNomAngle;
						}
					}
				}
			}
		}
	}


	if(nextPlHd==NULL) // 2004/10/25
	{
		// Think about the possibility that the point moved across a vertex, and landed on an edge.
		YsArray <YsShellVertexHandle,16> connVtHd;
		if(search->GetConnectedVertexList(connVtHd,shl,nextEdVtHd)==YSOK)
		{
			int i;
			YsVec3 edVtPos[2],edVec,vecMax;
			YsShellVertexHandle toVtHd;
			double dotMax;

			shl.GetVertexPosition(edVtPos[0],nextEdVtHd);
			toVtHd=NULL;
			dotMax=0.707107;  // cos(45deg)
			for(i=0; i<connVtHd.GetN(); i++)
			{
				shl.GetVertexPosition(edVtPos[1],connVtHd[i]);
				edVec=edVtPos[1]-edVtPos[0];
				if(edVec*currentDir>YsTolerance)
				{
					edVec.Normalize();
					if(edVec*currentDir>dotMax)
					{
						toVtHd=connVtHd[i];
						dotMax=edVec*currentDir;
						vecMax=edVec;
					}
				}
			}
			if(toVtHd!=NULL)
			{
				int nFoundPlHd;
				const YsShellPolygonHandle *foundPlHd;

				if(search->FindPolygonListByEdge(nFoundPlHd,foundPlHd,shl,nextEdVtHd,toVtHd)==YSOK &&
				   nFoundPlHd>0)
				{
					nextPlHd=foundPlHd[0];
					nextDir=currentDir; // ? 2007/10/28
				}
			}
		}
	}


	if(nextPlHd==NULL)
	{
		reachedDeadEnd=YSTRUE;
		return YSERR;
	}
	else
	{
		// Memo: Even when crawlDirFunc is given, nextDir still needs to be calculated
		//       because crawlDirFunc may give a reverse direction.

		currentPlHd=nextPlHd;
		currentDir=nextDir;
	}

	currentState=2;

	return YSOK;
}

////////////////////////////////////////////////////////////

YSRESULT YsGetLoopAroundShellPolygon
   (YsArray <YsShellVertexHandle> &loopVtx,const YsShell &shl,YsShellPolygonHandle plHd)
{
	const YsShellSearchTable *search;
	search=shl.GetSearchTable();
	if(search!=NULL)
	{
		YsArray <YsEdgeTemplate <YsShellVertexHandle,unsigned>,64> edgeList;
		YsHashTable <YsShellPolygonHandle> visitedPlg(16);
		YsFixedLengthToMultiHashTable <int,2,2> visitedEdge(16);
		YSSIZE_T i,j,nPlVt;
		const YsShellVertexHandle *plVtHd;
		YsShellPolygonHandle tst;

		nPlVt=shl.GetNumVertexOfPolygon(plHd);
		plVtHd=shl.GetVertexListOfPolygon(plHd);

		for(i=0; i<nPlVt; i++)
		{
			YsShellVertexHandle pivotVtHd;
			int nFoundPlHd;
			const YsShellPolygonHandle *foundPlHd;

			pivotVtHd=plVtHd[i];
			search->FindPolygonListByVertex(nFoundPlHd,foundPlHd,shl,plVtHd[i]);

			for(j=0; j<nFoundPlHd; j++)
			{
				if(visitedPlg.FindElement(tst,shl.GetSearchKey(foundPlHd[j]))!=YSOK)
				{
					int k;
					int nPlVt;
					const YsShellVertexHandle *plVtHd;

					visitedPlg.AddElement(shl.GetSearchKey(foundPlHd[j]),foundPlHd[j]);

					nPlVt=shl.GetNumVertexOfPolygon(foundPlHd[j]);
					plVtHd=shl.GetVertexListOfPolygon(foundPlHd[j]);
					for(k=0; k<nPlVt; k++)
					{
						YsShellVertexHandle vtHd[2];
						unsigned vtKey[2];
						const YsArray <int,2> *foundEdge;
						vtHd[0]=plVtHd[k];
						vtHd[1]=plVtHd[(k+1)%nPlVt];

						vtKey[0]=shl.GetSearchKey(vtHd[0]);
						vtKey[1]=shl.GetSearchKey(vtHd[1]);

						foundEdge=visitedEdge.FindElement(2,vtKey);
						if(foundEdge!=NULL)
						{
							edgeList[(*foundEdge)[0]].attr++;
						}
						else
						{
							YsEdgeTemplate <YsShellVertexHandle,unsigned> newEdge;
							newEdge.vt[0]=vtHd[0];
							newEdge.vt[1]=vtHd[1];
							newEdge.attr=1;
							edgeList.Append(newEdge);
							visitedEdge.AddElement(2,vtKey,(int)edgeList.GetN()-1);
						}
					}
				}
			}
		}

		for(i=edgeList.GetN()-1; i>=0; i--)
		{
			if(edgeList[i].attr!=1)
			{
				edgeList.DeleteBySwapping(i);
			}
		}

		YsArray <const YsEdgeTemplate <YsShellVertexHandle,unsigned> *> loopEdge;
		if(edgeList.GetN()>0 &&
		   YsAssembleLoopFromEdgeList <YsShellVertexHandle,unsigned> (loopEdge,loopVtx,edgeList.GetN(),edgeList)==YSOK)
		{
			return YSOK;
		}
		loopVtx.Set(0,NULL);

		return YSERR;
	}
	else
	{
		YsErrOut("YsGetLoopAroundShellPolygon()\n");
		YsErrOut("  This function needs a search table.\n");
		return YSERR;
	}
}

double YsGetCosDihedralAngleAcrossShellEdge
   (const YsShell &shl,YsShellVertexHandle edVtHd1,YsShellVertexHandle edVtHd2)
{
	const YsShellSearchTable *search;
	int nFoundPlHd;
	const YsShellPolygonHandle *foundPlHd;

	search=shl.GetSearchTable();
	if(search!=NULL)
	{
		if(search->FindPolygonListByEdge(nFoundPlHd,foundPlHd,shl,edVtHd1,edVtHd2)==YSOK &&
		   nFoundPlHd==2)
		{
			YsVec3 nom[2];
			shl.GetNormalOfPolygon(nom[0],foundPlHd[0]);
			shl.GetNormalOfPolygon(nom[1],foundPlHd[1]);
			return nom[0]*nom[1];
		}
	}
	return -1.0;
}




YSRESULT YsShellFindPathBySteepestDescend
	    (YsArray <YsShellVertexHandle,16> &pathVtHd,
	     const YsShell &shl,YsShellVertexHandle fromVtHd,YsShellVertexHandle toVtHd)
{
	YsHashTable <YsShellVertexHandle> visited(1+shl.GetNumVertex()/16);
	YsArray <YsShellVertexHandle,16> conn;
	YsShellVertexHandle currentVtHd;
	YsVec3 goalPos;
	double orgDist,curDist;
	const YsShellSearchTable *search;

	pathVtHd.Set(0,NULL);

	search=shl.GetSearchTable();
	if(search==NULL)
	{
		return YSERR;
	}

	orgDist=shl.ComputeEdgeLength(fromVtHd,toVtHd);

	shl.GetVertexPosition(goalPos,toVtHd);

	currentVtHd=fromVtHd;
	visited.AddElement(shl.GetSearchKey(fromVtHd),fromVtHd);
	while(currentVtHd!=toVtHd)
	{
		YsShellVertexHandle nextVtHd,tst;

		pathVtHd.Append(currentVtHd);
		nextVtHd=NULL;
		if(search->GetConnectedVertexList(conn,shl,currentVtHd)==YSOK && conn.GetN()>0)
		{
			int i;
			double dMin;
			YsVec3 p;

			dMin=YsSqr(YsInfinity);
			for(i=0; i<conn.GetN(); i++)
			{
				if(visited.FindElement(tst,shl.GetSearchKey(conn[i]))!=YSOK)
				{
					visited.AddElement(shl.GetSearchKey(conn[i]),conn[i]);
					shl.GetVertexPosition(p,conn[i]);
					if(nextVtHd==NULL || (p-goalPos).GetSquareLength()<dMin)
					{
						dMin=(p-goalPos).GetSquareLength();
						nextVtHd=conn[i];
					}
				}
			}
		}

		if(nextVtHd==NULL)
		{
			break;
		}

		currentVtHd=nextVtHd;

		curDist=shl.ComputeEdgeLength(currentVtHd,toVtHd);
		if(curDist>orgDist*1.5)
		{
			break;
		}
	}

	if(currentVtHd==toVtHd)
	{
		pathVtHd.Append(toVtHd);
		return YSOK;
	}
	else
	{
		pathVtHd.Set(0,NULL);
		return YSERR;
	}
}

YSRESULT YsShellFindPathByStraightLine
	    (YsArray <YsShellVertexHandle,16> &pathVtHd,
	     const YsShell &shl,YsShellVertexHandle fromVtHd,YsShellVertexHandle toVtHd)
{
	YsArray <YsShellVertexHandle,16> conn;
	YsShellVertexHandle currentVtHd;
	YsVec3 orgPos,goalPos;
	double orgDist,curDist;
	const YsShellSearchTable *search;

	pathVtHd.Set(0,NULL);

	search=shl.GetSearchTable();
	if(search==NULL)
	{
		return YSERR;
	}

	orgDist=shl.ComputeEdgeLength(fromVtHd,toVtHd);

	shl.GetVertexPosition(orgPos,fromVtHd);
	shl.GetVertexPosition(goalPos,toVtHd);

	currentVtHd=fromVtHd;

	while(currentVtHd!=toVtHd)
	{
		YsShellVertexHandle nextVtHd;
		YsVec3 curPos;
		double minDistToLine=YsInfinity;

		pathVtHd.Append(currentVtHd);
		shl.GetVertexPosition(curPos,currentVtHd);
		curDist=(curPos-goalPos).GetSquareLength();

		nextVtHd=NULL;
		if(search->GetConnectedVertexList(conn,shl,currentVtHd)==YSOK && conn.GetN()>0)
		{
			int i;
			double nxtDist,distToLine;
			YsVec3 p;

			for(i=0; i<conn.GetN(); i++)
			{
				shl.GetVertexPosition(p,conn[i]);
				nxtDist=(p-goalPos).GetSquareLength();
				if(nxtDist<curDist)
				{
					distToLine=YsGetPointLineDistance3(orgPos,goalPos,p);
					if(nextVtHd==NULL || distToLine<minDistToLine)
					{
						minDistToLine=distToLine;
						nextVtHd=conn[i];
					}
				}
			}
		}

		if(nextVtHd==NULL)
		{
			break;
		}

		currentVtHd=nextVtHd;
	}

	if(currentVtHd==toVtHd)
	{
		pathVtHd.Append(toVtHd);
		return YSOK;
	}
	else
	{
		pathVtHd.Set(0,NULL);
		return YSERR;
	}
}

YSRESULT YsShellFindPath(
    YsArray <YsShellVertexHandle,16> &pathVtHd,
    const YsShell &shl,YsShellVertexHandle from,YsShellVertexHandle to)
{
	int i,j,shortest;
	YSRESULT res[3];
	YsArray <YsShellVertexHandle,16> path[3];

	res[0]=YsShellFindPathBySteepestDescend(path[0],shl,from,to);
	res[1]=YsShellFindPathBySteepestDescend(path[1],shl,to,from);
	res[2]=YsShellFindPathByStraightLine(path[2],shl,from,to);


	shortest=-1;

	double lng,shortLng=YsInfinity;
	YsVec3 p[2];

	for(i=0; i<3; i++)
	{
		if(res[i]==YSOK)
		{
			lng=0.0;
			if(path[i].GetN()>=2)
			{
				shl.GetVertexPosition(p[1],path[i][0]);
				for(j=1; j<path[i].GetN(); j++)
				{
					p[0]=p[1];
					shl.GetVertexPosition(p[1],path[i][j]);
					lng+=(p[1]-p[0]).GetLength();
				}
			}

			if(shortest<0 || lng<shortLng)
			{
				shortest=i;
				shortLng=lng;
			}
		}
	}

	if(shortest>=0)
	{
		pathVtHd.Set(path[shortest].GetN(),path[shortest]);
		if(pathVtHd[0]!=from)  // 2008/09/24
		{                      // 2008/09/24
			pathVtHd.Invert(); // 2008/09/24
		}                      // 2008/09/24
		return YSOK;
	}

	pathVtHd.Set(0,NULL);
	return YSERR;
}



YSRESULT YsShellFindProximityPolygonListByVertex
   (YsArray <YsShellPolygonHandle,16> &plHdList,
    const YsShell &shl,YsShellVertexHandle from,const double &dist)
{
	int nVtPl;
	const YsShellPolygonHandle *vtPlHd;
	const YsShellSearchTable *search;
	YsHashTable <YsShellPolygonHandle> visited(64);
	YsShellPolygonHandle plHd,neighbor,tst;
	YsArray <YsShellPolygonHandle,128> todo;
	YsVec3 refp;

	plHdList.Set(0,NULL);

	search=shl.GetSearchTable();
	if(search==NULL)
	{
		return YSERR;
	}

	if(search->FindPolygonListByVertex(nVtPl,vtPlHd,shl,from)==YSOK && nVtPl>0)
	{
		int i;

		shl.GetVertexPosition(refp,from);

		todo.Append(nVtPl,vtPlHd);
		for(i=0; i<nVtPl; i++)
		{
			visited.AddElement(shl.GetSearchKey(vtPlHd[i]),vtPlHd[i]);
		}
		while(todo.GetN()>0)
		{
			int nVt;
			const YsShellVertexHandle *plVtHd;
			YSBOOL proximity;
			YsVec3 edVtPos[2];

			plHd=todo[0];
			shl.GetVertexListOfPolygon(nVt,plVtHd,plHd);

			proximity=YSFALSE;
			shl.GetVertexPosition(edVtPos[1],plVtHd[nVt-1]);
			for(i=0; i<nVt; i++)
			{
				edVtPos[0]=edVtPos[1];
				shl.GetVertexPosition(edVtPos[1],plVtHd[i]);

				if((edVtPos[0]-refp).GetSquareLength()<dist*dist)
				{
					proximity=YSTRUE;
					break;
				}

				YsVec3 neap;
				if(YsGetNearestPointOnLine3(neap,edVtPos[0],edVtPos[1],refp)==YSOK &&
				   YsCheckInBetween3(neap,edVtPos[0],edVtPos[1])==YSTRUE &&
				   (neap-refp).GetSquareLength()<dist*dist)
				{
					proximity=YSTRUE;
					break;
				}
			}

			if(proximity==YSTRUE)
			{
				plHdList.Append(plHd);
				for(i=0; i<nVt; i++)
				{
					neighbor=search->GetNeighborPolygon(shl,plHd,i);
					if(neighbor!=NULL && visited.FindElement(tst,shl.GetSearchKey(neighbor))!=YSOK)
					{
						todo.Append(neighbor);
						visited.AddElement(shl.GetSearchKey(neighbor),neighbor);
					}
				}
			}

			todo.DeleteBySwapping(0);
		}

		return YSOK;
	}
	else
	{
		return YSERR;
	}
}



YSRESULT YsShellFindProximityPolygonListByEdge
   (YsArray <YsShellPolygonHandle,16> &plHdList,
    const YsShell &shl,YsShellVertexHandle fromEdVtHd[2],const double &dist)
{
	int nEdPl;
	const YsShellPolygonHandle *edPlHd;
	YsHashTable <YsShellPolygonHandle> visited(64);
	YsShellPolygonHandle plHd,neighbor,tst;
	YsArray <YsShellPolygonHandle,128> todo;
	YsVec3 refp[2];
	const YsShellSearchTable *search;

	plHdList.Set(0,NULL);

	search=shl.GetSearchTable();
	if(search==NULL)
	{
		return YSERR;
	}

	if(search->FindPolygonListByEdge(nEdPl,edPlHd,shl,fromEdVtHd[0],fromEdVtHd[1])==YSOK)
	{
		int i;

		shl.GetVertexPosition(refp[0],fromEdVtHd[0]);
		shl.GetVertexPosition(refp[1],fromEdVtHd[1]);

		todo.Set(nEdPl,edPlHd);
		for(i=0; i<nEdPl; i++)
		{
			visited.AddElement(shl.GetSearchKey(edPlHd[i]),edPlHd[i]);
		}

		while(todo.GetN()>0)
		{
			int nPlVt;
			const YsShellVertexHandle *plVtHd;
			YSBOOL proximity;
			YsVec3 edVtPos[2];
			double d;

			plHd=todo[0];

			shl.GetVertexListOfPolygon(nPlVt,plVtHd,plHd);

			proximity=YSFALSE;
			shl.GetVertexPosition(edVtPos[1],plVtHd[nPlVt-1]);
			for(i=0; i<nPlVt; i++)
			{
				edVtPos[0]=edVtPos[1];
				shl.GetVertexPosition(edVtPos[1],plVtHd[i]);
				d=YsGetDistanceBetweenTwoLineSegment(edVtPos[0],edVtPos[1],refp[0],refp[1]);
				if(d<dist)
				{
					proximity=YSTRUE;
					break;
				}
			}

			if(proximity==YSTRUE)
			{
				plHdList.Append(plHd);
				for(i=0; i<nPlVt; i++)
				{
					neighbor=search->GetNeighborPolygon(shl,plHd,i);
					if(neighbor!=NULL && visited.FindElement(tst,shl.GetSearchKey(neighbor))!=YSOK)
					{
						todo.Append(neighbor);
						visited.AddElement(shl.GetSearchKey(neighbor),neighbor);
					}
				}
			}

			todo.DeleteBySwapping(0);
		}

		return YSOK;
	}
	else
	{
		return YSERR;
	}
}

YSRESULT YsShellGetNthEdge(YsShellVertexHandle edVtHd[2],int nPlVt,const YsShellVertexHandle plVtHd[],int n)
{
	return YsGetNthEdge <YsShellVertexHandle> (edVtHd,nPlVt,plVtHd,n);
}

YSRESULT YsShellGetNthEdgeCyclic(YsShellVertexHandle edVtHd[2],int nPlVt,const YsShellVertexHandle plVtHd[],int n)
{
	return YsGetNthEdgeCyclic <YsShellVertexHandle> (edVtHd,nPlVt,plVtHd,n);
}

YSRESULT YsShellMakePolygonStrip(
    YSBOOL &isLoop,YsArray <YsShellPolygonHandle> &plgStrip,
    const YsShell &shl,YSSIZE_T nPl,const YsShellPolygonHandle plHdList[])
{
	YsKeyStore plKey(nPl/2+1);
	YsShellPolygonHandle loopSeed,stripSeed,neiPlHd,seedPlHd;
	const YsShellSearchTable *search=shl.GetSearchTable();
	int i,j,n;

	if(search==NULL)
	{
		YsPrintf("YsShellMakePolygonStrip()\n");
		YsPrintf("  Need search table!\n");
		return YSERR;
	}

	for(i=0; i<nPl; i++)
	{
		plKey.AddKey(shl.GetSearchKey(plHdList[i]));
	}

	loopSeed=NULL;
	stripSeed=NULL;
	for(i=0; i<nPl && stripSeed==NULL; i++)
	{
		int nNei,nPlVt;
		nPlVt=shl.GetNumVertexOfPolygon(plHdList[i]);
		nNei=0;
		for(j=0; j<nPlVt; j++)
		{
			neiPlHd=search->GetNeighborPolygon(shl,plHdList[i],j);
			if(neiPlHd!=NULL && plKey.IsIncluded(shl.GetSearchKey(neiPlHd))==YSTRUE)
			{
				nNei++;
			}
		}
		if(nNei==1)
		{
			stripSeed=plHdList[i];
		}
		else if(nNei==2)
		{
			loopSeed=plHdList[i];
		}
		else
		{
			return YSERR;  // Not a loop or a strip
		}
	}

	if(stripSeed!=NULL)
	{
		seedPlHd=stripSeed;
		isLoop=YSFALSE;
	}
	else if(loopSeed!=NULL)
	{
		seedPlHd=loopSeed;
		isLoop=YSTRUE;
	}
	else
	{
		return YSERR;  // Just in case.  2010/04/18
	}

	n=0;
	plgStrip.Set(nPl,NULL);
	plgStrip[n++]=seedPlHd;
	plKey.DeleteKey(shl.GetSearchKey(seedPlHd));

	for(;;) // Visual C++ gives a warning for "while(1)"
	{
		int nPlVt,nPrev;
		YSBOOL grow;

		nPlVt=shl.GetNumVertexOfPolygon(seedPlHd);
		nPrev=n;
		grow=YSFALSE;
		for(j=0; j<nPlVt; j++)
		{
			neiPlHd=search->GetNeighborPolygon(shl,seedPlHd,j);
			if(neiPlHd!=NULL && plKey.IsIncluded(shl.GetSearchKey(neiPlHd))==YSTRUE)
			{
				grow=YSTRUE;
				seedPlHd=neiPlHd;
				plgStrip[n++]=seedPlHd;
				plKey.DeleteKey(shl.GetSearchKey(seedPlHd));
				break;
			}
		}

		if(grow!=YSTRUE || n==nPl)
		{
			break;
		}
	}

	if(n==nPl)
	{
		return YSOK;
	}
	else
	{
		plgStrip.Set(0,NULL);
		return YSERR;
	}
}

YSRESULT YsMakePolygonSubGroup(
    YsArray <YsShellPolygonHandle,64> &plHdList,
    const YsShell &shl,YsShellPolygonHandle seedPlHd,const YsKeyStore &includedPlKey)
{
	const YsShellSearchTable *search;

	YsArray <YsShellPolygonHandle,64> todo;
	YsKeyStore visitedPlg(16);

	search=shl.GetSearchTable();
	if(search==NULL)
	{
		YsPrintf("YsMakePolygonSubGroup\n");
		YsPrintf("  Need search table.\n");
		return YSERR;
	}

	visitedPlg.EnableAutoResizing(16,shl.GetNumPolygon()/8+1);

	plHdList.Set(0,NULL);

	todo.Append(seedPlHd);
	visitedPlg.AddKey(shl.GetSearchKey(seedPlHd));
	plHdList.Append(seedPlHd);

	while(todo.GetN()>0)
	{
		int i,nPlVt;
		nPlVt=shl.GetNumVertexOfPolygon(todo[0]);
		for(i=0; i<nPlVt; i++)
		{
			YsShellPolygonHandle neiPlHd;
			neiPlHd=search->GetNeighborPolygon(shl,todo[0],i);
			if(neiPlHd!=NULL &&
			   visitedPlg.IsIncluded(shl.GetSearchKey(neiPlHd))!=YSTRUE &&
			   includedPlKey.IsIncluded(shl.GetSearchKey(neiPlHd))==YSTRUE)
			{
				todo.Append(neiPlHd);
				plHdList.Append(neiPlHd);
				visitedPlg.AddKey(shl.GetSearchKey(neiPlHd));
			}
		}
		todo.DeleteBySwapping(0);
	}

	return YSOK;
}



YSRESULT YsGlueShellPolygon
    (int &nvNew,YsShellVertexHandle vNew[],
     int nv1,const YsShellVertexHandle v1[],int nv2,const YsShellVertexHandle v2[],
     YsShellVertexHandle sharedEdVtHd1,YsShellVertexHandle sharedEdVtHd2)
{
	// Added 2002/05/02 >>
	if(sharedEdVtHd1==NULL || sharedEdVtHd2==NULL)
	{
		int i,j;
		YSBOOL found;
		YsShellVertexHandle edVtHd1[2],edVtHd2[2];
		found=YSFALSE;
		for(i=0; i<nv1 && found!=YSTRUE; i++)
		{
			edVtHd1[0]=v1[i];
			edVtHd1[1]=v1[(i+1)%nv1];
			for(j=0; j<nv2; j++)
			{
				edVtHd2[0]=v2[j];
				edVtHd2[1]=v2[(j+1)%nv2];
				if((edVtHd1[0]==edVtHd2[0] && edVtHd1[1]==edVtHd2[1]) ||
				   (edVtHd1[0]==edVtHd2[1] && edVtHd1[1]==edVtHd2[0]))
				{
					sharedEdVtHd1=edVtHd1[0];
					sharedEdVtHd2=edVtHd1[1];
					found=YSTRUE;
					break;
				}
			}
		}
		if(found!=YSTRUE)
		{
			return YSERR;
		}
	}
	// Added 2002/05/02 <<


	int i,j,n;
	YSBOOL reverse;

	nvNew=0;

	n=-1;
	for(i=0; i<nv1; i++)
	{
		if(v1[i]==sharedEdVtHd1)
		{
			n=i;
			break;
		}
	}

	if(n<0)
	{
		return YSERR;
	}


	nvNew=0;
	if(v1[(n+1)%nv1]==sharedEdVtHd2)
	{
		reverse=YSTRUE;
		for(i=0; i<nv1; i++)
		{
			vNew[nvNew++]=v1[(n-i+nv1)%nv1];
		}
	}
	else if(v1[(n+nv1-1)%nv1]==sharedEdVtHd2)
	{
		reverse=YSFALSE;
		for(i=0; i<nv1; i++)
		{
			vNew[nvNew++]=v1[(n+i)%nv1];
		}
	}
	else
	{
		return YSERR;
	}


	n=-1;
	for(i=0; i<nv2; i++)
	{
		if(v2[i]==sharedEdVtHd2)
		{
			n=i;
			break;
		}
	}

	if(n<0)
	{
		return YSERR;
	}

	if(v2[(n+1)%nv2]==sharedEdVtHd1)
	{
		for(i=1; i<nv2-1; i++)
		{
			vNew[nvNew++]=v2[(n-i+nv2)%nv2];
		}
	}
	else if(v2[(n+nv2-1)%nv2]==sharedEdVtHd1)
	{
		for(i=1; i<nv2-1; i++)
		{
			vNew[nvNew++]=v2[(n+i)%nv2];
		}
	}
	else
	{
		return YSERR;
	}

	if(reverse==YSTRUE)
	{
		for(i=0; i<nvNew/2; i++)
		{
			YsSwapSomething <YsShellVertexHandle> (vNew[i],vNew[nvNew-1-i]);
		}
	}


	// Post-process
	//    2003/01/31 (1)...-A-B-A-...   =>   ...-A-A-...  It's wrong, it should be...
	// (1)...-A-B-A-C-... =>  ...-A-C-...
	for(;;) // Visual C++ gives a warning for "while(1)"
	{
		YSBOOL corrected;
		corrected=YSFALSE;

		for(i=0; i<nvNew; i++)
		{
			if(vNew[i]==vNew[(i+2)%nvNew])
			{
				if(i+2<=nvNew)
				{
					for(j=i; j<nvNew-2; j++)
					{
						vNew[j]=vNew[j+2];
					}
				}
				else
				{
					// vNew[0] and vNew[nvNew-1] must be deleted.
					for(j=0; j<nvNew-1; j++)
					{
						vNew[j]=vNew[j+1];
					}
				}
				nvNew-=2;
				corrected=YSTRUE;
			}
		}
		if(corrected!=YSTRUE)
		{
			break;
		}
	}

	// (2)...-A-B-B-C-... => ...-A-B-C-...
	for(i=nvNew-1; i>=0; i--)
	{
		if(vNew[i]==vNew[(i+1)%nvNew])
		{
			for(j=i; j<nvNew-1; j++)
			{
				vNew[j]=vNew[j+1];
			}
			nvNew--;
			if(nvNew<=2)
			{
				return YSERR;
			}
		}
	}

	return YSOK;
}

YSRESULT YsMergeShellPolygon(YsShell &shl,int nOrgPlg,const YsShellPolygonHandle orgPlHd[])
{
	YSSIZE_T i,j;
	YsArray <YsShellPolygonHandle> plHdList;

	plHdList.Set(nOrgPlg,orgPlHd);

	for(i=0; i<plHdList.GetN(); i++)
	{
	RETRY:
		for(j=plHdList.GetN()-1; j>i; j--)
		{
			int nPlVt1,nPlVt2;
			YsShellPolygonHandle plHd[2];
			const YsShellVertexHandle *plVt1,*plVt2;
			int nPlVtNew;
			YsArray <YsShellVertexHandle,16> plVtNew;

			plHd[0]=plHdList[i];
			plHd[1]=plHdList[j];

			nPlVt1=shl.GetNumVertexOfPolygon(plHd[0]);
			plVt1=shl.GetVertexListOfPolygon(plHd[0]);
			nPlVt2=shl.GetNumVertexOfPolygon(plHd[1]);
			plVt2=shl.GetVertexListOfPolygon(plHd[1]);

			int m,n;
			YsShellVertexHandle sharedEdVtHd[2];
			sharedEdVtHd[1]=plVt1[nPlVt1-1];
			YSBOOL found;
			found=YSFALSE;
			for(m=0; m<nPlVt1 && found!=YSTRUE; m++)
			{
				sharedEdVtHd[0]=sharedEdVtHd[1];
				sharedEdVtHd[1]=plVt1[m];
				for(n=0; n<nPlVt2; n++)
				{
					if((plVt2[n]==sharedEdVtHd[0] && plVt2[(n+1)%nPlVt2]==sharedEdVtHd[1]) ||
					   (plVt2[n]==sharedEdVtHd[1] && plVt2[(n+1)%nPlVt2]==sharedEdVtHd[0]))
					{
						found=YSTRUE;
						break;
					}
				}
			}

			if(found==YSTRUE)
			{
				plVtNew.Set(nPlVt1+nPlVt2,NULL);
				if(YsGlueShellPolygon
				      (nPlVtNew,plVtNew,
				       nPlVt1,plVt1,nPlVt2,plVt2,sharedEdVtHd[0],sharedEdVtHd[1])==YSOK)
				{
					shl.ModifyPolygon(plHdList[i],nPlVtNew,plVtNew);
					shl.DeletePolygon(plHdList[j]);
					plHdList.DeleteBySwapping(j);
					goto RETRY;
				}
			}
		}
	}

	if(plHdList.GetN()<nOrgPlg)
	{
		return YSOK;
	}
	return YSERR;
}



YSRESULT YsGetCrawlToNearestPointOnShell(YsVec3 &nearPos,YsShellElemType &nearElem,const YsShell &shl,const YsShellElemType &from,const YsVec3 &to)
{
	YsShellVertexHandle trkVtHd,nxtVtHd;
	int i,j,nPlVt;
	const YsShellVertexHandle *plVtHd;
	YsVec3 vtPos;
	double dist,minDist,curDist;
	YsArray <YsShellVertexHandle,16> connVtHd;
	const YsShellSearchTable *search;

	search=shl.GetSearchTable();
	if(search==NULL)
	{
		YsErrOut("YsGetCrawlToNearestPointOnShell()\n");
		YsErrOut("  Need search table.\n");
		return YSERR;
	}

	switch(from.elemType)
	{
	default:
		return YSERR;
	case YSSHELL_VERTEX:
		trkVtHd=from.vtHd;
		break;
	case YSSHELL_EDGE:
		trkVtHd=from.edVtHd[0];
		break;
	case YSSHELL_POLYGON:
		shl.GetVertexListOfPolygon(nPlVt,plVtHd,from.plHd);
		trkVtHd=plVtHd[0];
		shl.GetVertexPosition(vtPos,plVtHd[0]);
		minDist=(vtPos-to).GetSquareLength();
		for(i=1; i<nPlVt; i++)
		{
			shl.GetVertexPosition(vtPos,plVtHd[i]);
			dist=(vtPos-to).GetSquareLength();
			if(dist<minDist)
			{
				trkVtHd=plVtHd[i];
			}
		}
		break;
	}



	shl.GetVertexPosition(vtPos,trkVtHd);
	curDist=(vtPos-to).GetSquareLength();
	for(;;) // Visual C++ gives a warning for "while(1)"
	{
		nxtVtHd=NULL;
		search->GetConnectedVertexList(connVtHd,shl,trkVtHd);
		forYsArray(i,connVtHd)
		{
			shl.GetVertexPosition(vtPos,connVtHd[i]);
			dist=(vtPos-to).GetSquareLength();
			if(dist<curDist)
			{
				nxtVtHd=connVtHd[i];
				curDist=dist;
			}
		}

		if(nxtVtHd==NULL)
		{
			break;
		}

		trkVtHd=nxtVtHd;
	}



	YsPlane pln;
	YsArray <YsVec3,4> plVtPos;
	int nPl;
	const YsShellPolygonHandle *plHdList;
	double nearDist;

	nearElem.elemType=YSSHELL_VERTEX;
	nearElem.vtHd=trkVtHd;
	nearDist=curDist;
	shl.GetVertexPosition(nearPos,trkVtHd);

	if(search->FindPolygonListByVertex(nPl,plHdList,shl,trkVtHd)==YSOK && nPl>0)
	{
		for(i=0; i<nPl; i++)
		{
			YsVec3 prj;

			shl.GetVertexListOfPolygon(plVtPos,plHdList[i]);
			pln.MakeBestFitPlane(plVtPos.GetN(),plVtPos);
			pln.GetNearestPoint(prj,to);

			if(YsCheckInsidePolygon3(prj,plVtPos.GetN(),plVtPos)==YSINSIDE)
			{
				dist=(prj-to).GetSquareLength();
				if(dist<nearDist)
				{
					nearElem.elemType=YSSHELL_POLYGON;
					nearElem.plHd=plHdList[i];
					nearDist=dist;
					nearPos=prj;
				}
			}
			else
			{
				shl.GetVertexListOfPolygon(nPlVt,plVtHd,plHdList[i]);
				for(j=0; j<plVtPos.GetN(); j++)
				{
					if(YsGetNearestPointOnLine3(prj,plVtPos[j],plVtPos[(j+1)%plVtPos.GetN()],to)==YSOK &&
					   YsCheckInBetween3(prj,plVtPos[j],plVtPos[(j+1)%plVtPos.GetN()])==YSTRUE)
					{
						dist=(prj-to).GetSquareLength();
						if(dist<nearDist)
						{
							nearElem.elemType=YSSHELL_EDGE;
							nearElem.edVtHd[0]=plVtHd[j];
							nearElem.edVtHd[1]=plVtHd[(j+1)%plVtPos.GetN()];
							nearDist=dist;
							nearPos=prj;
						}
					}
				}
			}
		}
		return YSOK;
	}

	return YSERR;
}
