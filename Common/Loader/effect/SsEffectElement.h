#ifndef __SSEFFECTELEMENT__
#define __SSEFFECTELEMENT__


#include "sstypes.h"
#include "ssarchiver.h"

enum EffectPartType
{
	EffectPartTypeEmiiter,
	EffectPartTypeParticle

};



enum EmmiterType
{
    EmmiterTypeNormal,
    EmmiterTypeRibbon,
};



enum SsRenderType{
    BaseNode,
	EmmiterNode,
    ParticleNode,
};


enum SsRenderBlendType
{
	Mix,
	Add
};


class SsCell;

//�͈͒l�N���X
template<class mytype>
class VarianceValue : public SsXmlStringConverter
{
private:
	enum RangeType{
		None,
		MinMax,
		PlusMinus,
	};

	RangeType 	type;
	mytype 		value;
	mytype 		subvalue;


public:

	VarianceValue( mytype v ){
		value = v;
		subvalue = v;
        type = RangeType::None;
	}
	VarianceValue( mytype v , mytype v2){
		value = v;
		subvalue = v2;
        type = RangeType::MinMax;
	}

	void	setPlusMinus( mytype v1 , mytype v2 )
	{
		value = v;
		subvalue = plusminus;
        type = RangeType::PlusMinus;
	}

	void	setMinMax(mytype min , mytype max)
	{
		value = min;
		subvalue = max;
        type = RangeType::MinMax;
	}

	mytype	getValue(){ return value;}
	mytype	getMinValue(){ return value;}
	mytype	getMaxValue(){ return subvalue;}

	bool	isTypeNone(){ return  type == RangeType::None; }
	bool	isTypeMinMax(){ return  type == RangeType::MinMax; }
	bool	isTypePlusMinus(){ return  type == RangeType::PlusMinus; }

	mytype* getlpValue(){ return &value;}
	mytype* getlpSubValue(){ return &subvalue;}

	VarianceValue& operator=(mytype v) {  value = v; return *this; }
	operator mytype() { return value; }

private:
	//SS_FRIEND_SERIALIZATION_ACCESS
	// AARRGGBB �̂P�U�i��������ŕۑ�
	//SS_SERIALIZATION_SPLIT_MEMBER();
	//bool	save(SsXmlOArchive& ar, const unsigned int version) const;
	//bool	load(SsXmlIArchive& ar, const unsigned int version);

	virtual bool	inputString(SsString str )
	{}
};


typedef VarianceValue<float>   			f32VValue;
typedef VarianceValue<int>    			i32VValue;
typedef VarianceValue<SsU8Color>    	SsU8cVValue;
typedef VarianceValue<SsVector2>    	SsVec2VValue;


template<> bool VarianceValue<float>::inputString(SsString str){}
template<> bool VarianceValue<int>::inputString(SsString str){}
template<> bool VarianceValue<SsU8Color>::inputString(SsString str){}
template<> bool VarianceValue<SsVector2>::inputString(SsString str){}



class SsEffectRenderEmitter;
class SsEffectRenderParticle;


class   SsEffectElementBase 
{
public:
	SsString            myName;
	SsString			myCaption;	//�G�������g�̐������Ƃ�
	static int          uid;
	int					myuid;
public:
	SsEffectElementBase();
	virtual ~SsEffectElementBase();

	//�e�R�}���h�ɑΉ������f�o�b�O�\���p
	virtual void    ToolGuideDraw(SsEffectRenderEmitter* emmiter){}

	//�e���Ŏ�������
	virtual void InitializeEmmiter( SsEffectRenderEmitter* emmiter ) {}
	virtual void UpdateEmmiter( SsEffectRenderEmitter* emmiter ){}
	virtual void UpdateEndEmmiter( SsEffectRenderEmitter* emmiter ){}


	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle ){}
	virtual void UpdateParticle( SsEffectRenderParticle* particle ){}


	//�V���A���C�U
	//SS_DECLARE_SPLIT_SERIALIZE_FUNCS();

	SSSERIALIZE_BLOCK
	{
		SSAR_DECLARE( myName );
		SSAR_DECLARE( myCaption );
		SSAR_DECLARE( uid );
		SSAR_DECLARE( myuid );
	}

};





