#pragma once

#include <cmath>
#include <iostream>
//#include <GL/glut.h>
//#include <GL/gl.h>
#include <GL/freeglut.h> 


#define PLOTSIZE (2)
#define SCALE (2.0 * 3.14159265358979323846)  // �}�E�X�̑��Έʒu����]�p�̊��Z�W��


namespace GLSpace {

	int LoopTime=0;

	//--------------------//
	//   �����X���b�h�p   //
	//--------------------//
	#define GL_COUNT 2
	int WinID[GL_COUNT]; //�E�B���h�EID
	const char *WindowName[]={"Window 1", "Window 2"};

	/*****************************************************
	 *   �`��֐��̕ϐ�������
	 ******************************************************/
	int WindowPositionX = 800;  //��������E�B���h�E�ʒu��X���W
	int WindowPositionY = 0;  //��������E�B���h�E�ʒu��Y���W
	int WindowWidth = 800;    //��������E�B���h�E�̕�
	int WindowHeight= 800;    //��������E�B���h�E�̍���
	char WindowTitle[] = "Result";  //�E�B���h�E�̃^�C�g��
	int plot=PLOTSIZE;
	int cx, cy;                // �h���b�O�J�n�ʒu
	double sx, sy;              // �}�E�X�̐�Έʒu���E�B���h�E���ł̑��Έʒu�̊��Z�W��
	double cq[4] = { 1.0, 0.0, 0.0, 0.0 };  // ��]�̏����l (�N�H�[�^�j�I��)
	double tq[4];              // �h���b�O���̉�] (�N�H�[�^�j�I��)
	double rt[16];              // ��]�̕ϊ��s��

	unsigned int listNumber;
	float camera_z_pos =0.0;
	float dist =2000.0f;
	int latest_mouse=-1;


	double l_scale =1.0;
	double z_scale =1.0;
	double z_slide =500/z_scale;

	/*****************************************************
	 *   �`��Ɋւ���֐��̃v���g�^�C�v�錾
	 ******************************************************/

	void timer(int value);
	void Initialize(void);
	void Idle(void);
	void Reshape(int _width, int _height);
	void Display(void);
	void Ground(void);  //��n�̕`��

	void qmul(double r[], const double p[], const double q[]);
	void qrot(double r[], double q[]);
	void mouse_motion(int x, int y);
	void mouse_on(int button, int state, int x, int y);
	void keyboard(unsigned char key, int x, int y);
	void specialkeyboard(int key, int x, int y);

	bool hsv2rgbColor(const int h_tmp, const int maxL, const int minL, const double s=255, const double v=255);
	bool hsv2rgb(double &r, double &g, double &b, const int h_tmp, const int maxL, const int minL, const double s=255, const double v=255);


	void setRTs(double rt0, double rt1, double rt2, double rt3, double rt4, double rt5, double rt6, double rt7, double rt8, double rt9, double rt10, double rt11, double rt12, double rt13, double rt14, double rt15, double _z_slide);
	void showRTs();

	void moveCameraZPos(const float move);
	void moveCameraZSlide(const float move);

	void startGL(int _LoopTime, int argc, char *argv[]);



	

