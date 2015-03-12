// plistConverter.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

void gettexturesize(void);
void getcelldata(void);
void outputcelldata(void);

bool err = false;	//�G���[���o���珈�����f
std::string open_filename;
std::string output_filename;
bool option_offset = true;

int tex_size_w = 0;	//�e�N�X�`���T�C�Y
int tex_size_h = 0;	//�e�N�X�`���T�C�Y

//�Z���f�[�^
struct CELLDATA
{
	int x;
	int y;
	int w;
	int h;
	int offset_x;
	int offset_y;
	std::string name;
};

#define SELLDATA_MAX 3000
CELLDATA celldata[SELLDATA_MAX];
int cell_count = 0;

int main(int argc, char* argv[])
{
	//�����`�F�b�N
	int i;
	bool ishelp = false;
	int inputcount = 0;

	if (argc == 1)
	{
		ishelp = true;
	}
	for (i = 1; i < argc; i++ )
	{
		if (
		     (strcmp(argv[i], "-h") == 0)
		  || (strcmp(argv[i], "-H") == 0)
		   )
		{
			//�w���v�\��
			ishelp = true;
		}
		else if (
			   (strcmp(argv[i], "-o") == 0)
			|| (strcmp(argv[i], "-O") == 0)
			)
		{
			option_offset = false;
		}
		else
		{
			if (inputcount == 0)
			{
				open_filename = argv[i];
				inputcount++;
			}
			else
			{
				output_filename = argv[i];
			}
		}
	}
	//�g���q�`�F�b�N
	if (open_filename.find(".plist") == std::string::npos)
	{
		err = true;	//�G���[���o���珈�����f
	}

	if ( inputcount == 1 )
	{
		//�o�̓t�@�C�������ȗ�����Ă���ꍇ
		output_filename = open_filename.substr(0, open_filename.length() - 5);
		output_filename = output_filename + "xml";
	}



	if (ishelp == true)
	{
		printf("�������Fplist�̃p�X\r\n");
		printf("�������F�o�̓t�@�C���̃p�X�i�ȗ��j\r\n");
		printf("�I�v�V����\r\n");
		printf("-o�F�I�t�Z�b�g�o�͂Ȃ�\r\n");
		printf("-h�F�w���v�\��\r\n");
	}
	else
	{
		if (err == false)
		{
			gettexturesize();	//�e�N�X�`���̃T�C�Y�擾
			getcelldata();		//�Z�����̎擾
			outputcelldata();	//�Z�����̏����o��
		}
	}

	return 0;
}

//�e�N�X�`���T�C�Y�̎擾
void gettexturesize(void)
{
	FILE *fp;
	char buf[1024];
	// �t�@�C���̃I�[�v��
	fopen_s(&fp, open_filename.c_str(), "r");
	if (fp == NULL)
	{
		//�G���[
		printf("file open error!!\n");
		err = true;
	}
	if ( err == false )
	{
		int seq = 0;
		int yomitobasi = 0;
		//�P�s�P�ʂœǂݏo��
		while (fgets(buf, 1024, fp) != NULL)
		{
			std::string str(buf);

			switch( seq )
			{
			case 0:
				//metadata�^�O��T��
				{
					unsigned int loc = str.find("metadata", 0);
					if (loc != std::string::npos)
					{
						//��������
						seq++;
						yomitobasi = 0;
					}
				}
				break;
			case 1:
				//6�s�ǂݔ�΂�
				yomitobasi++;
				if (yomitobasi == 6 )
				{
					seq++;
				}
				break;
			case 2:
				//�T�C�Y���擾����
				unsigned int loc1 = str.find("{", 0);
				unsigned int loc2 = str.find(",", 0);
				unsigned int loc3 = str.find("}", 0);
				if (
				     (loc1 != std::string::npos)
			      && (loc2 != std::string::npos)
				  && (loc3 != std::string::npos)
				   )
				{
					//��������
					//���l���擾
					std::string w = str.substr(loc1+1, loc2 - loc1-1);
					std::string h = str.substr(loc2+1, loc3 - loc2-1);

					tex_size_w = std::atoi(w.c_str());;	//�e�N�X�`���T�C�Y
					tex_size_h = std::atoi(h.c_str());;	//�e�N�X�`���T�C�Y
				}
				break;
			}
		}
	}
	fclose(fp);	//�t�@�C���̃N���[�Y
}

