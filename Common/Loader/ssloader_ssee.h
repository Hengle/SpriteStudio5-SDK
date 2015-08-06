#ifndef __SSLOADER_SSEE__

#include "sstypes.h"
#include "ssarchiver.h"

#include "effect/SsEffectBehavior.h"



class SimpleTree
{
public:
	SimpleTree *parent;
	SimpleTree *ctop;
	SimpleTree *prev;
	SimpleTree *next;

public:
	SimpleTree()
		: parent(0),ctop(0),prev(0),next(0)
	{}
	~SimpleTree()
	{
		destroy();
	}

	void	addChildEnd( SimpleTree* c ){ 
		if ( ctop == 0 )
		{
			ctop = c; 
		}else{
			ctop->addSiblingEnd( c );
		}
		c->parent = this;
	}
	void	addSiblingEnd( SimpleTree* c )
	{
		if ( next == 0 )
		{
			c->prev = this;
			next = c;
		}else{
			next->addSiblingEnd(c);
		}

		c->parent = this->parent;
	}

	void destroysub(SimpleTree *t)
	{
		if ( t->ctop )
		{
			destroysub(t->ctop);
		}
		if ( t->next )
		{
			destroysub(t->next);
		}

		this->ctop = 0;
		this->next = 0;
		this->prev = 0;
		delete this;
	}
	void destroy()
	{
		destroysub(this);	
	}
};






class SsEffectNode : public SimpleTree
{
public:
	int						arrayIndex;
	int						parentIndex;
	SsEffectNodeType::_enum	type;
	bool					visible;

	SsEffectBehavior		behavior;

public:
	SsEffectNode() :
	  arrayIndex(0), parentIndex(0),	
		  type(SsEffectNodeType::invalid)
	{}
	~SsEffectNode(){}

	SSSERIALIZE_BLOCK
	{
		SSAR_DECLARE(arrayIndex);
		SSAR_DECLARE(parentIndex);		
		SSAR_DECLARE_ENUM(type);	
		SSAR_DECLARE(visible);	
		SSAR_STRUCT_DECLARE(behavior);	
	}
};


class SsEffectModel
{
private:
	SsEffectNode* root;
	std::vector<SsEffectNode*> nodeList;

public:
	int			lockRandSeed; 	 // �����_���V�[�h�Œ�l
	bool    	isLockRandSeed;  // �����_���V�[�h���Œ肷�邩�ۂ�
	int			fps;             //
	SsString	   bgcolor;

public:
	SsEffectModel()
	{}

	virtual ~SsEffectModel(){
		delete root;
		root = 0;
	}

    //�A�N�Z�X
	SsEffectNode* GetRoot(){ return root;}


	//�V���A���C�U
	SSSERIALIZE_BLOCK
	{
		SSAR_DECLARE(lockRandSeed);
		SSAR_DECLARE(isLockRandSeed);
		SSAR_DECLARE(fps);
		SSAR_DECLARE(bgcolor);

		SSAR_DECLARE_LISTEX(nodeList,"node"); 

		EffectNodeLoader(ar);
	}

	void	EffectNodeLoader(ISsXmlArchiver* ar);


};




class SsEffectFile
{
public:
	SsEffectModel	   effectData;  //�e�q�\���{�e�A�g���r���[�g


	SsEffectFile(){}
	virtual ~SsEffectFile(){}

	SSSERIALIZE_BLOCK
	{
		SSAR_STRUCT_DECLARE( effectData );
	}

};


//!sspj�̃��[�_�[�N���X�ł��B
class ssloader_ssee
{
public:
	ssloader_ssee(){}
	virtual ~ssloader_ssee(){}

	static SsEffectFile*	Load(const std::string& filename );

};

#endif
