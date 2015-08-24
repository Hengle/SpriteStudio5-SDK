#include <stdio.h>
#include <cstdlib>

#include "../loader/ssloader.h"

#include "ssplayer_animedecode.h"
#include "ssplayer_effect.h"
#include "ssplayer_macro.h"
#include "ssplayer_matrix.h"
#include "ssplayer_render.h"
#include "ssplayer_effectfunction.h"


#if 0
#include "SsPch.h"
#pragma hdrstop



//---------------------------------------------------
//     OpenGL
//---------------------------------------------------
#ifdef _WIN32
#include "lib/glew/glew.h"	// windows �ł� GL_CLAMP_TO_EDGE ��`�̂��߂ɕK�v�B
#endif

// GLEW
#include "lib/glew/glew.h"
#ifdef _WIN32
#pragma link "lib/glew/glew32.lib"
#pragma link "lib/glew/glew32_2.lib"
#endif
#ifdef __APPLE__
#pragma link "lib/glew/ssglew.dylib"
#endif


#include "SsOpenGLRender.h"


#include "SsCell.h"
#include "SsCellMap.h"
#include "SsImage.h"
#include "SsOpenGLTexture.h"


#include "SsPartState.h"

#include "SsMatrix.h"

//-------------------------------------------


#include "SsEffect.h"
#include "SsParticleMethods.h"


#include "SsUtility.h"

#include "SsEffectRenderer.h"

/*
enum SsRenderType{
    BaseNode,
	EmmiterNode,
    ParticleNode,
};
*/

#endif


//static SsEffectRenderAtom* CreateAtom( unsigned int seed , SsEffectRenderAtom* parent , SsEffectNode* node );
class SsEffectRenderParticle;


static  int seed_table[] =
{
	485, 583, 814, 907, 1311, 1901, 2236, 3051, 3676, 4338,
	4671, 4775, 4928, 4960, 5228, 5591, 5755, 5825, 5885, 5967, 6014, 6056,
	6399, 6938, 7553, 8280, 8510, 8641, 8893, 9043, 31043,
};
//--------



#define ONEFRAME ( 1.0f / 60.0f )