//--------------------------------------------------------------------------------------
//�p�[�e�B�N�����\�������{�̒l
class  ParticleElementBasic
{
public:
	int		maximumParticle;
	f32VValue	speed;
	i32VValue 	lifespan;
	float	angle;
	float	angleVariance;
	int		interval;
	int		lifetime;
	int		attimeCreate;
    int		priority;

public:
	ParticleElementBasic()
			: 	maximumParticle( 50 ),
				speed( 5.0f , 5.0f),
				lifespan( 30.0f ,30.0f),
				angle(0.0f),
				angleVariance(45),
				interval(1.0f),
				lifetime(30.0f),
				attimeCreate(1),
                priority(64)
	{
	}

	virtual ~ParticleElementBasic(){}

	virtual void ToolGuideDraw(SsEffectRenderEmitter* emmiter);

	virtual void InitializeEmmiter( SsEffectRenderEmitter* emmiter );
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle ){}

	SSSERIALIZE_BLOCK
	{
		SSAR_DECLARE( maximumParticle );
		SSAR_DECLARE( speed );
		SSAR_DECLARE( lifespan );
		SSAR_DECLARE( angle );
		SSAR_DECLARE( angleVariance );
		SSAR_DECLARE( interval );
		SSAR_DECLARE( lifetime );
		SSAR_DECLARE( attimeCreate );
		SSAR_DECLARE( priority );
	}

};


//--------------------------------------------------------------------------------------
//�x��
class  ParticleElementRndSeedChange : public SsEffectElementBase
{
public:
	int		Seed;

	ParticleElementRndSeedChange()

		:	Seed( 0 )
	{

	}
	virtual ~ParticleElementRndSeedChange(){}

	//�e���Ŏ�������
	virtual void InitializeEmmiter( SsEffectRenderEmitter* emmiter );


	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();
	SSSERIALIZE_BLOCK
	{
		SSAR_DECLARE( Seed );
	}

};

//--------------------------------------------------------------------------------------
//�x��
class  ParticleElementDelay : public SsEffectElementBase
{
public:
	int		DelayTime;

	ParticleElementDelay()
		:	DelayTime( 0 )
	{
	}
	virtual ~ParticleElementDelay(){}

	//�e���Ŏ�������
	virtual void InitializeEmmiter( SsEffectRenderEmitter* emmiter );
	virtual void UpdateEmmiter( SsEffectRenderEmitter* emmiter );

	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle ){}
	virtual void UpdateParticle( SsEffectRenderParticle* particle ){}

	virtual void UpdateEndEmmiter( SsEffectRenderEmitter* emmiter );

	SSSERIALIZE_BLOCK
	{
		SSAR_DECLARE( DelayTime );
	}

	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();

};


//--------------------------------------------------------------------------------------
//�d�͂ւ̉e��
class  ParticleElementGravity : public SsEffectElementBase
{
public:

	SsVector2   Gravity;

public:
	ParticleElementGravity()
        : Gravity(	0	,	-3.0f	)
    {

	}
	virtual ~ParticleElementGravity(){}

	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );

	SSSERIALIZE_BLOCK
	{
		SSAR_DECLARE( Gravity );
	}
	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();

};

//--------------------------------------------------------------------------------------
//�����ʒu�ւ̉e��
class  ParticleElementPosition : public SsEffectElementBase
{
public:
	f32VValue   OffsetX;
	f32VValue   OffsetY;


	ParticleElementPosition()
		: OffsetX(0,0),OffsetY(0,0)
	{
	}
	virtual ~ParticleElementPosition(){}
	virtual    SsEffectElementBase*  new_(){ return new ParticleElementPosition(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );

	SSSERIALIZE_BLOCK
	{
		SSAR_DECLARE( OffsetX );
		SSAR_DECLARE( OffsetY );
	}

	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();

};
//--------------------------------------------------------------------------------------
//�����ʒu�ւ̉e��
class  ParticleElementTransPosition : public SsEffectElementBase
{
public:
	f32VValue   OffsetX;
	f32VValue   OffsetY;


