#ifndef __SSEFFECTBEHAVIOR__
#define __SSEFFECTBEHAVIOR__

#include "SsEffectElement.h"


class SsEffectBehavior
{
public:

	//ParticleElementList 	plist;
	//ParticleElementBasic*   basicData;	//���X�K�v�ȃf�[�^�Ȃ̂ŕʂɂ��邩��


public:
    SsCell*					refCell;
	SsString				CellName;
	SsString				CellMapName;
	SsRenderBlendType       blendType;

public:
	SsEffectBehavior(){}
	virtual ~SsEffectBehavior(){}


};






#endif