	void timer(int value) {
		glutPostRedisplay();
		glutTimerFunc(LoopTime, timer , 0);
	}
	/*****************************************************
	 *   Initialize�֐�
	 ******************************************************/
	void Initialize(void){
		glClearColor(0.0,0.0,0.0,0.0);
		glEnable(GL_DEPTH_TEST);//�f�v�X�o�b�t�@���g�p�FglutInitDisplayMode() �� GLUT_DEPTH ���w�肷��
		// �f�B�X�v���C���X�g���쐬
		listNumber = glGenLists(1);
		glNewList( listNumber, GL_COMPILE );
		glEndList();
		//--------------------------------------
		// �}�E�X�|�C���^�ʒu�̃E�B���h�E���̑��ΓI�ʒu�ւ̊��Z�p
		sx = 1.0 / (double)WindowWidth;
		sy = 1.0 / (double)WindowHeight;
		// ��]�s��̏�����
		//qrot(rt, cq);
		//--------------------------------------

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		//�����ϊ��s��̐ݒ�------------------------------
		glMatrixMode(GL_PROJECTION);//�s�񃂁[�h�̐ݒ�iGL_PROJECTION : �����ϊ��s��̐ݒ�AGL_MODELVIEW�F���f���r���[�ϊ��s��j
		glLoadIdentity();//�s��̏�����
		gluPerspective(15.0, (double)WindowWidth/(double)WindowHeight, 10.0, 5000.0); //�������e�@�̎��̐�gluPerspactive(th, w/h, near, far);
	}
	//----------------------------------------------------
	// �A�C�h�����ɌĂяo�����֐�
	//----------------------------------------------------
	void Idle(){
		glutPostRedisplay(); //glutDisplayFunc()���P����s����
	}

	//----------------------------------------------------
	// ���T�C�Y���ɌĂяo�����֐�
	//----------------------------------------------------
	void Reshape(int _width, int _height){
		WindowWidth = _width;    //��������E�B���h�E�̕�
		WindowHeight= _height;    //��������E�B���h�E�̍���
		Initialize();
	}
	//----------------------------------------------------
	// �}�E�X���[�h�I��
	//----------------------------------------------------
	void mouse_mode(int button, int state){
		if(button==0 || button == 2){// �����E
			if(state==0){ // SET
				if(latest_mouse==-1)latest_mouse=button;
			}else if(state==1){ //FREE
				if(latest_mouse==button)latest_mouse=-1;
			}
		}
	}

	//----------------------------------------------------
	// �}�E�X�h���b�O��
	//----------------------------------------------------
	void mouse_motion(int x, int y){
		double dx, dy, a;

		// �}�E�X�|�C���^�̈ʒu�̃h���b�O�J�n�ʒu����̕ψ�
		dx = (x - cx) * sx;
		dy = (y - cy) * sy;

		// �}�E�X�|�C���^�̈ʒu�̃h���b�O�J�n�ʒu����̋���
		a = sqrt(dx * dx + dy * dy);
		if( a != 0.0 ){
			// �}�E�X�̃h���b�O�ɔ�����]�̃N�H�[�^�j�I�� dq �����߂�
			double ar = a * SCALE * 0.5 * 0.5;
			double as = sin(ar) / a;
			if(latest_mouse==0){
				double dq[4] = { cos(ar), dy * as, 0.0, -dx * as};  // ����̌����ł͂��ꂪ�킩��₷����
				qmul(tq, dq, cq); // ��]�̏����l cq �� dq ���|���ĉ�]������
				qrot(rt, tq);  // �N�H�[�^�j�I�������]�̕ϊ��s������߂�
			}else if(latest_mouse==2){
				double OAxOB = sqrt(( x-WindowWidth/2.0)*( x-WindowWidth/2.0)+( y-WindowHeight/2.0)*( y-WindowHeight/2.0))
					*sqrt((cx-WindowWidth/2.0)*(cx-WindowWidth/2.0)+(cy-WindowHeight/2.0)*(cy-WindowHeight/2.0));
				double sin_rq=((x-WindowWidth/2.0)*(cy-WindowHeight/2.0)-(y-WindowHeight/2.0)*(cx- WindowWidth/2.0))/(OAxOB);
				double cos_rq=((x-WindowWidth/2.0)*(cx- WindowWidth/2.0)+(y-WindowHeight/2.0)*(cy-WindowHeight/2.0))/(OAxOB);

				double dq[4] = {sqrt((1+cos_rq)/2), 0.0, sqrt((1-cos_rq)/2), 0.0};  // ����̌����ł͂��ꂪ�킩��₷����
				if(sin_rq<0)dq[0]*=-1;
				qmul(tq, dq, cq); // ��]�̏����l cq �� dq ����]������
				qrot(rt, tq);  // �N�H�[�^�j�I�������]�̕ϊ��s������߂�
			}
		}
	}
	//----------------------------------------------------
	// �}�E�X�N���b�N��
	//----------------------------------------------------
	void mouse_on(int button, int state, int x, int y)
	{
		switch (button) {
			case 0: // LEFT
				switch (state) {
					case 0: //Down
						// �h���b�O�J�n�_���L�^
						cx = x;
						cy = y;
						break;
					case 1: //up
						// ��]�̕ۑ�
						cq[0] = tq[0];
						cq[1] = tq[1];
						cq[2] = tq[2];
						cq[3] = tq[3];
						break;
					default:
						break;
				}
				break;
			case 1: //Center
				switch (state) {
					case 0:
						// �h���b�O�J�n�_���L�^
						cx = x;
						cy = y;
						break;
					case 1:
						// ��]�̕ۑ�
						cq[0] = tq[0];
						cq[1] = tq[1];
						cq[2] = tq[2];
						cq[3] = tq[3];
						break;
					default:
						break;
				}
				std::cout << "here" << std::endl;
				if(latest_mouse==0)plot++;
				break;
			case 2: // Right
				switch (state) {
					case 0:
						// �h���b�O�J�n�_���L�^
						cx = x;
						cy = y;
						break;
					case 1:
						// ��]�̕ۑ�
						cq[0] = tq[0];
						cq[1] = tq[1];
						cq[2] = tq[2];
						cq[3] = tq[3];
						break;
					default:
						break;
				}
				std::cout << "here?" << std::endl;
				if(plot>1 && latest_mouse==0)plot--;
				break;
			case 3: //Center move
				dist-=1;
				break;
			case 4: //Center move
				dist+=1;
				break;
			default:
				std::cout << "HERE" << button << std::endl;
				break;
		}
		std::cout << x << " " << y<<std::endl;
		mouse_mode(button,state);
	}