	ParticleElementTransPosition()
		: OffsetX(0,0),OffsetY(0,0)
	{
	}
	virtual ~ParticleElementTransPosition(){}
	virtual    SsEffectElementBase*  new_(){ return new ParticleElementTransPosition(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle ){}

	SSSERIALIZE_BLOCK
	{
		SSAR_DECLARE( OffsetX );
		SSAR_DECLARE( OffsetY );
	}

	//�V���A���C�U
	//SS_DECLARE_SPLIT_SERIALIZE_FUNCS();

};


//--------------------------------------------------------------------------------------
//�p�x�ω���
class  ParticleElementRotation : public SsEffectElementBase
{
public:
	f32VValue   Rotation;
	f32VValue   RotationAdd;

	ParticleElementRotation()
		:  Rotation( 0 ,0 ),
		   RotationAdd(0,0)
	{
	}
	virtual ~ParticleElementRotation(){}
	virtual    SsEffectElementBase*  new_(){ return new ParticleElementRotation(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );

	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();

};

//--------------------------------------------------------------------------------------
//�p�x�ω���
class  ParticleElementRotationTrans : public SsEffectElementBase
{
public:
	float   RotationFactor;
	float	EndLifeTimePer;

	ParticleElementRotationTrans()
		:  	RotationFactor( 0 ) ,
			EndLifeTimePer( 75 )
	{
	}
	virtual ~ParticleElementRotationTrans(){}
	virtual    SsEffectElementBase*  new_(){ return new ParticleElementRotationTrans(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
 	virtual void UpdateParticle( SsEffectRenderParticle* particle );

	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();

};

//--------------------------------------------------------------------------------------
class  ParticleElementTransSpeed : public SsEffectElementBase
{
public:
	f32VValue	Speed;

	ParticleElementTransSpeed()
        :	Speed( 0,0 )
	{

	}
	virtual ~ParticleElementTransSpeed(){}
	virtual    SsEffectElementBase*  new_(){ return new ParticleElementTransSpeed(); }

	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
 	virtual void UpdateParticle( SsEffectRenderParticle* particle );

	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();
};


//--------------------------------------------------------------------------------------
//�ڐ�����x��^����
class  ParticleElementTangentialAcceleration : public SsEffectElementBase
{
public:
	f32VValue	Acceleration;

	ParticleElementTangentialAcceleration()
			: Acceleration( 0, 0 )
	{

	}
	virtual ~ParticleElementTangentialAcceleration(){}
	virtual    SsEffectElementBase*  new_(){ return new ParticleElementTangentialAcceleration(); }

	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );


	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();
};

//--------------------------------------------------------------------------------------
//���_�J���[�𐧌䂷��   (���o�[�W����
class  ParticleElementColor : public SsEffectElementBase
{
public:
	SsU8Color	StartColor;
	SsU8Color   StartColorVariance;
	SsU8Color	EndColor;
	SsU8Color   EndColorVariance;

	ParticleElementColor()
        :	StartColor( 255,255,255,255 ),
            StartColorVariance( 255,255,255,255 ),
        	EndColor( 255,255,255,255 ),
            EndColorVariance( 255,255,255,255 )
	{
	}
	virtual ~ParticleElementColor(){}
	virtual	SsEffectElementBase*  new_(){ return new ParticleElementColor(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );
	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();
};



//--------------------------------------------------------------------------------------
//���_�J���[�𐧌䂷��
class  ParticleElementInitColor : public SsEffectElementBase
{
public:
	SsU8cVValue Color;

	ParticleElementInitColor()
		: Color( SsU8Color(255,255,255,255) , SsU8Color(255,255,255,255) )
	{
	}
	virtual ~ParticleElementInitColor(){}
	virtual	SsEffectElementBase*  new_(){ return new ParticleElementInitColor(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );
	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();
};

//--------------------------------------------------------------------------------------
//���_�J���[�𐧌䂷��
class  ParticleElementTransColor : public SsEffectElementBase
{
public:
	SsU8cVValue Color;

