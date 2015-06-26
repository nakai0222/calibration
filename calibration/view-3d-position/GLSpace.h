#pragma once

#include <cmath>
#include <iostream>
//#include <GL/glut.h>
//#include <GL/gl.h>
#include <GL/freeglut.h> 


#define PLOTSIZE (2)
#define SCALE (2.0 * 3.14159265358979323846)  // マウスの相対位置→回転角の換算係数


namespace GLSpace {

	int LoopTime=0;

	//--------------------//
	//   複数スレッド用   //
	//--------------------//
	#define GL_COUNT 2
	int WinID[GL_COUNT]; //ウィンドウID
	const char *WindowName[]={"Window 1", "Window 2"};

	/*****************************************************
	 *   描画関数の変数初期化
	 ******************************************************/
	int WindowPositionX = 800;  //生成するウィンドウ位置のX座標
	int WindowPositionY = 0;  //生成するウィンドウ位置のY座標
	int WindowWidth = 800;    //生成するウィンドウの幅
	int WindowHeight= 800;    //生成するウィンドウの高さ
	char WindowTitle[] = "Result";  //ウィンドウのタイトル
	int plot=PLOTSIZE;
	int cx, cy;                // ドラッグ開始位置
	double sx, sy;              // マウスの絶対位置→ウィンドウ内での相対位置の換算係数
	double cq[4] = { 1.0, 0.0, 0.0, 0.0 };  // 回転の初期値 (クォータニオン)
	double tq[4];              // ドラッグ中の回転 (クォータニオン)
	double rt[16];              // 回転の変換行列

	unsigned int listNumber;
	float camera_z_pos =0.0;
	float dist =2000.0f;
	int latest_mouse=-1;


	double l_scale =1.0;
	double z_scale =1.0;
	double z_slide =500/z_scale;

	/*****************************************************
	 *   描画に関する関数のプロトタイプ宣言
	 ******************************************************/

	void timer(int value);
	void Initialize(void);
	void Idle(void);
	void Reshape(int _width, int _height);
	void Display(void);
	void Ground(void);  //大地の描画

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
	 *   Initialize関数
	 ******************************************************/
	void Initialize(void){
		glClearColor(0.0,0.0,0.0,0.0);
		glEnable(GL_DEPTH_TEST);//デプスバッファを使用：glutInitDisplayMode() で GLUT_DEPTH を指定する
		// ディスプレイリストを作成
		listNumber = glGenLists(1);
		glNewList( listNumber, GL_COMPILE );
		glEndList();
		//--------------------------------------
		// マウスポインタ位置のウィンドウ内の相対的位置への換算用
		sx = 1.0 / (double)WindowWidth;
		sy = 1.0 / (double)WindowHeight;
		// 回転行列の初期化
		//qrot(rt, cq);
		//--------------------------------------

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		//透視変換行列の設定------------------------------
		glMatrixMode(GL_PROJECTION);//行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
		glLoadIdentity();//行列の初期化
		gluPerspective(15.0, (double)WindowWidth/(double)WindowHeight, 10.0, 5000.0); //透視投影法の視体積gluPerspactive(th, w/h, near, far);
	}
	//----------------------------------------------------
	// アイドル時に呼び出される関数
	//----------------------------------------------------
	void Idle(){
		glutPostRedisplay(); //glutDisplayFunc()を１回実行する
	}

	//----------------------------------------------------
	// リサイズ時に呼び出される関数
	//----------------------------------------------------
	void Reshape(int _width, int _height){
		WindowWidth = _width;    //生成するウィンドウの幅
		WindowHeight= _height;    //生成するウィンドウの高さ
		Initialize();
	}
	//----------------------------------------------------
	// マウスモード選択
	//----------------------------------------------------
	void mouse_mode(int button, int state){
		if(button==0 || button == 2){// 左か右
			if(state==0){ // SET
				if(latest_mouse==-1)latest_mouse=button;
			}else if(state==1){ //FREE
				if(latest_mouse==button)latest_mouse=-1;
			}
		}
	}