	/*****************************************************
	 *   ��n�̕`��
	 ******************************************************/
	void Ground(void) {

		double ground_max_x = 400.0;
		double ground_max_y = 400.0;
		double ground_z=0.0;
		double ground_step=50.0;

		glColor3d(0.8, 0.8, 0.8);  // ��n�̐F
		glBegin(GL_LINES);
		for(double ly = -ground_max_y ;ly <= ground_max_y; ly+=ground_step){
			glVertex3d(-ground_max_x, ly,ground_z);
			glVertex3d(ground_max_x, ly,ground_z);
		}
		for(double lx = -ground_max_x ;lx <= ground_max_x; lx+=ground_step){
			glVertex3d(lx, ground_max_y,ground_z);
			glVertex3d(lx, -ground_max_y,ground_z);
		}
		glEnd();
	}

	//////////////////////////////////////////////////////////////////////////
	// �}�E�X�h���b�O�ɂ���]
	//////////////////////////////////////////////////////////////////////////
	// �N�H�[�^�j�I���̐� r <- p x q
	void qmul(double r[], const double p[], const double q[])
	{
		r[0] = p[0] * q[0] - p[1] * q[1] - p[2] * q[2] - p[3] * q[3];
		r[1] = p[0] * q[1] + p[1] * q[0] + p[2] * q[3] - p[3] * q[2];
		r[2] = p[0] * q[2] - p[1] * q[3] + p[2] * q[0] + p[3] * q[1];
		r[3] = p[0] * q[3] + p[1] * q[2] - p[2] * q[1] + p[3] * q[0];
	}