	ParticleElementTransColor()
		: Color( SsU8Color(255,255,255,255) , SsU8Color(255,255,255,255) )
	{
	}
	virtual ~ParticleElementTransColor(){}
	virtual	SsEffectElementBase*  new_(){ return new ParticleElementTransColor(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );
	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();
};


//--------------------------------------------------------------------------------------
//���_�J���[�𐧌䂷��
class  ParticleElementAlphaFade : public SsEffectElementBase
{
public:
	f32VValue  disprange;


	ParticleElementAlphaFade()
		: disprange(25,75)
	{
	}
	virtual ~ParticleElementAlphaFade(){}
	virtual	SsEffectElementBase*  new_(){ return new ParticleElementAlphaFade(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );
	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();
};

//--------------------------------------------------------------------------------------
//�T�C�Y����
class  ParticleElementSize : public SsEffectElementBase
{
public:
	f32VValue SizeX;
	f32VValue SizeY;
	f32VValue ScaleFactor;

	ParticleElementSize()
		: 	SizeX( 1.0f , 1.0f ),
			SizeY( 1.0f , 1.0f ),
			ScaleFactor( 1.0f , 1.0f )
	{
	}
	virtual ~ParticleElementSize(){}
	virtual SsEffectElementBase*  new_(){ return new ParticleElementSize(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );

	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();

};


//--------------------------------------------------------------------------------------
//�T�C�Y�ύX
class  ParticleElementTransSize : public SsEffectElementBase
{
public:
	f32VValue SizeX;
	f32VValue SizeY;
	f32VValue ScaleFactor;


	ParticleElementTransSize()
		: 	SizeX( 1.0f , 1.0f ),
			SizeY( 1.0f , 1.0f ),
			ScaleFactor( 1.0f , 1.0f )
	{
	}
	virtual ~ParticleElementTransSize(){}
	virtual SsEffectElementBase*  new_(){ return new ParticleElementTransSize(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );

	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();

};

//--------------------------------------------------------------------------------------
//�d�͓_
class  ParticlePointGravity : public SsEffectElementBase
{
public:

	SsVector2   Position;
    float		Power;


	ParticlePointGravity()
		: Position( 0 , 0 ) ,Power(0.0f)
	{
	}
	virtual ~ParticlePointGravity(){}
	virtual SsEffectElementBase*  new_(){ return new ParticlePointGravity(); }


	//�e���Ŏ�������
	virtual void InitializeEmmiter( SsEffectRenderEmitter* emmiter ){}
	virtual void UpdateEmmiter( SsEffectRenderEmitter* emmiter ){}

	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );


	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();

};

//--------------------------------------------------------------------------------------
//���t���N�V�����{�b�N�X
class  ParticleReflectionBox : public SsEffectElementBase
{
public:
	SsVector2   WH;
	float		Factor;


	ParticleReflectionBox() :Factor( 70 )
	{
	}
	virtual ~ParticleReflectionBox(){}
	virtual SsEffectElementBase*  new_(){ return new ParticleReflectionBox(); }

	virtual void    ToolGuideDraw(SsEffectRenderEmitter* emmiter);


	//�e���Ŏ�������
	virtual void InitializeEmmiter( SsEffectRenderEmitter* emmiter ){}
	virtual void UpdateEmmiter( SsEffectRenderEmitter* emmiter ){}


	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle ){}
	virtual void UpdateParticle( SsEffectRenderParticle* particle );


	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();

};



//--------------------------------------------------------------------------------------
class  ParticleTurnToDirectionEnabled : public SsEffectElementBase
{
public:

	ParticleTurnToDirectionEnabled()
	{

	}
	virtual ~ParticleTurnToDirectionEnabled(){}
	virtual SsEffectElementBase*  new_(){ return new ParticleTurnToDirectionEnabled(); }


	//�e���Ŏ�������
	virtual void InitializeEmmiter( SsEffectRenderEmitter* emmiter ){}
	virtual void UpdateEmmiter( SsEffectRenderEmitter* emmiter ){}


	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle ){}


	//�V���A���C�U
//	SS_DECLARE_SPLIT_SERIALIZE_FUNCS();

};








#endif
