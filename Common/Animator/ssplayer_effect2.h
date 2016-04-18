#ifndef __SSEFFECTRENDER_V3__
#define __SSEFFECTRENDER_V3__


#include "xorshift32.h"

//SsVector�Ȃ�
//#include "SsUtility.h"

//#include "ISSEffectRender.h"


class SsEffectModel;
class SsRenderEffectBase;
class SsEffectNode;
class SsPartState;
class SsEffectRenderAtom;
class SsCell;



#define SEED_MAGIC (7573)
#define LOOP_TYPE1 (0)
#define LOOP_TYPE2 (0)
#define LOOP_TYPE3 (1)



//v3.1
struct particleExistSt
{
	int  id;
	int	 cycle;
	int	 exist;
    int	 born;
	long stime;
	long endtime;
};


//v3.1
struct emitPattern
{
	int   offsetTime;//0�t���[������̊J�n�I�t�Z�b�g
	int   life;
    int   cycle;
};


//�ŏI�`��p�f�[�^
struct particleDrawData
{
	int	id;
	int	pid;
	int	stime;		//�������ꂽ�J�n����
	int lifetime;

	//�`��p���[�N
	float	x;
	float	y;
	float	rot;
	float	direc;

    SsU8Color color;
    SsVector2 scale;
};



//�G�~�b�^�[�����p�����[�^
//�G�f�B�^��t�@�C��������͂����
struct emitterParameter
{
	int	life;
	int interval;
	int	emitnum;		//��x�ɔr�o������
	int emitmax;		//�ő�r�o��
	int	particleLife;	//���������p�[�e�B�N���̎���
	int particleLife2;	//�����ő�l
    bool Infinite;      //��������

	int	loopStart;
	int loopEnd;
	int loopLen;
	int	loopGen;


	//�e�X�g�p�f�t�H���g
	emitterParameter() : life(15),interval(1),emitnum(2),emitmax(32),particleLife(15),particleLife2(15),Infinite(false){}

};


//�p�[�e�B�N�������p�����[�^
//�G�f�B�^��t�@�C��������͂����
struct particleParameter
{

	SsVector2 	scale;

	SsU8Color   startcolor; //�X�^�[�g���̃J���[
	SsU8Color   endcolor;   //�I�����̃J���[

	//����
	float		speed;		//����
	float		speed2;		//�����ő�l


	float		angle;		  //�����Ă�p�x
	float       angleVariance;//�ύ�

	bool		useGravity;
	SsVector2	gravity;


	bool		useOffset;
	SsVector2   offset;
	SsVector2   offset2;

	bool        useRotation;
	float		rotation;
	float		rotation2;

	float		rotationAdd;
	float		rotationAdd2;

	bool		useRotationTrans;
	float		rotationFactor;
	float		endLifeTimePer;

	bool        useTanAccel;
	float		tangentialAccel;
	float		tangentialAccel2;

	bool        useColor;
	SsU8Color   initColor;
	SsU8Color   initColor2;

	bool		useTransColor;
	SsU8Color   transColor;
	SsU8Color   transColor2;

	bool        useInitScale;
	SsVector2   scaleRange;
	float		scaleFactor;
	float		scaleFactor2;

	bool        useTransScale;
	SsVector2   transscale;
	SsVector2   transscaleRange;
	float		transscaleFactor;
	float		transscaleFactor2;

	float		delay;

	bool		usePGravity;
	SsVector2	gravityPos;
	float		gravityPower;

	bool		useAlphaFade;
	float       alphaFade;
	float       alphaFade2;

	bool		useTransSpeed;
	float	   	transSpeed;
	float	   	transSpeed2;

	bool		useTurnDirec;

    bool		userOverrideRSeed;
	int			overrideRSeed;


	particleParameter(){}
};


//�G�~�b�^����N���X
class SsEffectEmitter
{
public:

	int					priority;

	emitterParameter  	emitter;
	particleParameter   particle;
	xorshift32			rand;