	// ��]�̕ϊ��s�� r <- �N�H�[�^�j�I�� q
	static void qrot(double r[], double q[]){
		double x2 = q[1] * q[1] * 2.0;
		double y2 = q[2] * q[2] * 2.0;
		double z2 = q[3] * q[3] * 2.0;
		double xy = q[1] * q[2] * 2.0;
		double yz = q[2] * q[3] * 2.0;
		double zx = q[3] * q[1] * 2.0;
		double xw = q[1] * q[0] * 2.0;
		double yw = q[2] * q[0] * 2.0;
		double zw = q[3] * q[0] * 2.0;

		r[ 0] = 1.0 - y2 - z2;  r[ 1] = xy + zw;       r[ 2] = zx - yw;
		r[ 4] = xy - zw;        r[ 5] = 1.0 - z2 - x2; r[ 6] = yz + xw;
		r[ 8] = zx + yw;        r[ 9] = yz - xw;       r[10] = 1.0 - x2 - y2;
		r[ 3] = r[ 7] = r[11] = r[12] = r[13] = r[14] = 0.0;
		r[15] = 1.0;
	}

	bool hsv2rgbColor(const int h_tmp, const int maxL,const int minL, const double s, const double v){
		double h = (h_tmp-minL)*360/(maxL-minL);
		int hi=int(h/60);
		double F=(h-hi*60)/60;
		double M=v*(255-s)/255;
		double N=v*(255-s*F)/255;
		double K=v*(255-s*(1-F))/255;
		switch(hi){
			case 0:
				glColor3d(M/255.0,K/255.0,v/255.0);
				break;
				//return Scalar(M,K,v);
			case 1:
				glColor3d(M/255.0,v/255.0,N/255.0);
				break;
				//return Scalar(M,v,N);
			case 2:
				glColor3d(K/255.0,v/255.0,M/255.0);
				break;
				//return Scalar(K,v,M);
			case 3:
				glColor3d(v/255.0,N/255.0,M/255.0);
				break;
				//return Scalar(v,N,M);
			case 4:
				glColor3d(v/255.0,M/255.0,K/255.0);
				break;
				//return Scalar(v,M,K);
			case 5:
				glColor3d(N/255.0,M/255.0,v/255.0);
				break;
				//return Scalar(N,M,v);
			default:
				return false;
				break;
		}
		return true;
	}

	bool hsv2rgb(double &r, double &g, double &b, const int h_tmp, const int maxL,const int minL, const double s, const double v){
		double h = (h_tmp-minL)*360/(maxL-minL);
		int hi=int(h/60);
		double F=(h-hi*60)/60;
		double M=v*(255-s)/255;
		double N=v*(255-s*F)/255;
		double K=v*(255-s*(1-F))/255;
		switch(hi){
			case 0:
				r=M/255.0;
				g=K/255.0;
				b=v/255.0;
				break;
			case 1:
				r=M/255.0;
				g=v/255.0;
				b=N/255.0;
				break;
			case 2:
				r=K/255.0;
				g=v/255.0;
				b=M/255.0;
				break;
			case 3:
				r=v/255.0;
				g=N/255.0;
				b=M/255.0;
				break;
			case 4:
				r=v/255.0;
				g=M/255.0;
				b=K/255.0;
				break;
			case 5:
				r=N/255.0;
				g=M/255.0;
				b=v/255.0;
				break;
			default:
				r=0;
				g=0;
				b=0;
				return false;
				break;
		}
		return true;
	}

	void startGL(int _LoopTime, int argc, char *argv[]){

		LoopTime=_LoopTime;

		//�`��֐�
		glutInit(&argc, argv);//���̏�����
		glutInitWindowPosition(WindowPositionX, WindowPositionY);//�E�B���h�E�̈ʒu�̎w��
		//for(int i=0; i<GL_COUNT; ++i){
		glutInitWindowSize(WindowWidth, WindowHeight); //�E�B���h�E�T�C�Y�̎w��
		glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);//�f�B�X�v���C���[�h�̎w��
		WinID[0]=glutCreateWindow(WindowName[0]);  //�E�B���h�E�̍쐬
		glutDisplayFunc(Display); //�`�掞�ɌĂяo�����֐����w�肷��i�֐����FDisplay�j
		glutMouseFunc(mouse_on);      //�}�E�X�N���b�N���ɌĂяo�����֐�
		glutMotionFunc(mouse_motion);      //�}�E�X�h���b�O�������ɌĂяo�����֐�
		glutKeyboardFunc(keyboard);
		glutSpecialFunc(specialkeyboard);

