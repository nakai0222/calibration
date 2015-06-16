#include "glutGraph.h"

int main(int argc, char* argv[]){
	// glutGraph���쐬�B�����̐���2D�̂�/3D�̂�/2D3D������I���ł���B
	//    �ȉ���640x480�T�C�Y�A100�_�̃��O��\������2D�E�B���h�E�ƁA
	//         512x512�T�C�Y��3D�E�B���h�E�𐶐������B
	glutGraph graph(&argc, argv, 640, 480, 100, 512, 512);

	// 2D�̃O���t��2�ǉ�
	//  �@�ȉ���RGB=(1.0,0.0,0.0)=�Ԃ̃O���t��+0.5=��̂ق���0.2�{�̐U���ŕ\�����A
	//    ������RGB=(0.0,1.0,0.0)=�΂̃O���t��-0.5=���̂ق���0.3�{�̐U���ŕ\�������B
	graph.pushGraph2D(1.0, 0.0, 0.0,  0.5, 0.2);
	graph.pushGraph2D(0.0, 1.0, 0.0, -0.5, 0.3);

	// 2D�ŕ\������T���v���f�[�^�̗p�ӁB
	//    �ȉ��ł�2�O���t������̂�data_2D��2�̗v�f��ǉ����Ă���i�����l��0.0�Ƃ��Ă���j�B
	std::vector<double> data_2D;
	data_2D.push_back(0.0);
	data_2D.push_back(0.0);

	// 3D�̓_�Q�O���[�v��2�ǉ�
	//    �ȉ���RGB=(1.0,1.0,1.0)=���̓_�Q�O���[�v��_�ŕ\�����A
	//    �����RGB=(0.0,1.0,1.0)=�V�A���̓_�Q�O���[�v�����ŕ\�������B
	graph.pushGraph3D(1.0, 1.0, 1.0, GL_POINTS);
	graph.pushGraph3D(0.0, 1.0, 1.0, GLUTGRAPH_SPHERE);
	
	// 3D�ŕ\������T���v���f�[�^�̗p�ӁB
	//    �ȉ��ł�2�_�Q�O���[�v������̂�data_3D��2�̗v�f��ǉ����Ă���B
	//    data_3D[0]�͈�_�݂̂������Adata_3D[1]�͉~��ɕ��񂾓_�����Ƃ��Ă���B
	graphPoint3D tmp_3D;
	std::vector<graphPoint3D> group_3D;
	std::vector<std::vector<graphPoint3D>> data_3D;
	group_3D.push_back({0.0,0.0,0.0});
	data_3D.push_back(group_3D);
	group_3D.clear();
	for (int i = 0; i < 360; i++){
		tmp_3D.x = 30*cos(i/180.0*M_PI);
		tmp_3D.y = 30*sin(i/180.0*M_PI);
		tmp_3D.z = -10*cos(i / 180.0*M_PI);
		group_3D.push_back(tmp_3D);
	}
	data_3D.push_back(group_3D);

	graph.pushData3D(0, data_3D[0]);
	graph.pushData3D(1, data_3D[1]);

	double t = 0;
	while (1){
		// 2D�̃O���t�͑S�O���t�𓯎��ɍX�V����d�l�B
		// ��ł̓O���t��2�i�v�f�ԍ�[0],[1]�ɑΉ��j�Ȃ̂ŁA
		// data_2D[0],data_2D[1]���X�V���ĕ\���B���O�𒴂����f�[�^�͗���Ă����B
		data_2D[0] = sin(  t/ 180 * M_PI);
		data_2D[1] = cos(2*t/ 180 * M_PI);
		graph.pushData2D(data_2D);

		// 3D�̃O���t�͓_�Q�O���[�v���ɍX�V����d�l�B
		// ��ł͓_�Q�O���[�v��2�i������0,1�ɑΉ��j�ŁA
		// data_3D[0]�����X�V�i�_��ǉ��j���ĕ\�����Ă���B
		tmp_3D.x = 30 * sin(  t / 180 * M_PI);
		tmp_3D.y = 30 * sin(2.18*t / 180 * M_PI);
		tmp_3D.z = 30 * cos(3.49*t / 180 * M_PI);
		data_3D[0].push_back(tmp_3D);
		graph.pushData3D(0, data_3D[0]);

		t++;
	}
	return 0;
}