	//----------------------------------------------------
	// マウスドラッグ時
	//----------------------------------------------------
	void mouse_motion(int x, int y){
		double dx, dy, a;

		// マウスポインタの位置のドラッグ開始位置からの変位
		dx = (x - cx) * sx;
		dy = (y - cy) * sy;

		// マウスポインタの位置のドラッグ開始位置からの距離
		a = sqrt(dx * dx + dy * dy);
		if( a != 0.0 ){
			// マウスのドラッグに伴う回転のクォータニオン dq を求める
			double ar = a * SCALE * 0.5 * 0.5;
			double as = sin(ar) / a;
			if(latest_mouse==0){
				double dq[4] = { cos(ar), dy * as, 0.0, -dx * as};  // 現状の見方ではこれがわかりやすいか
				qmul(tq, dq, cq); // 回転の初期値 cq に dq を掛けて回転を合成
				qrot(rt, tq);  // クォータニオンから回転の変換行列を求める
			}else if(latest_mouse==2){
				double OAxOB = sqrt(( x-WindowWidth/2.0)*( x-WindowWidth/2.0)+( y-WindowHeight/2.0)*( y-WindowHeight/2.0))
					*sqrt((cx-WindowWidth/2.0)*(cx-WindowWidth/2.0)+(cy-WindowHeight/2.0)*(cy-WindowHeight/2.0));
				double sin_rq=((x-WindowWidth/2.0)*(cy-WindowHeight/2.0)-(y-WindowHeight/2.0)*(cx- WindowWidth/2.0))/(OAxOB);
				double cos_rq=((x-WindowWidth/2.0)*(cx- WindowWidth/2.0)+(y-WindowHeight/2.0)*(cy-WindowHeight/2.0))/(OAxOB);

				double dq[4] = {sqrt((1+cos_rq)/2), 0.0, sqrt((1-cos_rq)/2), 0.0};  // 現状の見方ではこれがわかりやすいか
				if(sin_rq<0)dq[0]*=-1;
				qmul(tq, dq, cq); // 回転の初期値 cq に dq を回転を合成
				qrot(rt, tq);  // クォータニオンから回転の変換行列を求める
			}
		}
	}
	//----------------------------------------------------
	// マウスクリック時
	//----------------------------------------------------
	void mouse_on(int button, int state, int x, int y)
	{
		switch (button) {
			case 0: // LEFT
				switch (state) {
					case 0: //Down
						// ドラッグ開始点を記録
						cx = x;
						cy = y;
						break;
					case 1: //up
						// 回転の保存
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
						// ドラッグ開始点を記録
						cx = x;
						cy = y;
						break;
					case 1:
						// 回転の保存
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
						// ドラッグ開始点を記録
						cx = x;
						cy = y;
						break;
					case 1:
						// 回転の保存
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
	 *   大地の描画
	 ******************************************************/
	void Ground(void) {

		double ground_max_x = 400.0;
		double ground_max_y = 400.0;
		double ground_z=0.0;
		double ground_step=50.0;

		glColor3d(0.8, 0.8, 0.8);  // 大地の色
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
	// マウスドラッグによる回転
	//////////////////////////////////////////////////////////////////////////
	// クォータニオンの積 r <- p x q
	void qmul(double r[], const double p[], const double q[])
	{
		r[0] = p[0] * q[0] - p[1] * q[1] - p[2] * q[2] - p[3] * q[3];
		r[1] = p[0] * q[1] + p[1] * q[0] + p[2] * q[3] - p[3] * q[2];
		r[2] = p[0] * q[2] - p[1] * q[3] + p[2] * q[0] + p[3] * q[1];
		r[3] = p[0] * q[3] + p[1] * q[2] - p[2] * q[1] + p[3] * q[0];
	}

	// 回転の変換行列 r <- クォータニオン q
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

		//描画関数
		glutInit(&argc, argv);//環境の初期化
		glutInitWindowPosition(WindowPositionX, WindowPositionY);//ウィンドウの位置の指定
		//for(int i=0; i<GL_COUNT; ++i){
		glutInitWindowSize(WindowWidth, WindowHeight); //ウィンドウサイズの指定
		glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);//ディスプレイモードの指定
		WinID[0]=glutCreateWindow(WindowName[0]);  //ウィンドウの作成
		glutDisplayFunc(Display); //描画時に呼び出される関数を指定する（関数名：Display）
		glutMouseFunc(mouse_on);      //マウスクリック時に呼び出される関数
		glutMotionFunc(mouse_motion);      //マウスドラッグ解除時に呼び出される関数
		glutKeyboardFunc(keyboard);
		glutSpecialFunc(specialkeyboard);

		glutReshapeFunc(Reshape);

		if(LoopTime>0)glutTimerFunc( LoopTime, timer , 0); // 固定時間ループ
		else glutIdleFunc(Idle);       // 自動ループ, Idle:プログラムアイドル状態時に呼び出される関数

		Initialize(); //初期設定の関数を呼び出す
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
	 *   点群を表示する関数
	 ******************************************************/
	// 自分で実装すること
	// void Display(void)
	// 一例

#define POINTWIDTH 18
#define POINTHEIGHT 17
	
	void Display(void) {

		glClearColor(0.0,0.0,0.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //バッファの消去
		//モデルビュー変換行列の設定--------------------------
		glMatrixMode(GL_MODELVIEW);//行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
		glLoadIdentity();//行列の初期化
		glViewport(0, 0, WindowWidth, WindowHeight);
		//----------------------------------------------
		glPushMatrix();
		//視点の設定------------------------------
		gluLookAt(
				-100.0, 80.0, -100.0, // 視点の位置x,y,z;
				0.0, 0.0,  camera_z_pos,   // 視界の中心位置の参照点座標x,y,z
				0.0, 0.0, -1.0);  //視界の上方向のベクトルx,y,z
		gluLookAt(
				0.0, dist, 0.0, // 視点の位置x,y,z;
				0.0, 0.0,  camera_z_pos,   // 視界の中心位置の参照点座標x,y,z
				0.0, 0.0, -1.0);  //視界の上方向のベクトルx,y,z
		//----------------------------------------
		////回転///////////////////////////////////////////////
		glMultMatrixd(rt);
		///////////////////////////////////////////////////////
		glPointSize(plot);
		glBegin(GL_POINTS);
		
	
		/*	
		   if(colorflag)glColor3d(0.0,0.0,0.0);
		   else glColor3d(1.0,1.0,1.0);
		   glVertex3d(0.0,0.0,0.0);//原点のノイズ除去
		   for(int i=0;i<x_global.size();i++){
		   glColor3d(1.0,1.0,0.0);
		   hsv2rgbColor(z_global[i]);
		   glVertex3d(x_global[i]/l_scale,y_global[i]/l_scale,z_global[i]/z_scale-z_slide);

		   }
		*/
	

			
		//glEnd();
		Ground();
		glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可

	}
	/*****************************************************
	 *   キーボード入力
	 ******************************************************/
	// 自分で実装すること 
	// void keyboard(unsigned char key, int x, int y)
	// 一例

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
	/***************       文字表示        *************/
	/***************************************************/
	class GLFONT
	{
		public:
			HFONT Hfont;
			HDC Hdc;
			GLFONT(wchar_t *fontname, int size);
			void DrawStringW(int x,int y,int z,wchar_t *format, ...);
	}*font;
	//コンストラクタ フォント作成
	GLFONT::GLFONT(wchar_t *fontname, int size)
	{
		Hfont = CreateFontW(
				size,      //フォント高さ
				0,       //文字幅
				0,       //テキストの角度
				0,       //ベースラインとｘ軸との角度
				FW_REGULAR,     //フォントの太さ
				FALSE,      //イタリック体
				FALSE,      //アンダーライン
				FALSE,      //打ち消し線
				SHIFTJIS_CHARSET,   //文字セット
				OUT_DEFAULT_PRECIS,   //出力精度
				CLIP_DEFAULT_PRECIS,  //クリッピング精度
				ANTIALIASED_QUALITY,  //出力品質
				FIXED_PITCH | FF_MODERN, //ピッチとファミリー
				fontname);     //書体名

		Hdc = wglGetCurrentDC();
		SelectObject(Hdc, Hfont);
	}
	//ワイド文字列の描画
	void GLFONT::DrawStringW(int x,int y, int z, wchar_t *format, ...)
	{
		wchar_t buf[256];
		va_list ap;
		int Length=0;
		int list=0;

		//ポインタがNULLの場合は終了
		if ( format == NULL )
			return;

		//文字列変換
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
		//ディスプレイリストで描画
		for( int i=0; i<Length; i++ )
		{
			glCallList(list + i);
		}
		//glEnable(GL_LIGHTING);
		//ディスプレイリスト破棄
		glDeleteLists(list, Length);
		list = 0;
		Length = 0;
	}
	};	
