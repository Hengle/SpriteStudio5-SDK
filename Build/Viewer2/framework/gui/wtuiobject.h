#ifndef __WTUIOBJECT__
#define __WTUIOBJECT__

#include "../wtobject.h"

struct NVGcontext;
//UI�p�[�c�\�z�p�̊�{�N���X
class _wtUIObject : public wtObjectBase
{
private:
	bool	m_pause;//��~���
	bool	m_show;	//�\�����
	bool	m_dead;	//�j���\��


public:
	NVGcontext* vg;


	_wtUIObject(){}
	virtual ~_wtUIObject(){}

	virtual void	setPause( bool p ){ m_pause = p ; } 
	virtual bool	isPause(){ return m_pause; } 

	virtual void	setShow( bool p ){ m_show = p ; } 
	virtual bool	isShow(){ return m_show; } 

	virtual void	setDead( bool p ){ m_dead = p ; } 
	virtual bool	isDead(){ return m_dead; } 

	//�����p���z�֐�
	virtual void	draw(){}
	virtual void	before_update(){}
	virtual void	update(){}
	virtual void	after_update(){}


	//�C�x���g���X�i����p
	virtual bool	isMouseInArea( int x , int y ){ return false; }

	//�C�x���g���X�i
	virtual void	mouseDown(int x , int y, int button ){}
	virtual void	mouseUp(int x , int y, int button ){}
	virtual void	mouseMove(int x , int y){}

	virtual void	onCreate(){}
	virtual void	onDestroy(){}

};

typedef treeitem<_wtUIObject> wtUIObject;

/*
class wtUIObjectExecuter
{
private:
	wtUIObject	*root;
	
public:
	wtUIManager(){}
	virtual ~wtUIManager(){}

};
*/


#endif
