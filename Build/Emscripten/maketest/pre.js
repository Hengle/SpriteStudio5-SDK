Module['preRun'] = function () {
/*
	//������sspj��{�����Ďg���t�@�C�����C���[�W�Ƃ��Đ������Ă��܂��΂n�j�̂͂�
    FS.createFolder(
        '/', // �e�t�H���_�̎w��
        'GLSL', // �t�H���_��
        true, // �ǂݍ��݋���
        true // �������݋���(����̗��false�ł��悳��)
    );
	
    FS.createPreloadedFile(
        '/GLSL',
        'sprite.vs',
        'GLSL/sprite.vs',  // http�ŃA�N�Z�X����ۂ�URL���w��
        true,
        false
    );
    FS.createPreloadedFile(
        '/GLSL',
        'sprite.fs',
        'GLSL/sprite.fs',  // http�ŃA�N�Z�X����ۂ�URL���w��
        true,
        false
    );
    FS.createPreloadedFile(
        '/GLSL',
        'sprite.fs',
        'GLSL/sprite_pot.fs',  // http�ŃA�N�Z�X����ۂ�URL���w��
        true,
        false
    );
*/

    FS.createPreloadedFile(
        '/',
        'input.sspj',
        'data/NewProject.sspj',  // http�ŃA�N�Z�X����ۂ�URL���w��
        true,
        false
    );

    FS.createPreloadedFile(
        '/',
        'chr01.ssae',
        'data/chr01.ssae',  // http�ŃA�N�Z�X����ۂ�URL���w��
        true,
        false
    );

    FS.createPreloadedFile(
        '/',
        'chr_00.ssce',
        'data/chr_00.ssce',  // http�ŃA�N�Z�X����ۂ�URL���w��
        true,
        false
    );

    FS.createPreloadedFile(
        '/',
        'chr_00.png',
        'data/chr_00.png',  // http�ŃA�N�Z�X����ۂ�URL���w��
        true,
        false
    );	
};