		glutReshapeFunc(Reshape);

		if(LoopTime>0)glutTimerFunc( LoopTime, timer , 0); // �Œ莞�ԃ��[�v
		else glutIdleFunc(Idle);       // �������[�v, Idle:�v���O�����A�C�h����Ԏ��ɌĂяo�����֐�

		Initialize(); //�����ݒ�̊֐����Ăяo��
		glutMainLoop();


	}

	void setRTs(double rt0, double rt1, double rt2, double rt3, double rt4, double rt5, double rt6, double rt7, double rt8, double rt9, double rt10, double rt11, double rt12, double rt13, double rt14, double rt15, double _z_slide){
		rt[0]=rt0;   rt[1]=rt1;   rt[2]=rt2;   rt[3]=rt3;
		rt[4]=rt4;   rt[5]=rt5;   rt[6]=rt6;   rt[7]=rt7;
		rt[8]=rt8;   rt[9]=rt9;   rt[10]=rt10; rt[11]=rt11;
		rt[12]=rt12; rt[13]=rt13; rt[14]=rt14; rt[15]=rt15;
		z_slide=_z_slide;//*/
	}

	void showRTs(){
		for(size_t i=0; i<16;++i){
			std::cout << GLSpace::rt[i] << std::endl;
		}
		std::cout << "z" << GLSpace::z_slide << std::endl;
	}

	void moveCameraZPos(const float move){camera_z_pos+=move;}
	void moveCameraZSlide(const float move){z_slide+=move;}

	/*****************************************************
	 *   �_�Q��\������֐�
	 ******************************************************/
	// �����Ŏ������邱��
	// void Display(void)
	// ���

#define POINTWIDTH 18
#define POINTHEIGHT 17
	
	void Display(void) {

		glClearColor(0.0,0.0,0.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�o�b�t�@�̏���
		//���f���r���[�ϊ��s��̐ݒ�--------------------------
		glMatrixMode(GL_MODELVIEW);//�s�񃂁[�h�̐ݒ�iGL_PROJECTION : �����ϊ��s��̐ݒ�AGL_MODELVIEW�F���f���r���[�ϊ��s��j
		glLoadIdentity();//�s��̏�����
		glViewport(0, 0, WindowWidth, WindowHeight);
		//----------------------------------------------
		glPushMatrix();
		//���_�̐ݒ�------------------------------
		gluLookAt(
				-100.0, 80.0, -100.0, // ���_�̈ʒux,y,z;
				0.0, 0.0,  camera_z_pos,   // ���E�̒��S�ʒu�̎Q�Ɠ_���Wx,y,z
				0.0, 0.0, -1.0);  //���E�̏�����̃x�N�g��x,y,z
		gluLookAt(
				0.0, dist, 0.0, // ���_�̈ʒux,y,z;
				0.0, 0.0,  camera_z_pos,   // ���E�̒��S�ʒu�̎Q�Ɠ_���Wx,y,z
				0.0, 0.0, -1.0);  //���E�̏�����̃x�N�g��x,y,z
		//----------------------------------------
		////��]///////////////////////////////////////////////
		glMultMatrixd(rt);
		///////////////////////////////////////////////////////
		glPointSize(plot);
		glBegin(GL_POINTS);
		
	
		/*	
		   if(colorflag)glColor3d(0.0,0.0,0.0);
		   else glColor3d(1.0,1.0,1.0);
		   glVertex3d(0.0,0.0,0.0);//���_�̃m�C�Y����
		   for(int i=0;i<x_global.size();i++){
		   glColor3d(1.0,1.0,0.0);
		   hsv2rgbColor(z_global[i]);
		   glVertex3d(x_global[i]/l_scale,y_global[i]/l_scale,z_global[i]/z_scale-z_slide);

		   }
		*/
	

			
		//glEnd();
		Ground();
		glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)�Ń_�u���o�b�t�@�����O�𗘗p��

	}
	/*****************************************************
	 *   �L�[�{�[�h����
	 ******************************************************/
	// �����Ŏ������邱�� 
	// void keyboard(unsigned char key, int x, int y)
	// ���

	void keyboard(unsigned char key, int x, int y){
		switch ( key )
		{
			case 'a':
				break;
			case 'q':
				exit(0);
				break;
			default:
				break;
		}
	}

	void specialkeyboard(int key, int x, int y){
		switch ( key )
		{
			case GLUT_KEY_UP:

				break;
			case GLUT_KEY_DOWN:
				break;
			case GLUT_KEY_LEFT:
				break;
			case GLUT_KEY_RIGHT:
				break;
			default:
				break;
		}
	}


	void DrawString(const char *str,void *font,float x,float y,float z){
		glRasterPos3f(x,y,z);

		while(*str){
			glutBitmapCharacter(font, *str);
			++str;
		}
	}




	/***************************************************/
	/***************       �����\��        *************/
	/***************************************************/
	class GLFONT
	{
		public:
			HFONT Hfont;
			HDC Hdc;
			GLFONT(wchar_t *fontname, int size);
			void DrawStringW(int x,int y,int z,wchar_t *format, ...);
	}*font;
	//�R���X�g���N�^ �t�H���g�쐬
	GLFONT::GLFONT(wchar_t *fontname, int size)
	{
		Hfont = CreateFontW(
				size,      //�t�H���g����
				0,       //������
				0,       //�e�L�X�g�̊p�x
				0,       //�x�[�X���C���Ƃ����Ƃ̊p�x
				FW_REGULAR,     //�t�H���g�̑���
				FALSE,      //�C�^���b�N��
				FALSE,      //�A���_�[���C��
				FALSE,      //�ł�������
				SHIFTJIS_CHARSET,   //�����Z�b�g
				OUT_DEFAULT_PRECIS,   //�o�͐��x
				CLIP_DEFAULT_PRECIS,  //�N���b�s���O���x
				ANTIALIASED_QUALITY,  //�o�͕i��
				FIXED_PITCH | FF_MODERN, //�s�b�`�ƃt�@�~���[
				fontname);     //���̖�

		Hdc = wglGetCurrentDC();
		SelectObject(Hdc, Hfont);
	}
	//���C�h������̕`��
	void GLFONT::DrawStringW(int x,int y, int z, wchar_t *format, ...)
	{
		wchar_t buf[256];
		va_list ap;
		int Length=0;
		int list=0;

		//�|�C���^��NULL�̏ꍇ�͏I��
		if ( format == NULL )
			return;

		//������ϊ�
		va_start(ap, format);
		vswprintf_s(buf, format, ap);
		va_end(ap);

		Length = wcslen(buf);
		list = glGenLists(Length);
		for( int i=0; i<Length; i++ ){
			wglUseFontBitmapsW(Hdc, buf[i], 1, list + (DWORD)i);
		}

		//glDisable(GL_LIGHTING);
		glRasterPos3i(x, y,z);
		//�f�B�X�v���C���X�g�ŕ`��
		for( int i=0; i<Length; i++ )
		{
			glCallList(list + i);
		}
		//glEnable(GL_LIGHTING);
		//�f�B�X�v���C���X�g�j��
		glDeleteLists(list, Length);
		list = 0;
		Length = 0;
	}
	};	