//------------------------------------------------------------------------------
//	���[�e�B���e�B
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�v�f�����֐�
//------------------------------------------------------------------------------
SsEffectRenderAtom* SsEffectRenderer::CreateAtom( unsigned int seed , SsEffectRenderAtom* parent , SsEffectNode* node )
{
	SsEffectRenderAtom* ret = 0;
	SsEffectNodeType::_enum type = node->GetType(); 


	if ( type == SsEffectNodeType::particle )
	{
#if PFMEM_TEST
		if ( SSEFFECTRENDER_PARTICLE_MAX <= pa_pool_count )
		{
			 return 0;
		}
		SsEffectRenderParticle* p = &pa_pool[pa_pool_count];
		p->InitParameter();
		pa_pool_count++;

		p->data = node;
		p->parent = parent;

#else
		SsEffectRenderParticle* p = new SsEffectRenderParticle( node , parent );
#endif

		updatelist.push_back( p );
		createlist.push_back( p );
		SsEffectRenderEmitter*	em = (SsEffectRenderEmitter*)parent;
        em->myBatchList->drawlist.push_back( p );

		ret = p;
	}


	if ( type == SsEffectNodeType::emmiter )
	{

#if PFMEM_TEST
		if ( SSEFFECTRENDER_EMMITER_MAX <= em_pool_count )
		{
			return 0;
		}
		if ( SSEFFECTRENDER_EMMITER_MAX <= dpr_pool_count ){
			 return 0;
		}
		SsEffectRenderEmitter* p = &em_pool[em_pool_count];
		SsEffectDrawBatch* bl = &drawPr_pool[dpr_pool_count];

		p->InitParameter();
		em_pool_count++;
		dpr_pool_count++;

		p->data = node;
		p->parent = parent;
		p->myBatchList = bl;
#else
		SsEffectRenderEmitter* p = new SsEffectRenderEmitter( node , parent);
#endif
		p->setMySeed( seed );
		p->TrushRandom( em_pool_count%9 );

		//p->data->behavior.initalize(p);
		SsEffectFunctionExecuter::initalize( &p->data->behavior , p );

		//�Z���f�[�^�̌����ƃZ�b�g
		//�I���W�i���ł͏�Linitialize�ł���Ă��邪�N���X�K�w�̊֌W���炱����ł��
		SsCelMapLinker* link = this->curCellMapManager->getCellMapLink( p->data->behavior.CellMapName );
		if ( link )
		{
			SsCell * cell = link->findCell( p->data->behavior.CellName );
		
			getCellValue(	this->curCellMapManager , 
				p->data->behavior.CellMapName ,
				p->data->behavior.CellName , 
				p->dispCell ); 
		}

		updatelist.push_back( p );
		createlist.push_back( p );
		drawBatchList.push_back( bl );

		ret = p;
	}
	return ret;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool particleDelete(SsEffectRenderAtom* d)
{

	if ( d->m_isInit )
	{
		if ( d->m_isLive == false )
		{
		   //	delete d;
			return true;
		}

		if ( d->_life <= 0.0f)
		{
			d->m_isLive = false;
			return true;
		}
	}

    return false;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool particleDeleteAll(SsEffectRenderAtom* d)
{
	delete d;
	return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderEmitter::setMySeed( unsigned int seed )
{

	if ( seed > 31 ){
		this->MT.init_genrand( seed );

	}else{
		this->MT.init_genrand( seed_table[seed] );
	}
	myseed = seed ;
}


//----------------------------------------------------------------------
//�����t�F�[�Y           SsEffectRenderer�ֈړ����Ă���������
//----------------------------------------------------------------------
void	SsEffectRenderEmitter::Initialize()
{
	SsEffectNode* n = static_cast<SsEffectNode*>(this->data->ctop);

	if ( !m_isInit )
	{                                                                                                                                                 		//�q�v�f�����(��x�����j
		while ( n )
		{
			if ( n->GetType() ==  SsEffectNodeType::particle )
			{
				param_particle = n;
			}

			n = static_cast<SsEffectNode*>(n->next);
		}

		if (this->data->GetMyBehavior())
		{
			//this->data->GetMyBehavior()->initalize( this );
			SsEffectFunctionExecuter::initalize( this->data->GetMyBehavior() , this );
		}
        intervalleft = this->interval;
	}


	m_isInit = true;
}

//----------------------------------------------------------------------
//�p�[�e�B�N���I�u�W�F�N�g�̐���
//----------------------------------------------------------------------
bool	SsEffectRenderEmitter::genarate( SsEffectRenderer* render )
{

	if ( !generate_ok )return true;
	if ( m_isLive == false ) return true;

	int create_count = this->burst;
	if ( create_count <= 0 ) create_count = 1;


	int pc = particleCount;

	while(1)
	{
		if ( this->intervalleft >= this->interval )
		{
			for ( int i = 0 ; i < create_count; i++)//�ő�쐬��
			{
				if ( pc < maxParticle )
				{
					if ( param_particle )
					{
						SsEffectRenderAtom* a = render->CreateAtom( 0 , this , param_particle );
						if ( a )
						{
							a->Initialize();
							a->update(render->frameDelta);
							bool ret = a->genarate(render);
							pc++;
							if ( ret == false ) return false;
						}else{
							return false;
						}
					}
				}
			}
			this->intervalleft-=this->interval;
			if ( this->interval == 0 )return true;
		}else{
			return true;
		}
	}
	return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderEmitter::update(float delta)
{


	_exsitTime+=delta;

	_life = _lifetime - _exsitTime;
	intervalleft+=delta;

	if ( this->parent )
	{
		//�ȉ��͉�
		this->position = this->parent->position;
		this->rotation = this->parent->rotation;
		this->scale = this->parent->scale;
		this->alpha = this->parent->alpha;

	}
	if (this->data->GetMyBehavior())
	{
//		this->data->GetMyBehavior()->updateEmmiter(this);
		SsEffectFunctionExecuter::initalize( this->data->GetMyBehavior() , this );

	}

	if ( this->myBatchList )
	{
		this->myBatchList->priority = this->drawPriority;
		this->myBatchList->dispCell = &this->dispCell;
		this->myBatchList->blendType = this->data->GetMyBehavior()->blendType;

	}

}


//----------------------------------------------------------------------
//�p�[�e�B�N���N���X
//----------------------------------------------------------------------
//�����t�F�[�Y
void	SsEffectRenderParticle::Initialize()
{


	if ( !m_isInit )
	{
		SsEffectNode* n = static_cast<SsEffectNode*>(this->data->ctop);

		//�q�v�f�����  ��{�I�ɃG�~�b�^�[�݂̂̐����̂͂��@�i�o�ł̓G���[�ł����j
		//�������Ȃ��ăG�~�b�^�[�����̂���ō쐬����
		//�p�[�e�B�N���ɕR�Â����G�~�b�^�[�����������
		parentEmitter = 0;

		parentEmitter = static_cast<SsEffectRenderEmitter*>(this->parent);

		dispCell = &parentEmitter->dispCell;
		if ( parentEmitter->data == 0 )
		{
			this->_life = 0.0f;
			m_isInit = false;
			return ;
		}

		this->refBehavior = parentEmitter->data->GetMyBehavior();
		if ( refBehavior )
		{
			 //refBehavior->initializeParticle( parentEmitter , this );
			 SsEffectFunctionExecuter::initializeParticle( refBehavior , parentEmitter , this );

		}
	}

	m_isInit = true;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool	SsEffectRenderParticle::genarate( SsEffectRenderer* render )
{
	SsEffectNode* n = static_cast<SsEffectNode*>(this->data->ctop);
	if ( m_isInit && !m_isCreateChild)
	{
		if ( parentEmitter )
		{
			while ( n )
			{
				if ( parentEmitter == NULL ) return true;
				SsEffectRenderAtom* r = render->CreateAtom( parentEmitter->myseed , this , n );
				if ( r )
				{
					n = static_cast<SsEffectNode*>(n->next);
					r->Initialize();
					r->update( render->frameDelta );
					r->genarate( render );
				}else{
					return false;
				}
			}
		}

		m_isCreateChild = true;
	}

	return true;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderParticle::update(float delta)
{


	 //_rotation = 0;
     if ( !this->isInit() )return ;
	 this->position.x = this->_position.x;
	 this->position.y = this->_position.y;
	 this->scale = this->parent->scale;
	 this->alpha = this->parent->alpha;

	 //�����l�˂�����ł����A�p�[�e�B�N�����\�b�h�̃A�b�v�f�[�g�Ɏ����Ă��H
	 this->_color = this->_startcolor;

	//this->parent
	if ( parentEmitter )
	{
    	updateDelta( delta );

		if ( refBehavior )
		{
			 //refBehavior->updateParticle( parentEmitter , this );
			 SsEffectFunctionExecuter::updateParticle( refBehavior , parentEmitter , this );
		}

		updateForce( delta );

		if (parent->_life <= 0.0f)
		{
//			_life = 0.0f;
		}else{
			//��
			this->position.x = this->_position.x;
			this->position.y = this->_position.y;
		}

	}

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderParticle::updateDelta(float delta)
{
	_rotation+=( _rotationAdd*delta );

	_exsitTime+=delta;
	_life = _lifetime - _exsitTime;

	SsVector2	tangential = SsVector2( 0 , 0 );

	//�ڐ������x�̌v�Z
	SsVector2  radial = SsVector2(this->_position.x,this->_position.y);

    SsVector2::normalize( radial , &radial );
	tangential = radial;

    radial = radial * _radialAccel;//SsVector2( radial.x * radialAccel , radial.x * radialAccel );

	float newY = tangential.x;
	tangential.x = -tangential.y;
	tangential.y = newY;

	tangential = tangential* _tangentialAccel;// SsVector2( tangential.x * tangentialAccel , tangential.x * tangentialAccel );

	SsVector2 tmp = radial + tangential;

	this->_execforce = tmp;


}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void 	SsEffectRenderParticle::updateForce(float delta)
{

	this->_backposition = this->_position;

	this->_force = _gravity;// * _exsitTime;
	SsVector2 ff = (this->vector * this->speed) + this->_execforce + this->_force;


	if ( isTurnDirection )
	{
		this->direction =  SsPoint2::get_angle_360( SsVector2( 1.0f , 0.0f ) , ff ) - (float)DegreeToRadian(90);
	}else{
        this->direction = 0;
	}

	//�t�H�[�X�����Z
	this->_position.x+= (ff.x * delta );
	this->_position.y+= (ff.y * delta );

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderParticle::draw(SsEffectRenderer* render)
{

	if ( this->parentEmitter == NULL  )return;
	if ( refBehavior == NULL ) return;
	if ( dispCell == NULL ) return ;
	if ( dispCell->texture == NULL ) return;

	float		matrix[4 * 4];	///< �s��
	IdentityMatrix( matrix );

	if ( render->parentState )
	{
		MultiplyMatrix( matrix ,render->parentState->matrix ,matrix ); 
	}
	TranslationMatrixM( matrix , _position.x, _position.y, 0.0f );

	RotationXYZMatrixM( matrix , 0 , 0 , DegreeToRadian(_rotation)+direction );

    ScaleMatrixM(  matrix , _size.x, _size.y, 1.0f );

	SsFColor fcolor;
	fcolor.fromARGB( _color.toARGB() );
	fcolor.a = fcolor.a * this->alpha;


	SsVector2 pivot = SsVector2( dispCell->cell->pivot.x ,dispCell->cell->pivot.y);

	pivot.x = pivot.x * dispCell->cell->size.x;
	pivot.y = pivot.y * dispCell->cell->size.y;

	SsVector2 dispscale = dispCell->cell->size;


	SsCurrentRenderer::getRender()->renderSpriteSimple(
		matrix,
		dispscale.x , dispscale.y ,  pivot,
				dispCell->uvs[0],
				dispCell->uvs[3], fcolor );

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderer::setFrame( float frame )
{

	reload();

	float addf = ONEFRAME * 2.0f;
	float timef = (1.0f / 30.0f) * frame;
	float addt = timef / 10.0f;
	addf = 0.5f;

	if ( frame == 0 )
	{
		update( 0 );
		return ;
	}

	float t = 0.0f;
	for ( int i = 0 ; i <= (int)(frame*2) ; i ++ )
	{
		if ( i == 0 )
		{
			update( 0 );
		}else{
			update( addf );
		}

	}

}
//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------

bool compare_priority( SsEffectDrawBatch* left,  SsEffectDrawBatch* right)
{
  //	return true;
  return left->priority < right->priority ;
}
//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------

void	SsEffectRenderer::update(float delta)
{

	if (m_isPause) return;
	if (!m_isPlay) return;
    if ( this->render_root == 0 ) return ;

	frameDelta = delta;

	if ( parentState )
	{
		
		SsVector3 pos = SsVector3( parentState->matrix[3*4] ,
								   parentState->matrix[3*4+1] ,
								   parentState->matrix[3*4+2] );

		layoutPosition = pos;

		this->render_root->setPosistion( 0 , 0 , 0 );

		this->render_root->rotation = 0;
		this->render_root->scale = SsVector2(1.0f,1.0f);
		this->render_root->alpha = parentState->alpha;
	}

	size_t loopnum = updatelist.size();
	for ( size_t i = 0 ; i < loopnum ; i++ )
	{
		SsEffectRenderAtom* re = updatelist[i];
		re->Initialize();
		re->count();
	}

	loopnum = updatelist.size();
	size_t updatecount = 0;
	for ( size_t i = 0 ; i < loopnum ; i++ )
	{
		SsEffectRenderAtom* re = updatelist[i];

		if ( re->m_isLive == false ) continue;

		if ( re->parent && re->parent->_life  <= 0.0f || re->_life <= 0.0f )
		{
			re->update(delta);
		}else{
			re->update(delta);
			re->genarate(this);
		}

		updatecount++;
	}

	//�㏈��  �����ō폜
	//���S���o�A�폜��2�i�K
	std::vector<SsEffectRenderAtom*>::iterator endi = remove_if( updatelist.begin(), updatelist.end(), particleDelete );
    updatelist.erase( endi, updatelist.end() );

	drawBatchList.sort(compare_priority);


	if ( m_isLoop )
	{
		if ( updatecount== 0)
		{
		  reload();
		}
	}




}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderer::draw()
{
	SsCurrentRenderer::getRender()->renderSetup();					

	foreach( std::list<SsEffectDrawBatch*> , drawBatchList , e )
	{
		//�Z�b�g�A�b�v
		if ( (*e)->dispCell )
		{
			switch( (*e)->blendType )
			{
				case SsRenderBlendType::Mix:
					SsCurrentRenderer::getRender()->SetAlphaBlendMode(SsBlendType::mix);					
					break;
				case SsRenderBlendType::Add:
					SsCurrentRenderer::getRender()->SetAlphaBlendMode(SsBlendType::add);					
					break;
			}
			SsCurrentRenderer::getRender()->SetTexture( (*e)->dispCell );
		}

		foreach( std::list<SsEffectRenderAtom*> , (*e)->drawlist , e2 )
		{
			if ( (*e2) )
			{
				if ( !(*e2)->m_isLive ) continue;
				if ( (*e2)->_life <=0.0f ) continue;

				(*e2)->draw(this);
			}
		}

	}



}



//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
SsEffectRenderer::~SsEffectRenderer()
{
	clearUpdateList();

	delete render_root;
	render_root = 0;

}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderer::clearUpdateList()
{


	size_t s = createlist.size();
	size_t s2 = updatelist.size();


#if PFMEM_TEST
	em_pool_count = 0;
	pa_pool_count = 0;
	dpr_pool_count = 0;

#else
	for ( size_t i = 0 ; i <  createlist.size() ; i++ )
	{
		delete createlist[i];
	}
#endif

	updatelist.clear();
	createlist.clear();

	foreach( std::list<SsEffectDrawBatch*> , drawBatchList , e )
	{
		(*e)->drawlist.clear();
	}

	drawBatchList.clear();

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

void    SsEffectRenderer::reload()
{
	clearUpdateList();

	//���W����̂��߂̃��[�g�m�[�h���쐬����
	if ( render_root == 0 )
	{
		render_root = new SsEffectRenderAtom();
	}

	//���[�g�̎q�v�f�𒲍����č쐬����
	SsEffectNode* root = this->effectData->GetRoot();

	//�V�[�h�l�̌���
	u32 seed = 0;

	if ( this->effectData->isLockRandSeed )
	{
    	seed = this->effectData->lockRandSeed;
	}else{
        seed = mySeed;
	}

	SimpleTree* n = root->ctop;
	//�q�v�f���������Ă������ɂ���
	while( n )
	{
		SsEffectNode* enode = static_cast<SsEffectNode*>(n);
		SsEffectRenderAtom* effectr = CreateAtom( seed , render_root , enode );

		n = n->next;
	}

}

void    SsEffectRenderer::play()
{
	m_isPlay = true;
	m_isPause = false;




}
	
void	SsEffectRenderer::stop()
{
	m_isPlay = false;

}
	
void    SsEffectRenderer::pause()
{
	m_isPause = true;

}

void	SsEffectRenderer::setLoop(bool flag)
{
	m_isLoop = flag;
}