//�Z���f�[�^�̎擾
void getcelldata(void)
{
	FILE *fp;
	char buf[1024];
	// �t�@�C���̃I�[�v��
	fopen_s(&fp, open_filename.c_str(), "r");
	if (fp == NULL)
	{
		//�G���[
		printf("file open error!!\n");
		err = true;
	}
	if (err == false)
	{
		cell_count = 0;
		int seq = 0;
		int yomitobasi = 0;
		//�P�s�P�ʂœǂݏo��
		while (fgets(buf, 1024, fp) != NULL)
		{
			std::string str(buf);

			switch (seq)
			{
			case 0:
			//metadata�^�O��T��
			{
				unsigned int loc = str.find("<key>frame</key>", 0);
				if (loc != std::string::npos)
				{
					//��������
					seq++;
				}
				break;
			}
			case 1:
			{
				//�T�C�Y���擾����
				unsigned int loc1 = str.find("{{", 0);
				unsigned int loc2 = str.find(",", loc1 + 2);
				unsigned int loc3 = str.find("},{", loc2 + 1);
				unsigned int loc4 = str.find(",", loc3 + 3);
				unsigned int loc5 = str.find("}}", loc4 + 1);
				if (
						(loc1 != std::string::npos)
					&& (loc2 != std::string::npos)
					&& (loc3 != std::string::npos)
					&& (loc4 != std::string::npos)
					&& (loc5 != std::string::npos)
					)
				{
					//��������
					//���l���擾
					std::string x = str.substr(loc1 + 2, loc2 - loc1 - 2);
					std::string y = str.substr(loc2 + 1, loc3 - loc2 - 1);
					std::string w = str.substr(loc3 + 3, loc4 - loc3 - 3);
					std::string h = str.substr(loc4 + 1, loc5 - loc4 - 1);

					celldata[cell_count].x = std::atoi(x.c_str());;	//�e�N�X�`���T�C�Y
					celldata[cell_count].y = std::atoi(y.c_str());;	//�e�N�X�`���T�C�Y
					celldata[cell_count].w = std::atoi(w.c_str());;	//�e�N�X�`���T�C�Y
					celldata[cell_count].h = std::atoi(h.c_str());;	//�e�N�X�`���T�C�Y

					seq++;
				}
				break;
			}
			case 2:
			//�I�t�Z�b�g���擾����
			{
				unsigned int loc = str.find("<key>offset</key>", 0);
				if (loc != std::string::npos)
				{
					//��������
					seq++;
				}
				break;
			}
			case 3:
			{
				//�I�t�Z�b�g���擾����
				unsigned int loc1 = str.find("{", 0);
				unsigned int loc2 = str.find(",", loc1 + 1);
				unsigned int loc3 = str.find("}", loc2 + 1);
				if (
					   (loc1 != std::string::npos)
					&& (loc2 != std::string::npos)
					&& (loc3 != std::string::npos)
					)
				{
					//��������
					//���l���擾
					std::string x = str.substr(loc1 + 1, loc2 - loc1 - 1);
					std::string y = str.substr(loc2 + 1, loc3 - loc2 - 1);

					celldata[cell_count].offset_x = std::atoi(x.c_str());	//�e�N�X�`���T�C�Y
					celldata[cell_count].offset_y = std::atoi(y.c_str());	//�e�N�X�`���T�C�Y

					char name[128];
					sprintf_s(name, 128, "frame%03d.png", cell_count);
					celldata[cell_count].name = name;	//���O

					cell_count++;
					seq = 0;	//���̃f�[�^���쐬����
				}
				break;
			}
			}
		}
	}
	fclose(fp);	//�t�@�C���̃N���[�Y
}

//�Z���f�[�^�̏o��
void outputcelldata(void)
{
	FILE *fp;
	wchar_t buf[1024];
	// �t�@�C���̃I�[�v��
	fopen_s(&fp, output_filename.c_str(), "w,ccs=UTF-8");
	if (fp == NULL)
	{
		//�G���[
		printf("file open error!!\n");
		err = true;
	}
	if (err == false)
	{
		//���P�[���w��
		setlocale(LC_ALL, "japanese");

		//�w�b�_�o��
		fputws(_T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"), fp);
		fputws(_T("<root version=\"1.0\">\n"), fp);

		swprintf_s(buf, 1024, _T("  <info w=\"%d\" h=\"%d\" />\n"), tex_size_w, tex_size_h);
		fputws(buf, fp);
		fputws(_T("  <textures>\n"), fp);

		int i;
		for (i = 0; i < cell_count; i++ )
		{
			//�ϊ�
			wchar_t	name[50];
			size_t wLen = 0;
			mbstowcs_s(&wLen, name, 20, celldata[i].name.c_str(), _TRUNCATE);

			//�Z���f�[�^�̏o��
			if (option_offset == false)
			{
				swprintf_s(buf, 1024, _T("    <texture filename=\"%s\" x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\" />\n"), name, celldata[i].x, celldata[i].y, celldata[i].w, celldata[i].h);
			}
			else
			{
				swprintf_s(buf, 1024, _T("    <texture filename=\"%s\" x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\" offset_x=\"%d\" offset_y=\"%d\" />\n"), name, celldata[i].x, celldata[i].y, celldata[i].w, celldata[i].h, celldata[i].offset_x, celldata[i].offset_y);
			}

			fputws(buf, fp);
		}
		//�t�b�_�o��
		fputws(_T("  </textures>\n"), fp);
		fputws(_T("</root>\n"), fp);
		fputws(_T("\n"), fp);

	}
	fclose(fp);	//�t�@�C���̃N���[�Y
}