	int				emitterSeed;

	//�����p�̃����O�o�b�t�@
	//particleRingBuffer<particleLifeSt>	_tempbuf;

//	emitPattern*    	_emitpattern;
	std::vector<emitPattern>    	_emitpattern;


    particleExistSt*     particleExistList;


	//���O�v�Z�o�b�t�@
	//particleLifeSt*				particleList;
	int							particleIdMax;

	size_t						particleListBufferSize;
    unsigned long*              seedList;


	SsVector2   				position;
//	SsEffectEmitter*			_child;
	SsEffectEmitter*			_parent;



    int							_parentIndex;

	SsCell*						refCell;    //�`��p�Z��
	SsEffectBehavior*           refData;	//�f�[�^�X�V�p

	size_t						globaltime;


public:
	SsEffectEmitter() :
//			particleList(0),
			_parentIndex(-1),
			seedList(0),
			particleListBufferSize(180*100),  //�����o����p�[�e�B�N���̍ő�l
			_emitpattern(0),
			particleExistList(0)
	{
		emitterSeed = SEED_MAGIC;
	}
	virtual ~SsEffectEmitter(){}



//	const particleLifeSt*	getParticleDataFromID(int id) { return &particleList[id]; }

#if  LOOP_TYPE3

	int	getParticleIDMax() { return _emitpattern.size(); }
	const 	particleExistSt*	getParticleDataFromID(int id);
	void	updateEmitter( double time );

#else

	int	getParticleIDMax() { return particleIdMax; }
	const particleLifeSt*	getParticleDataFromID(int id);
#endif


	int	getTimeLength() { return emitter.life + ( emitter.particleLife + emitter.particleLife2); }

	//���ݎ��Ԃ���Y�o�����ʒu�����߂�
	//time�ϐ����狁�߂��鎮�Ƃ���
	void	updateParticle(float time, particleDrawData* p, bool recalc = false );

	//�p�[�e�B�N���̔����Ԋu�����O�v�Z����
	//�����ŏo�͂��m�肷��

	void	precalculate2();

};


class SsEffectRenderV3 //: public ISsEffectRenderer
{
public:

	//�G�t�F�N�g�̃p�����[�^�f�[�^
	SsEffectModel*		effectData;

	//Model�ɋL�ڂ���Ă���G�~�b�^�̃��X�g
	std::vector<SsEffectEmitter*>   emmiterList;

	std::vector<SsEffectEmitter*>   updateList;

	//�����_���V�[�h
	u32				mySeed;

	SsVector3		layoutPosition;

	float			nowFrame;
    float			secondNowFrame;

	size_t          effectTimeLength;

    bool			Infinite;	//�����ɔ����o���邩�ǂ���

	SsPartState*		parentState;


public:
	//�f�o�b�O�p
	int  loopcnt;
	int  drawcnt;


protected:
	void 	particleDraw(SsEffectEmitter* e , double t , SsEffectEmitter* parent = 0 , particleDrawData* plp = 0 );
	void	initEmitter( SsEffectEmitter* e , SsEffectNode* node);

	void	clearEmitterList();



public:
	SsEffectRenderV3() : effectTimeLength(0) {}
	virtual ~SsEffectRenderV3(){}


	virtual void	setEffectData(SsEffectModel* data);


	virtual void	setSeed( u32 seed )
	{
   		mySeed = seed * SEED_MAGIC;
	}


	virtual void	setFrame( float frame )
	{
    	nowFrame = frame;
	}

    virtual float	getFrame(){ return nowFrame; }

	virtual void	update(float delta);
	virtual void	draw();

	virtual void    reload();

    virtual size_t  getEffectTimeLength();

//	virtual int		getVersion(){ return EFFECTRENDERVERSION_V2; }

	virtual  void	setParentAnimeState( SsPartState* state ){ parentState = state; }

	virtual void stop(){}

};


#endif
