// emtest2.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

//#include "stdafx.h"
#include "tinyxml2.h"
#include <emscripten.h>

//JS������^����ꂽXML�t�@�C�������󂯎�b���̂w�l�k�p�[�T�[���g�p�\���ǂ������`�F�b�N����v���O����
extern "C" {
	int MergeFile(const char* x);
};

//���C�����[�v
void one_iter() {
	static int status = 0;
	using namespace tinyxml2;

	switch(status)
	{
	case 0://project file �ǉ��҂����Ď�
		{
			XMLDocument		xml;
			if ( XML_SUCCESS == xml.LoadFile("input.sspj") )
			{
				printf( "XML_SUCCESS" );
				xml.Print();
				status = 1;
			}else{
				printf( "XML_ERROR_FILE_NOT_FOUND" );
			}
		}
		break;
	case 1:
		{

		}
		break;
	}
}

int main(void)
{
	//�g�p����t�@�C���̒ǉ�
	MergeFile( "input.sspj" );

	emscripten_set_main_loop(one_iter, 16,1);

	return 0;
}

